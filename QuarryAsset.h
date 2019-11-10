#pragma once

#include "FacilityAsset.h"

class NaturalResourceAsset;

class QuarryAsset :
	public FacilityAsset
{
public:
	FilePath	m_soundPath;
	shared_ptr<NaturalResourceAsset> m_export;

public:
	shared_ptr<FacilityState>	makeState() override;
	bool canConstructAt(const shared_ptr<Region>& region) const override;

	void initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;
	void updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;

	const FilePath& getSoundPath() const { return m_soundPath; }

	// JSON
	void	load(const JSONValue& json) override;
};
