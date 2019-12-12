#pragma once

#include "Popup.h"

class FacilityAsset;

class FacilityConstructionPopup :
	public Popup
{
	shared_ptr<FacilityAsset> m_facilityAsset;

public:
	FacilityConstructionPopup(const shared_ptr<FacilityAsset>& facilityAsset)
		: m_facilityAsset(facilityAsset)
	{}

	void	update() override;
	void	init() override;
};

