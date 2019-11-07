#pragma once

class PlanetManager;
class FacilityAsset;
class FacilityState;
class TerrainAsset;
class Road;

class Region
	: public enable_shared_from_this<Region>
{
	friend class PlanetManager;

	Vec3		m_position;
	Array<Vec3>	m_polygon;
	weak_ptr<PlanetManager>	m_planet;
	Array<weak_ptr<Region>>		m_connecteds;
	Array<shared_ptr<Road>>		m_roads;
	shared_ptr<TerrainAsset>	m_terrainAsset;
	shared_ptr<FacilityState>	m_facilityState;

	// for 探索
	shared_ptr<Region>	m_from;
	double	m_cost = 0.0;

	void	draw(const Mat4x4& mat, double d, Color color) const;
	void	drawLineString(const Mat4x4& mat, double d, Color color) const;

public:
	bool	mouseOver(const Mat4x4& mat) const;
	void	draw(const Mat4x4& mat) const;
	void	setFacilityState(const shared_ptr<FacilityState> facilityState) { m_facilityState = facilityState; }
	double	getArea(const Mat4x4& mat) const;

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
