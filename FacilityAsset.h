#pragma once

#include "Asset.h"
#include "Mesh.h"
#include "Storage.h"

class FacilityState;
class Region;

class FacilityAsset
	: public Asset
{
	friend class FacilityState;

	Array<Mesh>	m_meshes;
	Storage		m_material;
	FilePath	m_audio;
	double		m_constructinDamage;

public:
	virtual shared_ptr<FacilityState>	makeState() = 0;
	virtual bool canConstructAt(const shared_ptr<Region>& region) const { return true; }

	const Array<Mesh>& getMeshes() const { return m_meshes; }
	const FilePath& getAudioPath() const { return m_audio; }

	virtual String getBuildText() const;

	// JSON
	void	load(const JSONValue& json) override;
};

