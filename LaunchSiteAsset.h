#pragma once

#include "FacilityAsset.h"

class LaunchSiteAsset :
	public FacilityAsset
{
public:
	shared_ptr<FacilityState>	makeState() override;

	void initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;
	void updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;
};

