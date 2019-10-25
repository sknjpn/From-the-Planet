﻿#pragma once

class Region;
class Chip;
class Road;
class FacilityState;
class FacilityAsset;

class PlanetManager
{
public:
	Array<shared_ptr<Region>>	m_regions;
	Array<shared_ptr<Chip>>		m_chips;
	Array<shared_ptr<Road>>		m_roads;
	Array<shared_ptr<FacilityState>>	m_facilityStates;
	double	m_radius = 100.0;

	shared_ptr<Region>	m_mouseOverRegion;
	shared_ptr<Region>	m_selectedRegion;

public:
	PlanetManager()
		: m_radius(100.0)
	{}

	const shared_ptr<FacilityState>& makeFacility(const shared_ptr<FacilityAsset> facilityAsset, const shared_ptr<Region> region);

	void	generateRegions(size_t n);
	void	connectRegions();
	void	makeChips();

	void	generateTerrain();

	void	saveRegions(const FilePath& path);
	void	loadRegions(const FilePath& path);

	void	drawRegions(const BasicCamera3D& camera);
	void	drawChips(const BasicCamera3D& camera);
	void	drawRoads(const BasicCamera3D& camera);
	void	drawFacilities(const BasicCamera3D& camera);
	
	bool	canSee(const BasicCamera3D& camera, const Vec3& position) const;
};

extern unique_ptr<PlanetManager> g_planetManagerPtr;