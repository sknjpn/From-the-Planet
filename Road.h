#pragma once

class Region;

class Road
{
	shared_ptr<Region> m_fr;
	shared_ptr<Region> m_to;

public:
	Road() {}
	Road(const shared_ptr<Region> fr, const shared_ptr<Region> to)
		: m_fr(fr)
		, m_to(to)
	{}

	void	draw(const BasicCamera3D& camera) const;

	// get
	const shared_ptr<Region>& getFr() const { return m_fr; }
	const shared_ptr<Region>& getTo() const { return m_to; }
};

