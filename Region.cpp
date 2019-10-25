#include "Region.h"
#include "PlanetManager.h"
#include "FacilityAsset.h"
#include "Road.h"

bool Region::draw(const Mat4x4& mat)
{
	m_color = m_height > 0.0 ? Palette::Green : Palette::Royalblue;
	if (m_height > 0.3) m_color = Palette::Gray;
	if (m_height > 0.5) m_color = Palette::White;

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

	auto t =Sqrt( area) / 10.0;
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

}

void Region::makeFacilityState(const shared_ptr<FacilityAsset>& facilityAsset)
{
	m_facilityState = facilityAsset->makeState();
}

void Region::makeRoad(const shared_ptr<Region>& to)
{
}
