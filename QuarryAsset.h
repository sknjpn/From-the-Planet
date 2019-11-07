#pragma once

#include "FacilityAsset.h"

class QuarryAsset :
	public FacilityAsset
{
public:
	shared_ptr<ItemAsset> m_export;

public:
	shared_ptr<FacilityState>	makeState() override;

	String getBuildText() const override;

	// JSON
	void	load(const JSONValue& json) override;
};
