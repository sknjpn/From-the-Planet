#pragma once

class PlanetManager;
class FacilityState;

class Region
{
public:
	Vec3		m_position;
	Array<Vec3>	m_polygon;
	Color		m_color;
	double		m_height;
	weak_ptr<PlanetManager>	m_planet;
	Array<weak_ptr<Region>>		m_connecteds;
	shared_ptr<FacilityState>	m_facilityState;

	// returns isMouseover
	bool	draw(const Mat4x4& mat);
	void	connect(const shared_ptr<Region>& to);
	void	setFacilityState(const shared_ptr<FacilityState> facilityState) { m_facilityState = facilityState; }

	Vec3	getCenter() { return m_position + m_position * m_height * 0.1; }
};
