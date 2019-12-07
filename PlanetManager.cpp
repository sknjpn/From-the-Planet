﻿#include "PlanetManager.h"
#include "Region.h"
#include "Chip.h"
#include "Road.h"
#include "FacilityAsset.h"
#include "FacilityState.h"
#include "TruckState.h"
#include "AssetManager.h"
#include "ItemAsset.h"
#include "TerrainAsset.h"
#include "FacilitiesListViewer.h"
#include "PlanetViewer.h"
#include "GameOverViewer.h"
#include "ClearViewer.h"
#include "SoundManager.h"

unique_ptr<PlanetManager> g_planetManagerPtr;

const shared_ptr<FacilityState>& PlanetManager::makeFacility(const shared_ptr<FacilityAsset> facilityAsset, const shared_ptr<Region> region)
{
	auto& state = m_facilityStates.emplace_back(facilityAsset->makeState());
	state->m_region = region;
	state->m_facilityAsset = facilityAsset;

	region->m_facilityState = state;

	return state;
}

void PlanetManager::generateRegions(size_t n)
{
	Array<Vec3> positions;

	for (int i = 0; i < n - 1; ++i)
		positions.emplace_back(RandomVec3(1.0));

	double f_max = 2;
	double f_min = 0;
	double p_min = 0;
	double p_max = 0;
	double r = 0.01;
	LineString ls_max;
	LineString ls_min;
	while (System::Update())
	{
		if (KeyUp.down()) r *= 2;
		if (KeyDown.down()) r /= 2;

		static Font font(12);
		font(U"f_max=", f_max).draw(0, 0);
		font(U"f_min=", f_min).draw(0, 16);
		font(U"r=", r).draw(0, 32);

		ls_max.draw(Palette::Red);
		ls_min.draw(Palette::Blue);

		if (KeyEnter.down()) break;

		f_max = 0;
		f_min = 2;
		for (int i = 0; i < n; ++i)
		{
			auto& p1 = positions[i];

			double min = 2;
			for (int j = 0; j < n; ++j)
			{
				auto& p2 = positions[j];

				if (i == j) continue;
				else if (p1 == p2)
				{
					p2 = RandomVec3(1.0);

					continue;
				}

				auto d = p2 - p1;

				min = Min(min, d.length());

				p1 -= r * 0.01 * d.normalized();
				p1.normalize();
				p2 += r * 0.01 * d.normalized();
				p2.normalize();
			}

			f_max = Max(f_max, min);
			f_min = Min(f_min, min);
		}

		//if (p_min > f_min && p_max < f_max) r *= 0.5;

		p_min = f_min;
		p_max = f_max;

		ls_max.emplace_back(ls_max.size(), f_max * 256);
		ls_min.emplace_back(ls_min.size(), f_min * 256);

		Logger << U"MAX:" << f_max;
		Logger << U"MIN:" << f_min;
		Logger << U"R:" << r;
		Logger << U"---";
	}

	// Regionの登録と、半径の分だけ拡大
	for (const auto& p : positions)
		m_regions.emplace_back(MakeShared<Region>())->m_position = p * m_radius;
}

void PlanetManager::connectRegions()
{
	double f_max = 0;
	for (const auto& r1 : m_regions)
	{
		double min = 2.0 * m_radius;
		for (const auto& r2 : m_regions)
			if (r1 != r2) min = Min(min, (r1->getPosition() - r2->getPosition()).length());

		f_max = Max(f_max, min);
	}

	for (auto& r1 : m_regions)
	{
		for (auto& r2 : m_regions)
		{
			if (r1 == r2) continue;

			if (r1->getPosition().distanceFrom(r2->getPosition()) < f_max * 2)
				r1->m_connecteds.emplace_back(r2);
		}
	}
}

// Using for KDTree
struct RegionAdpater : KDTreeAdapter<Array<shared_ptr<Region>>, Vec3, double, 3>
{
	static const element_type* GetPointer(const point_type& point)
	{
		return &point.x;
	}

	static element_type GetElement(const dataset_type& dataset, size_t index, size_t dim)
	{
		return dataset[index]->getPosition().elem(dim);
	}

	static element_type DistanceSq(const dataset_type& dataset, size_t index, const element_type* other)
	{
		return dataset[index]->getPosition().distanceFromSq(Vec3(other[0], other[1], other[2]));
	}
};

