#pragma once

#include "Mesh.h"

class Region;

class FacilityState
{
public:
	std::weak_ptr<Region>	m_region;
	Array<Mesh>		m_meshes;

	FacilityState();

	void draw(const BasicCamera3D& camera);
};

