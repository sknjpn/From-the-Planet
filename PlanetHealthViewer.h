#pragma once

#include "EasyViewer.h"

class PlanetHealthViewer
	: public EasyViewer
{
	double	m_expectedDecrease = 0;

public:
	void init() override
	{
		setViewerRectInLocal(Rect(1400, 100).setCenter(Scene::Center().x, 60));
	}

	void update() override;

	void	setExpectedDecrease(double expectedDecrease) { m_expectedDecrease = expectedDecrease; }

	ColorF GetBarColor(double rate)
	{
		if (rate > 0.5)
		{
			return Palette::Green;
		}
		else if (rate > 0.2)
		{
			return Palette::Yellow;
		}
		else
		{
			return Palette::Red;
		}
	}
};

