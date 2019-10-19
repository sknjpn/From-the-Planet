#pragma once

class Region;

class Road
{
public:
	weak_ptr<Region> m_fr;
	weak_ptr<Region> m_to;

	Road() {}
	Road(const shared_ptr<Region> fr, const shared_ptr<Region> to)
		: m_fr(fr)
		, m_to(to)
	{}

	void	draw(const BasicCamera3D& camera) const;
};

