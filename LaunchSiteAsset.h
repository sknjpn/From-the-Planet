#pragma once

#include "FacilityAsset.h"

class LaunchSiteAsset :
	public FacilityAsset
{
public:
	shared_ptr<FacilityState>	makeState() override;
};

