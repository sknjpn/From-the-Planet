#pragma once

class Region
{
public:
	Vec3		m_position;
	Array<Vec3>	m_polygon;
	Color		m_color;
	double		m_height;
	bool		m_isMouseover;
	Array<weak_ptr<Region>>	m_connecteds;

	void	draw(const Mat4x4& mat);
	void	connect(const shared_ptr<Region>& to);

	Vec3	getCenter() { return m_position + m_position * m_height * 0.1; }
};

