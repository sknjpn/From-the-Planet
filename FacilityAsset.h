#pragma once

#include "Asset.h"
#include "Mesh.h"

class FacilityState;

class FacilityAsset :
	public Asset
{
	Array<Mesh>		m_meshes;

public:
	virtual shared_ptr<FacilityState>	makeState() = 0;

	const Array<Mesh>& getMeshes() const { return m_meshes; }

	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
};

