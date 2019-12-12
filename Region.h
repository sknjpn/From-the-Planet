#pragma once

class PlanetManager;
class FacilityAsset;
class FacilityState;
class TerrainAsset;
class NaturalResourceAsset;
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
	shared_ptr<NaturalResourceAsset>	m_naturalResourceAsset;

	// for 探索
	shared_ptr<Region>	m_from;
	double	m_cost = 0.0;

	void	drawLineString(const Mat4x4& mat, double d, Color color) const;

public:
	bool	mouseOver(const Mat4x4& mat) const;
	void	draw(const Mat4x4& mat) const;
	void	draw(const Mat4x4& mat, double d, Color color) const;
	void	setFacilityState(const shared_ptr<FacilityState> facilityState) { m_facilityState = facilityState; }
	double	getArea(const Mat4x4& mat) const;

	void	setNaturalResourceAsset(const shared_ptr<NaturalResourceAsset>& naturalResourceAsset) { m_naturalResourceAsset = naturalResourceAsset; }

	// Cost Map
	const shared_ptr<Region>& getFrom() const { return m_from; }
	double getCost() const { return m_cost; }

	// get
	const Vec3& getPosition() const { return m_position; }
	const shared_ptr<TerrainAsset>& getTerrainAsset() const { return m_terrainAsset; }
	const Array<weak_ptr<Region>>& getConnecteds() const { return m_connecteds; }
	const shared_ptr<NaturalResourceAsset>& getNaturalResourceAsset() const { return m_naturalResourceAsset; }

	// connection
	void	connect(const shared_ptr<Region>& to);
	void	disconnect(const shared_ptr<Region>& to);
	bool	hasConnection(const shared_ptr<Region>& region) const { return m_connecteds.any([&region](const auto& c) { return c.lock() == region; }); }

	// facility
	void	makeFacilityState(const shared_ptr<FacilityAsset>& facilityAsset);
	const shared_ptr<FacilityState>& getFacilityState() const { return m_facilityState; }
	bool	hasHarbor() const;

	// road
	shared_ptr<Road>	getRoad(const shared_ptr<Region>& to) const;
};
