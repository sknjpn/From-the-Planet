#pragma once

#include "Asset.h"

class FacilityState;

class FacilityAsset :
	public Asset
{
public:
	virtual shared_ptr<FacilityState>	makeState();
};

