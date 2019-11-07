#pragma once

#include "FacilityAsset.h"

class HarborAsset :
	public FacilityAsset
{
public:
	shared_ptr<FacilityState>	makeState() override;
	bool canConstructAt(const shared_ptr<Region>& region) const override;

	String getBuildText() const override;
};

