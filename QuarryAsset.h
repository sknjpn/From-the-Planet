#pragma once

#include "FacilityAsset.h"

class QuarryAsset :
	public FacilityAsset
{
public:
	shared_ptr<FacilityState>	makeState() override;
};