void PlanetManager::makeChips()
{
	KDTree<RegionAdpater> kdtree(m_regions);
	for (auto& r1 : m_regions)
	{
		for (auto& r2 : r1->m_connecteds)
		{
			if (&*r1 > &* r2.lock()) continue;

			for (auto& r3 : r2.lock()->m_connecteds)
			{
				if (&*r2.lock() > &* r3.lock()) continue;

				if (r1 == r3.lock()) continue;
				for (auto& r4 : r3.lock()->m_connecteds)
				{
					if (r1 == r4.lock())
					{
						// 外心に最も近い３点が三角形の頂点であることの確認
						const Array<Vec3> positions = { r1->getPosition(), r2.lock()->getPosition(), r3.lock()->getPosition() };
						if (kdtree.knnSearch(3, GetCircumcenter(positions)).all([this, r1, r2, r3](auto index) { return m_regions[index] == r1 || m_regions[index] == r2.lock() || m_regions[index] == r3.lock(); }))
						{
							auto& t = m_chips.emplace_back(MakeShared<Chip>(r1, r2.lock(), r3.lock()));
							r1->m_polygon.emplace_back(t->m_circumcenter);
							r2.lock()->m_polygon.emplace_back(t->m_circumcenter);
							r3.lock()->m_polygon.emplace_back(t->m_circumcenter);
						}
					}
				}
			}
		}
	}

	// Regionの繋がりをChipをもとに修正
	for (auto& r : m_regions) r->m_connecteds.clear();
	for (auto& c : m_chips)
	{
		c->m_r0->connect(c->m_r1);
		c->m_r0->connect(c->m_r2);
		c->m_r1->connect(c->m_r0);
		c->m_r1->connect(c->m_r2);
		c->m_r2->connect(c->m_r0);
		c->m_r2->connect(c->m_r1);
	}

	// RegionのPolygonを時計回りに修正
	for (auto& r : m_regions)
	{
		for (int i = 0; i < r->m_polygon.size() - 1; ++i)
		{
			auto p1 = r->m_polygon[i];

			int best = i;
			for (int j = i + 1; j < r->m_polygon.size(); ++j)
				if ((p1 - r->getPosition()).cross(r->m_polygon[j] - r->getPosition()).dot(r->getPosition()) > 0 &&
					(best == i || (r->m_polygon[best] - r->getPosition()).dot(p1 - r->getPosition()) < (r->m_polygon[j] - r->getPosition()).dot(p1 - r->getPosition())))
					best = j;

			auto pb = r->m_polygon[best];
			r->m_polygon[best] = r->m_polygon[i + 1];
			r->m_polygon[i + 1] = pb;
		}
	}
}

void PlanetManager::setTerrains()
{
	PerlinNoise noise(Random(INT_MAX));

	for (const auto& r : m_regions)
	{
		auto height = Abs(noise.octaveNoise(r->getPosition() * 0.01, 5)) - 0.2;

		if (height > 0.5)
		{
			r->m_terrainAsset = g_assetManagerPtr->getAsset<TerrainAsset>(U"氷河");
		}
		else if (height > 0.3)
		{
			r->m_terrainAsset = g_assetManagerPtr->getAsset<TerrainAsset>(U"岩地");
		}
		else if (height > 0.0)
		{
			if (RandomBool(0.95)) r->m_terrainAsset = g_assetManagerPtr->getAsset<TerrainAsset>(U"草原");
			else r->m_terrainAsset = g_assetManagerPtr->getAsset<TerrainAsset>(U"砂漠");
		}
		else
		{
			r->m_terrainAsset = g_assetManagerPtr->getAsset<TerrainAsset>(U"海洋");
		}
	}

	{
		auto t1 = g_assetManagerPtr->getAsset<TerrainAsset>(U"海洋");
		auto t2 = g_assetManagerPtr->getAsset<TerrainAsset>(U"近海");
		for (const auto& r1 : m_regions)
			if (r1->m_terrainAsset != t1 && r1->m_terrainAsset != t2)
				for (const auto& r2 : r1->m_connecteds)
					if (r2.lock()->m_terrainAsset == t1) r2.lock()->m_terrainAsset = t2;
	}

	{
		auto t1 = g_assetManagerPtr->getAsset<TerrainAsset>(U"草原");
		auto t2 = g_assetManagerPtr->getAsset<TerrainAsset>(U"砂漠");
		for (int i = 0; i < 5; ++i)
		{
			for (const auto& r1 : m_regions)
				if (r1->m_terrainAsset == t2)
					for (const auto& r2 : r1->m_connecteds)
						if (RandomBool(0.1) && r2.lock()->m_terrainAsset == t1) r2.lock()->m_terrainAsset = nullptr;

			for (const auto& r1 : m_regions)
				if (!r1->m_terrainAsset) r1->m_terrainAsset = t2;
		}
	}
}

void PlanetManager::saveRegions(const FilePath& path)
{
	Array<Vec3> positions;

	for (const auto& r : m_regions)
		positions.emplace_back(r->getPosition() / m_radius);

	TextWriter tw(path);
	tw.write(positions);

	tw.close();
}

