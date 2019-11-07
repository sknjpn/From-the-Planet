#include "Mesh.h"
#include "Translate.h"

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

void Mesh::drawBlack(const Mat4x4& mat) const
{
	for (const auto& l : m_lines)
	{
		const auto p0 = TranslateWorldToScreen(mat, m_vertices[l.first]);
		const auto p1 = TranslateWorldToScreen(mat, m_vertices[l.second]);

		Line(p0, p1).draw(m_thickness * 3.0, Palette::Black);
	}
}

void Mesh::draw(const Mat4x4& mat, const Color& color) const
{
	for (const auto& l : m_lines)
	{
		const auto p0 = TranslateWorldToScreen(mat, m_vertices[l.first]);
		const auto p1 = TranslateWorldToScreen(mat, m_vertices[l.second]);

		Line(p0, p1).draw(m_thickness, color);
	}
}

void Mesh::load(const JSONValue& json)
{
	Model::load(json);

	// color
	m_color = json[U"color"].get<Color>();

	// thickness
	m_thickness = json[U"thickness"].get<double>();

	// vertices
	for (auto m : json[U"vertices"].arrayView())
		m_vertices.emplace_back(m.get<Vec3>());

	// lines
	for (auto m : json[U"lines"].arrayView())
	{
		auto p = m.get<Point>();

		m_lines.emplace_back(p.x, p.y);
	}
}
