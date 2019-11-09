#pragma once

#include "Asset.h"
#include "Mesh.h"
#include "Storage.h"

class FacilityState;
class Region;
class FacilityDescPopup;

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

	const Storage& getMaterial() const { return m_material; }
	const Array<Mesh>& getMeshes() const { return m_meshes; }
	const FilePath& getAudioPath() const { return m_audio; }
	double getConstructinDamage() const { return m_constructinDamage; }

	virtual void onDrawPopup() const;
	virtual String getBuildText() const;
	
	virtual void initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const = 0;
	virtual void updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const = 0;

	const Color& getColor() const { return m_meshes.front().m_color; }

	// JSON
	void	load(const JSONValue& json) override;
};

