#pragma once

class Region;
class Chip;
class Road;
class FacilityState;
class FacilityAsset;
class TruckState;

class PlanetManager
{
public:
	Array<shared_ptr<Region>>	m_regions;
	Array<shared_ptr<Chip>>		m_chips;
	Array<shared_ptr<FacilityState>>	m_facilityStates;
	Array<shared_ptr<TruckState>>		m_truckStates;
	double	m_radius = 100.0;
	double	m_destroy = -1;
	double	m_health = 1.0;
	Audio	m_audio;

	shared_ptr<Region>	m_mouseOverRegion;
	shared_ptr<Region>	m_selectedRegion;
	Stopwatch m_sw;

public:
	PlanetManager()
		: m_radius(100.0)
		, m_sw(true)
	{}

	void	reset()
	{
		m_regions.clear();
		m_chips.clear();
		m_facilityStates.clear();
		m_truckStates.clear();
		m_radius = 100.0;
		m_destroy = -1;
		m_health = 1.0;
		m_mouseOverRegion = nullptr;
		m_selectedRegion = nullptr;
	}

	const shared_ptr<FacilityState>& makeFacility(const shared_ptr<FacilityAsset> facilityAsset, const shared_ptr<Region> region);

	void	generateRegions(size_t n);
	void	connectRegions();
	void	makeChips();

	void	setTerrains();

	void	saveRegions(const FilePath& path);
	void	loadRegions(const FilePath& path);

	void	update();
	void	destroy();
	void	addDamage(double value);

	void	updateMouseOver(const BasicCamera3D& camera);

	void	drawRegions(const BasicCamera3D& camera);
	void	drawChips(const BasicCamera3D& camera);
	void	drawRoads(const BasicCamera3D& camera);
	void	drawFacilities(const BasicCamera3D& camera);

	// Using CostMap
	void	resetCostMap();
	void	bakeCostMap(const shared_ptr<Region>& from, function<bool(const shared_ptr<Road>&)> func);
	Array<shared_ptr<Road>>	getRoute(const shared_ptr<Region> from, const shared_ptr<Region> to) const;
	double	getCost(const shared_ptr<Region> from, const shared_ptr<Region> to) const;
	
	bool	canSee(const BasicCamera3D& camera, const Vec3& position) const;
};

extern unique_ptr<PlanetManager> g_planetManagerPtr;