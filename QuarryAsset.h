#pragma once

#include "FacilityAsset.h"

class QuarryAsset :
	public FacilityAsset
{
	shared_ptr<FacilityState>	makeState() override;
};
