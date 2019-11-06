﻿#pragma once

#include "Model.h"

class Mesh
	: public Model
{
public:
	Array<Vec3>	m_vertices;
	Array<pair<size_t, size_t>>	m_lines;
	Color		m_color;
	double		m_thickness;

	Mesh() {}
	Mesh(const Array<Vec3>& vertices, const Color& color, double thickness = 1.0)
		: m_vertices(vertices)
		, m_color(color)
		, m_thickness(thickness)
	{}

	void	drawFill(const Mat4x4& mat) const;
	void	draw(const Mat4x4& mat) const;

	void	moveBy(const Vec3& delta) { m_vertices.each([&delta](auto& v) { v.moveBy(delta); }); }
	void	moveBy(double x, double y, double z) { moveBy(Vec3(x, y, z)); }

	// JSON
	void	load(const ptree& pt) override;
};

