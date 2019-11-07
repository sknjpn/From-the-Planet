﻿#pragma once

#include "Viewer.h"

class FacilityAsset;
class RoadAsset;

class FacilitiesListViewer :
	public Viewer
{
public:
	shared_ptr<FacilityAsset>	m_selectedFacilityAsset;
	shared_ptr<RoadAsset>		m_selectedRoadAsset;
	Stopwatch					m_mouseOverTimer;

	FacilitiesListViewer()
		: m_mouseOverTimer(false)
	{
		setViewerRect(Scene::Size().x - 170, 40, 130, 600);
	}
	
	void	update() override;
	void	init() override;
};
