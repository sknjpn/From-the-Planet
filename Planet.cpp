#include "Planet.h"
#include "Region.h"
#include "Chip.h"
#include "Road.h"

void Planet::generateRegions(size_t n)
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

	for (const auto& p : positions)
	{
		auto& r = m_regions.emplace_back(MakeShared<Region>());

		r->m_position = p * m_radius;
	}
}

void Planet::connectRegions()
{
	double f_max = 0;
	for (const auto& r1 : m_regions)
	{
		double min = 2.0 * m_radius;
		for (const auto& r2 : m_regions)
			if (r1 != r2) min = Min(min, (r1->m_position - r2->m_position).length());

		f_max = Max(f_max, min);
	}

	for (auto& r1 : m_regions)
	{
		for (auto& r2 : m_regions)
		{
			if (r1 == r2) continue;

			if (r1->m_position.distanceFrom(r2->m_position) < f_max * 2)
				r1->m_connecteds.emplace_back(r2);
		}
	}
}

void Planet::makeChips()
{
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
						// 外心に最も近い点が頂点でないことの証明
						{
							const Array<Vec3> positions = { r1->m_position, r2.lock()->m_position, r3.lock()->m_position };
							const Vec3 c = GetCircumcenter(positions);
							double min = positions.front().distanceFrom(c);
							bool flag = false;
							for (auto& ro : m_regions)
								if (ro->m_position.distanceFrom(c) < min && ro != r2.lock() && ro != r3.lock()) { flag = true; break; }

							if (flag) continue;
						}

						// 生成
						auto& t = m_chips.emplace_back(MakeShared<Chip>(r1, r2.lock(), r3.lock()));
						r1->m_polygon.emplace_back(t->m_center);
						r2.lock()->m_polygon.emplace_back(t->m_center);
						r3.lock()->m_polygon.emplace_back(t->m_center);
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
				if ((p1 - r->m_position).cross(r->m_polygon[j] - r->m_position).dot(r->m_position) > 0 &&
					(best == i || (r->m_polygon[best] - r->m_position).dot(p1 - r->m_position) < (r->m_polygon[j] - r->m_position).dot(p1 - r->m_position)))
					best = j;

			auto pb = r->m_polygon[best];
			r->m_polygon[best] = r->m_polygon[i + 1];
			r->m_polygon[i + 1] = pb;
		}
	}
}

void Planet::generateTerrain()
{
	PerlinNoise noise(Random(INT_MAX));

	for (auto& r : m_regions)
	{
		r->m_height = Max(Abs(noise.octaveNoise(r->m_position * 0.01, 5)) - 0.2, 0.0);

		r->m_color = r->m_height > 0.0 ? Palette::Green : Palette::Royalblue;
		if (r->m_height > 0.3) r->m_color = Palette::Gray;
		if (r->m_height > 0.5) r->m_color = Palette::White;
	}
}

void Planet::saveRegions(const FilePath& path)
{
	Array<Vec3> positions;

	for (const auto& r : m_regions)
		positions.emplace_back(r->m_position /= m_radius);

	TextWriter tw(path);
	tw.write(positions);

	tw.close();
}

void Planet::loadRegions(const FilePath& path)
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

void Planet::drawRegions(const BasicCamera3D& camera)
{
	auto mat = camera.getMat4x4();

	std::shared_ptr<Region> preMouseover;
	for (auto& r : m_regions) if (r->m_isMouseover) { preMouseover = r; break; }
	for (const auto& r : m_regions)
	{
		if (camera.getEyePosition().distanceFrom(r->m_position) > Sqrt(camera.getEyePosition().lengthSq() - Square(m_radius))) continue;

		r->draw(mat);
	}
	std::shared_ptr<Region> nowMouseover;
	for (auto& r : m_regions) if (r->m_isMouseover) { preMouseover = r; break; }
}

void Planet::drawChips(const BasicCamera3D& camera)
{
	auto mat = camera.getMat4x4();

	for (const auto& c : m_chips)
	{
		if (camera.getEyePosition().distanceFrom(c->m_center) > Sqrt(camera.getEyePosition().lengthSq() - Square(m_radius))) continue;

		c->draw(mat, HSV(&*c - &*m_chips.front()));
	}
}

void Planet::drawRoads(const BasicCamera3D& camera)
{
	for (const auto& r : m_roads) r->draw(camera);
}
