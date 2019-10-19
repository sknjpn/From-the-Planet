#include "Chip.h"
#include "Region.h"


Chip::Chip(const shared_ptr<Region>& r0, const shared_ptr<Region>& r1, const shared_ptr<Region>& r2)
	: m_r0(r0)
	, m_r1(r1)
	, m_r2(r2)
{
	m_circumcenter = GetCircumcenter(m_r0->m_position, m_r1->m_position, m_r2->m_position);
	m_center = (m_r0->m_position + m_r1->m_position + m_r2->m_position) / 3.0;
}

void Chip::draw(const Mat4x4& mat, const Color& color)
{
	Array<Vec2> positions = TranslateWorldToScreen(mat, getPositions());

	auto g = (positions[0] + positions[1] + positions[2]) / 3.0;

	for (int j = 0; j < 3; ++j)
		positions[j] += (g - positions[j]).normalized() * Min(4.0, (g - positions[j]).length());

	Triangle(positions[0], positions[1], positions[2]).draw(color);
}

Array<Vec3> Chip::getPositions() const
{
	return { m_r0->m_position, m_r1->m_position, m_r2->m_position };
}
