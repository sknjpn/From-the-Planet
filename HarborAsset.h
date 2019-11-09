#pragma once

#include "FacilityAsset.h"

class HarborAsset :
	public FacilityAsset
{
public:
	shared_ptr<FacilityState>	makeState() override;
	bool canConstructAt(const shared_ptr<Region>& region) const override;

	void initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;
	void updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const override;
};