void PlanetManager::loadRegions(const FilePath& path)
{
	// 読み込み
	JSONReader json(path);
	for (const auto& j : json.arrayView())
	{
		auto& r = m_regions.emplace_back(new Region);
		r->m_position.x = j.getArray<double>()[0];
		r->m_position.y = j.getArray<double>()[1];
		r->m_position.z = j.getArray<double>()[2];

		r->m_position *= m_radius;
	}

	connectRegions();
	makeChips();

	// Roadの設定
	for (const auto& r1 : m_regions)
		for (const auto& r2 : r1->m_connecteds)
			r1->m_roads.emplace_back(MakeShared<Road>(r1, r2.lock()));

	setTerrains();
}

void PlanetManager::update()
{
	if (!EasyViewer::GetRootViewer()
		->getChildViewer<PlanetViewer>()
		->hasChildViewer<ClearViewer>())
	{
		if (m_health > 0)
			m_health = Min(1.0, m_health + 0.0015);

		if (m_destroy >= 0.0)
		{
			if (m_destroy < 1.0) m_destroy += 0.01;
		}
		else
		{
			for (const auto& fs : m_facilityStates)
				fs->update();
		}
	}
}

void PlanetManager::destroy()
{
	g_soundManagerPtr->playSoundEffect(U"asset/models/facilities/sound/magic-quake2.mp3", SoundType::Environment);
	m_destroy = 0.0;

		EasyViewer::GetRootViewer()
			->getChildViewer<PlanetViewer>()
			->addChildViewer<GameOverViewer>();
}

void PlanetManager::addDamage(double value)
{
	if (m_health > 0)
	{
		m_health -= value;

		if (m_health < 0)
			g_planetManagerPtr->destroy();
	}
}

void PlanetManager::updateMouseOver(const BasicCamera3D& camera)
{
	auto mat = camera.getMat4x4();

	// 見えるものを近い順に整理
	const auto rs = m_regions
		.removed_if([&camera, this](const auto& r) { return !canSee(camera, r->m_position); })
		.sorted_by([&camera](const auto& r1, const auto& r2) { return camera.getEyePosition().distanceFromSq(r1->getPosition()) > camera.getEyePosition().distanceFromSq(r2->getPosition()); });

	// MouseOver
	m_mouseOverRegion = nullptr;
	for (const auto& r : rs)
		if (r->mouseOver(mat)) { m_mouseOverRegion = r; break; }
	if (m_mouseOverRegion) m_mouseOverRegion->draw(mat, Sqrt(m_mouseOverRegion->getArea(mat)) / 5.0, ColorF(1.0, 0.1));

	// Selected
	if (MouseL.up()) m_selectedRegion = nullptr;
	if (MouseL.down()) m_selectedRegion = m_mouseOverRegion;
}

void PlanetManager::drawRegions(const BasicCamera3D& camera)
{
	auto mat = camera.getMat4x4();

	// 見えるものを近い順に整理
	const auto rs = m_regions
		.removed_if([&camera, this](const auto& r) { return !canSee(camera, r->m_position); })
		.sorted_by([&camera](const auto& r1, const auto& r2) { return camera.getEyePosition().distanceFromSq(r1->getPosition()) > camera.getEyePosition().distanceFromSq(r2->getPosition()); });

	// draw
	for (const auto& r : rs)
		r->draw(mat);
}

void PlanetManager::drawChips(const BasicCamera3D& camera)
{
	auto mat = camera.getMat4x4();

	for (const auto& c : m_chips)
	{
		if (canSee(camera, c->m_center))
			c->draw(mat, HSV(double(&*c - &*m_chips.front())));
	}
}

void PlanetManager::drawRoads(const BasicCamera3D& camera)
{
	for (const auto& r : m_regions)
		for (const auto& road : r->m_roads)
			if (road->getRoadAsset() && canSee(camera, (road->getTo()->m_position + road->getFr()->m_position) / 2.0))
				road->draw(camera);

	for (const auto& ts : m_truckStates)
		ts->update();

	m_truckStates.remove_if([](const auto& ts) { return ts->m_route.isEmpty(); });

	for (const auto& ts : m_truckStates)
		if (canSee(camera, ts->getPosition())) ts->draw(camera);
}

void PlanetManager::drawFacilities(const BasicCamera3D& camera)
{
	auto fss = m_facilityStates.sorted_by([&camera](const auto& fs1, const auto& fs2) { return camera.getEyePosition().distanceFromSq(fs1->getPosition()) > camera.getEyePosition().distanceFromSq(fs2->getPosition()); });

	for (const auto& fs : fss)
		if (canSee(camera, fs->m_region.lock()->m_position)) fs->draw(camera);
}

bool PlanetManager::canSee(const BasicCamera3D& camera, const Vec3& position) const
{
	return camera.getEyePosition().distanceFrom(position) < Sqrt(camera.getEyePosition().lengthSq() - Square(m_radius));
}
