#include "Mesh.h"

void Mesh::drawFill(const Mat4x4& mat) const
{
	Polygon(TranslateWorldToScreen(mat, m_vertices)).draw(m_color);
}

void Mesh::draw(const Mat4x4& mat) const
{
	LineString(TranslateWorldToScreen(mat, m_vertices)).drawClosed(m_thickness, m_color);
}
