#pragma once

class PlanetManager;
class FacilityAsset;
class FacilityState;
class Road;

class Region
	: public enable_shared_from_this<Region>
{
public:
	Vec3		m_position;
	Array<Vec3>	m_polygon;
	Color		m_color;
	double		m_height;
	weak_ptr<PlanetManager>	m_planet;
	Array<weak_ptr<Region>>		m_connecteds;
	Array<weak_ptr<Road>>		m_roads;
	shared_ptr<FacilityState>	m_facilityState;

	// returns isMouseover
	bool	draw(const Mat4x4& mat);
	void	setFacilityState(const shared_ptr<FacilityState> facilityState) { m_facilityState = facilityState; }

	Vec3	getCenter() { return m_position + m_position * m_height * 0.1; }

	// get
	const Vec3& getPosition() const { return m_position; }
	double	getHeight() const { return m_height; }

	// connection
	void	connect(const shared_ptr<Region>& to);
	bool	hasConnection(const shared_ptr<Region>& region) const { return m_connecteds.any([&region](const auto& c) { return c.lock() == region; }) }

	// facility
	void	makeFacilityState(const shared_ptr<FacilityAsset>& facilityAsset);
};
