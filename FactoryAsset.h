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

	// JSON
	void	load(const JSONValue& json) override;
};

