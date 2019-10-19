#pragma once

class Region;

class Chip
{
public:
	std::shared_ptr<Region>	m_r0;
	std::shared_ptr<Region>	m_r1;
	std::shared_ptr<Region>	m_r2;
	Vec3	m_circumcenter;
	Vec3	m_center;

	Chip(const std::shared_ptr<Region>& r0, const std::shared_ptr<Region>& r1, const std::shared_ptr<Region>& r2);

	void	draw(const Mat4x4& mat, const Color& color);
	Array<Vec3>	getPositions() const;
};
