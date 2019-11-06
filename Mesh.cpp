#include "Mesh.h"

void Mesh::drawFill(const Mat4x4& mat) const
{
	Polygon(TranslateWorldToScreen(mat, m_vertices)).draw(m_color);
}

void Mesh::draw(const Mat4x4& mat) const
{
	for (const auto& l : m_lines)
	{
		const auto p0 = TranslateWorldToScreen(mat, m_vertices[l.first]);
		const auto p1 = TranslateWorldToScreen(mat, m_vertices[l.second]);

		Line(p0, p1).draw(m_thickness, m_color);
	}
}

void Mesh::load(const ptree& pt)
{
	Model::load(pt);

	// color
	m_color = Parse<Color>(Unicode::Widen(pt.get<string>("color")));

	// thickness
	m_thickness = pt.get<double>("thickness");

	// vertices
	for (auto m : pt.get_child("vertices"))
		m_vertices.emplace_back(Parse<Vec3>(Unicode::Widen(m.second.get_value<string>())));

	// lines
	for (auto m : pt.get_child("lines"))
	{
		auto p = Parse<Vec2>(Unicode::Widen(m.second.get_value<string>()));

		m_lines.emplace_back(p.x, p.y);
	}
}
