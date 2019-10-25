#include "PlanetManager.h"
#include "Region.h"
#include "Chip.h"
#include "Road.h"
#include "FacilityAsset.h"
#include "FacilityState.h"

unique_ptr<PlanetManager> g_planetManagerPtr;

const shared_ptr<FacilityState>& PlanetManager::makeFacility(const shared_ptr<FacilityAsset> facilityAsset, const shared_ptr<Region> region)
{
	auto& state = m_facilityStates.emplace_back(facilityAsset->makeState());

	state->m_region = region;
	state->m_facilityAsset = facilityAsset;

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
							r1->m_polygon.emplace_back(t->m_center);
							r2.lock()->m_polygon.emplace_back(t->m_center);
							r3.lock()->m_polygon.emplace_back(t->m_center);
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

void PlanetManager::generateTerrain()
{
	PerlinNoise noise(Random(INT_MAX));

	for (auto& r : m_regions)
	{
		r->m_height = Max(Abs(noise.octaveNoise(r->getPosition() * 0.01, 5)) - 0.2, 0.0);

		r->m_color = r->m_height > 0.0 ? Palette::Green : Palette::Royalblue;
		if (r->m_height > 0.3) r->m_color = Palette::Gray;
		if (r->m_height > 0.5) r->m_color = Palette::White;
	}
}

void PlanetManager::saveRegions(const FilePath& path)
{
	Array<Vec3> positions;

	for (const auto& r : m_regions)
		positions.emplace_back(r->getPosition() /= m_radius);

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
	generateTerrain();
}

void PlanetManager::drawRegions(const BasicCamera3D& camera)
{
	auto mat = camera.getMat4x4();
	shared_ptr<Region> mouseoverRegion;

	if (MouseL.up()) m_selectedRegion = nullptr;

	for (const auto& r : m_regions)
	{
		if (canSee(camera, r->m_position))
			if (r->draw(mat)) mouseoverRegion = r;
	}

	m_mouseOverRegion = mouseoverRegion;
	if (MouseL.down()) m_selectedRegion = mouseoverRegion;
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
	for (const auto& r : m_roads)
		if (canSee(camera, (r->m_to.lock()->m_position + r->m_fr.lock()->m_position) / 2.0)) r->draw(camera);
}

void PlanetManager::drawFacilities(const BasicCamera3D& camera)
{
	for (const auto& fs : m_facilityStates)
		if (canSee(camera, fs->m_region.lock()->m_position)) fs->draw(camera);
}

bool PlanetManager::canSee(const BasicCamera3D& camera, const Vec3& position) const
{
	return camera.getEyePosition().distanceFrom(position) < Sqrt(camera.getEyePosition().lengthSq() - Square(m_radius));
}
