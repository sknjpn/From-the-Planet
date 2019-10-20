#pragma once

#include "Mesh.h"

class Region;
class FacilityAsset;

class FacilityState
{
public:
	weak_ptr<Region>	m_region;
	shared_ptr<FacilityAsset>	m_facilityAsset;

	void draw(const BasicCamera3D& camera);
};

