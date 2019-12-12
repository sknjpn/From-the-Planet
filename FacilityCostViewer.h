#pragma once

#include "EasyViewer.h"

class FacilityAsset;

class FacilityCostViewer :
	public EasyViewer
{
	shared_ptr<FacilityAsset>	m_facilityAsset;

public:
	FacilityCostViewer(const shared_ptr<FacilityAsset>& facilityAsset)
		: m_facilityAsset(facilityAsset)
	{}

	void	init() override
	{
		setViewerSize(300, 48);
	}

	void	update() override;
};

