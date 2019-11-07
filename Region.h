#pragma once

class PlanetManager;
class FacilityAsset;
class FacilityState;
class Road;

class Region
	: public enable_shared_from_this<Region>
{
	friend class PlanetManager;

	Vec3		m_position;
	Array<Vec3>	m_polygon;
	Color		m_color;
	weak_ptr<PlanetManager>	m_planet;
	Array<weak_ptr<Region>>		m_connecteds;
	Array<shared_ptr<Road>>		m_roads;
	shared_ptr<FacilityState>	m_facilityState;

	// for 探索
	shared_ptr<Region>	m_from;
	double	m_cost = 0.0;

public:
	// returns isMouseover
	bool	draw(const Mat4x4& mat);
	void	setFacilityState(const shared_ptr<FacilityState> facilityState) { m_facilityState = facilityState; }

	// get
	const Vec3& getPosition() const { return m_position; }

	// connection
	void	connect(const shared_ptr<Region>& to);
	void	disconnect(const shared_ptr<Region>& to);
	bool	hasConnection(const shared_ptr<Region>& region) const { return m_connecteds.any([&region](const auto& c) { return c.lock() == region; }); }

	// facility
	void	makeFacilityState(const shared_ptr<FacilityAsset>& facilityAsset);
	const shared_ptr<FacilityState>& getFacilityState() const { return m_facilityState; }

	Array<shared_ptr<Road>>	getRouteTo(const shared_ptr<Region> to) const;

	// road
	void	makeRoad(const shared_ptr<Region>& to);
	bool	hasRoad(const shared_ptr<Region>& to) const;
	shared_ptr<Road>	getRoad(const shared_ptr<Region>& to) const;
};
