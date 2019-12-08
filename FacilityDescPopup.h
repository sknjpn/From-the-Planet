#pragma once

#include "Popup.h"

class FacilityAsset;

class FacilityDescPopup :
	public Popup
{
	shared_ptr<FacilityAsset>	m_facilityAsset;

public:
	FacilityDescPopup(const shared_ptr<FacilityAsset>& facilityAsset)
		: m_facilityAsset(facilityAsset)
	{}

	void	init();
	void	update();
};

