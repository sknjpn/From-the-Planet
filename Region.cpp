#include "Region.h"
#include "PlanetManager.h"
#include "FacilityAsset.h"
#include "Road.h"

bool Region::draw(const Mat4x4& mat)
{
	auto d = 4.0;
	Vec3 h = getCenter() - m_position;
	double area = 0.0;

	Vec2 center = TranslateWorldToScreen(mat, m_position + h);

	LineString ls;

	bool mouseover = false;
	for (int i = 0; i < m_polygon.size(); ++i)
	{
		Vec2 p0 = center;
		Vec2 p1 = TranslateWorldToScreen(mat, m_polygon[i] + h);
		Vec2 p2 = i == m_polygon.size() - 1 ? TranslateWorldToScreen(mat, m_polygon[0] + h) : TranslateWorldToScreen(mat, m_polygon[i + 1] + h);
		Vec2 a1 = p1 + (p0 - p1).normalized() * Min(d, (p0 - p1).length());
		Vec2 a2 = p2 + (p0 - p2).normalized() * Min(d, (p0 - p2).length());

		auto triangle = Triangle(p0, a1, a2);
		triangle.draw(g_planetManagerPtr->m_mouseOverRegion == shared_from_this() ? ColorF(Palette::Red) : ColorF(m_color, 0.5));
		if (triangle.mouseOver()) mouseover = true;
		area += triangle.area();

		ls.emplace_back(a1);
	}

	auto t = Sqrt(area) / 10.0;
	for (auto& p : ls)
		p += (center - p).normalized() * Min(t / 2.0, (center - p).length());

	ls.drawCatmullRomClosed(t, m_color);

	//for (auto& c : m_connecteds) Line(center, (TranslateWorldToScreen(mat, c.lock()->m_position) + center) / 2.0).stretched(-2).drawArrow(2.0, Vec2(10.0, 10.0), Palette::White);

	return mouseover;
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
