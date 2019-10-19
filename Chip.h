#pragma once

class Region;

class Chip
{
public:
	shared_ptr<Region>	m_r0;
	shared_ptr<Region>	m_r1;
	shared_ptr<Region>	m_r2;
	Vec3	m_circumcenter;
	Vec3	m_center;

	Chip(const shared_ptr<Region>& r0, const shared_ptr<Region>& r1, const shared_ptr<Region>& r2);

	void	draw(const Mat4x4& mat, const Color& color);
	Array<Vec3>	getPositions() const;
};
