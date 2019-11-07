#include "Region.h"
#include "PlanetManager.h"
#include "FacilityAsset.h"
#include "TerrainAsset.h"
#include "Road.h"

void Region::draw(const Mat4x4& mat, double d, Color color) const
{
	const Vec2 p0 = TranslateWorldToScreen(mat, m_position);

	for (int i = 0; i < m_polygon.size(); ++i)
	{
		const Vec2 p1 = TranslateWorldToScreen(mat, m_polygon[i]);
		const Vec2 p2 = i == m_polygon.size() - 1 ? TranslateWorldToScreen(mat, m_polygon[0]) : TranslateWorldToScreen(mat, m_polygon[i + 1]);
		const Vec2 a1 = p1 + (p0 - p1).normalized() * Min(d, (p0 - p1).length());
		const Vec2 a2 = p2 + (p0 - p2).normalized() * Min(d, (p0 - p2).length());

		Triangle(p0, a1, a2).draw(color);
	}
}

void Region::drawLineString(const Mat4x4& mat, double d, Color color) const
{
	const Vec2 p0 = TranslateWorldToScreen(mat, m_position);

	LineString ls;

	for (int i = 0; i < m_polygon.size(); ++i)
	{
		const Vec2 p1 = TranslateWorldToScreen(mat, m_polygon[i]);
		const Vec2 a1 = p1 + (p0 - p1).normalized() * Min(d, (p0 - p1).length());

		ls.emplace_back(a1);
	}

	auto t = Sqrt(getArea(mat)) / 10.0;
	for (auto& p : ls)
		p += (p0 - p).normalized() * Min(t / 2.0, (p0 - p).length());

	ls.drawCatmullRomClosed(t, color);
}

bool Region::mouseOver(const Mat4x4& mat) const
{
	const Vec2 p0 = TranslateWorldToScreen(mat, m_position);

	for (int i = 0; i < m_polygon.size(); ++i)
	{
		const Vec2 p1 = TranslateWorldToScreen(mat, m_polygon[i]);
		const Vec2 p2 = i == m_polygon.size() - 1 ? TranslateWorldToScreen(mat, m_polygon[0]) : TranslateWorldToScreen(mat, m_polygon[i + 1]);

		if (Triangle(p0, p1, p2).mouseOver()) return true;
	}

	return false;
}

void Region::draw(const Mat4x4& mat) const
{
	auto t = Sqrt(getArea(mat));
	auto color = m_terrainAsset->m_color;

	if(g_planetManagerPtr->m_destroy >= 0)
		color = color.lerp(Palette::Red, g_planetManagerPtr->m_destroy);

	draw(mat, t / 10.0, color);
	draw(mat, t / 5.0, (g_planetManagerPtr->m_mouseOverRegion == shared_from_this() ? ColorF(Palette::Red) : ColorF(color)).lerp(Palette::Black, 0.4));
}

double Region::getArea(const Mat4x4& mat) const
{
	const Vec2 p0 = TranslateWorldToScreen(mat, m_position);

	double area = 0.0;

	for (int i = 0; i < m_polygon.size(); ++i)
	{
		const Vec2 p1 = TranslateWorldToScreen(mat, m_polygon[i]);
		const Vec2 p2 = i == m_polygon.size() - 1 ? TranslateWorldToScreen(mat, m_polygon[0]) : TranslateWorldToScreen(mat, m_polygon[i + 1]);
		area += Triangle(p0, p1, p2).area();
	}

	return area;
}

void Region::connect(const shared_ptr<Region>& to)
{
	// 相互接続
	if (!hasConnection(to)) m_connecteds.emplace_back(to);
	if (!to->hasConnection(shared_from_this())) to->m_connecteds.emplace_back(shared_from_this());
}

void Region::disconnect(const shared_ptr<Region>& to)
{
	// 相互切断
	m_connecteds.remove_if([&to](const auto& c) { return c.lock() == to; });
	to->m_connecteds.remove_if([this](const auto& c) { return c.lock() == shared_from_this(); });
}

void Region::makeFacilityState(const shared_ptr<FacilityAsset>& facilityAsset)
{
	m_facilityState = facilityAsset->makeState();
}

Array<shared_ptr<Road>> Region::getRouteTo(const shared_ptr<Region> to) const
{
	if (shared_from_this() == to) return Array<shared_ptr<Road>>();

	Array<shared_ptr<Region>> list;
	for (const auto& r : to->m_roads)
	{
		if (r->getTo() != to)
		{
			list.emplace_back(r->getTo());
			r->getTo()->m_cost = r->getLength();
			r->getTo()->m_from = to;
		}
	}

	for (int i = 0; i < list.size(); ++i)
	{
		auto l = list[i];
		for (const auto& r : l->m_roads)
		{
			auto t = r->getTo();
			if (t != l && t != to)
			{
				if (!t->m_from || t->m_cost > l->m_cost + r->getLength())
				{
					list.emplace_back(t);
					t->m_from = l;
					t->m_cost = l->m_cost + r->getLength();
				}
			}
		}
	}

	if (m_from)
	{
		Array<shared_ptr<Road>> result;
		result.emplace_back(getRoad(m_from));
		for (;;)
		{
			if (!result.back()->getTo()->m_from) break;

			result.emplace_back(result.back()->getTo()->getRoad(result.back()->getTo()->m_from));
		}

		for (int i = 0; i < list.size(); ++i)
		{
			auto l = list[i];

			l->m_cost = 0;
			l->m_from = nullptr;
		}

		return result;
	}
	else
	{
		for (int i = 0; i < list.size(); ++i)
		{
			auto l = list[i];

			l->m_cost = 0;
			l->m_from = nullptr;
		}

		return Array<shared_ptr<Road>>();
	}
}

void Region::makeRoad(const shared_ptr<Region>& to)
{
	auto road = MakeShared<Road>(shared_from_this(), to);
	g_planetManagerPtr->m_roads.emplace_back(road);
	m_roads.emplace_back(road);
	to->m_roads.emplace_back(road);
}

bool Region::hasRoad(const shared_ptr<Region>& to) const
{
	return m_roads.any([&to](const auto& r) { return to == r->getTo(); });
}

shared_ptr<Road> Region::getRoad(const shared_ptr<Region>& to) const
{
	for (const auto& r : m_roads)
		if (r->getTo() == to) return r;

	return nullptr;
}
