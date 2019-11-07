#pragma once

#include "Asset.h"
#include "Mesh.h"
#include "Storage.h"

class FacilityState;

class FacilityAsset
	: public Asset
{
	friend class FacilityState;

	Array<Mesh>	m_meshes;
	Storage		m_material;
	FilePath	m_audio;

public:
	virtual shared_ptr<FacilityState>	makeState() = 0;

	const Array<Mesh>& getMeshes() const { return m_meshes; }
	const FilePath& getAudioPath() const { return m_audio; }

	// JSON
	void	load(const JSONValue& json) override;
};

