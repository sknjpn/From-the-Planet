#pragma once

class Region;
class RoadAsset;

class Road
{
	shared_ptr<Region> m_fr;
	shared_ptr<Region> m_to;
	double m_length;
	shared_ptr<RoadAsset>	m_roadAsset;

public:
	Road(const shared_ptr<Region> fr, const shared_ptr<Region> to);

	void	draw(const BasicCamera3D& camera) const;

	// set
	void	setRoadAsset(const shared_ptr<RoadAsset>& roadAsset) { m_roadAsset = roadAsset; }

	// get
	const shared_ptr<Region>& getFr() const { return m_fr; }
	const shared_ptr<Region>& getTo() const { return m_to; }
	const shared_ptr<RoadAsset>& getRoadAsset() const { return m_roadAsset; }
	double	getLength() const;

	shared_ptr<Road> getOppositeRoad() const;

	double	getCost() const;
};

