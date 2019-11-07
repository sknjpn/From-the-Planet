#pragma once

#include "FacilityState.h"

class LaunchSiteState
	: public FacilityState
{
public:
	void	onConstructed() override;
};

