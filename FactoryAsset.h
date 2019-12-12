#pragma once

#include "FacilityAsset.h"

class FactoryAsset :
	public FacilityAsset
{
public:
	FilePath	m_soundPath;
	Storage		m_import;
	shared_ptr<ItemAsset> m_export;

public:
	shared_ptr<FacilityState>	makeState() override;

	void initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;
	void updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;

	const FilePath& getSoundPath() const { return m_soundPath; }

	// JSON
	void	load(const JSONValue& json) override;
};

