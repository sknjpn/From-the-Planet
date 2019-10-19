#pragma once

#include "Mesh.h"

class Region;

class Facility
{
public:
	std::weak_ptr<Region>	m_region;
	Array<Mesh>		m_meshes;

	Facility();

	void draw(const BasicCamera3D& camera);
};

