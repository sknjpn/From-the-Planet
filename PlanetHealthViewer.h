#pragma once

#include "Viewer.h"

class PlanetHealthViewer
	: public Viewer
{
public:

	double m_health = 1.0;

	PlanetHealthViewer()
	{
		setDrawRect(Rect(1400, 100).setCenter(Scene::Center().x, 60));
	}

	void update() override;

	void addDamage(double value);

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

