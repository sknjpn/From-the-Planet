#pragma once

class Region;

class Road
{
public:
	std::weak_ptr<Region> m_fr;
	std::weak_ptr<Region> m_to;

	Road() {}
	Road(const std::shared_ptr<Region> fr, const std::shared_ptr<Region> to)
		: m_fr(fr)
		, m_to(to)
	{}

	void	draw(const BasicCamera3D& camera) const;
};

