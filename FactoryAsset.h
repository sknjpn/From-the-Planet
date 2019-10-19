#pragma once

#include "FacilityAsset.h"

class FactoryAsset :
	public FacilityAsset
{
public:
	shared_ptr<FacilityState>	makeState() override;
};

