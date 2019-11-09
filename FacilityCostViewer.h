#pragma once

#include "Viewer.h"

class FacilityAsset;

class FacilityCostViewer :
	public Viewer
{
	shared_ptr<FacilityAsset>	m_facilityAsset;

public:
	FacilityCostViewer(const shared_ptr<FacilityAsset>& facilityAsset)
		: m_facilityAsset(facilityAsset)
	{
		setViewerSize(300, 48);
	}

	void	update() override;
};

