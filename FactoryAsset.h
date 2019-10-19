#pragma once

#include "FacilityAsset.h"

class FactoryAsset :
	public FacilityAsset
{
	shared_ptr<FacilityState>	makeState() override;
};

