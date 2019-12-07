#pragma once

#include "EasyViewer.h"

class FacilityAsset;
class RoadAsset;

class FacilitiesListViewer :
	public EasyViewer
{
	class SelectedArrowViewer :
		public EasyViewer
	{
	public:
		SelectedArrowViewer()
		{
			setViewerSize(40, 40);
		}

		void	update() override
		{
			Line(0, 20, 40, 20).drawArrow(10, Vec2(20, 20), Palette::Yellow);
		}
	};


public:
	shared_ptr<FacilityAsset>	m_selectedFacilityAsset;
	shared_ptr<RoadAsset>		m_selectedRoadAsset;
	Stopwatch					m_mouseOverTimer;

	FacilitiesListViewer()
		: m_mouseOverTimer(false)
	{}

	void	update() override;
	void	init() override;
};
