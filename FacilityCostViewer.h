#pragma once

#include "Viewer.h"

class FacilityAsset;

class FacilityCostViewer :
	public Viewer
{
	shared_ptr<FacilityAsset>	m_facilityAsset;

public:
	FacilityCostViewer()
	{
		setViewerSize(300, 100);
	}

	void	update() override;
};

