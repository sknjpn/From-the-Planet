#pragma once

#include "FacilityAsset.h"

class FactoryAsset :
	public FacilityAsset
{
public:
	Storage	m_import;
	shared_ptr<ItemAsset> m_export;

public:
	shared_ptr<FacilityState>	makeState() override;

	void initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;
	void updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;

	// JSON
	void	load(const JSONValue& json) override;
};

