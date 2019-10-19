#pragma once

#include "Asset.h"

class FacilityState;

class FacilityAsset :
	public Asset
{
	virtual shared_ptr<FacilityState>	makeState();
};

