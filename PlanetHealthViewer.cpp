#include "PlanetHealthViewer.h"
#include "PlanetManager.h"

void PlanetHealthViewer::update()
{
	static double d = 1.0;

	d = Math::Lerp(d, m_health, 0.1);
	auto size = getViewerSize();
	RectF(size).draw(Palette::Black);
	RectF(size.x * d, size.y).draw(GetBarColor(d));
	RectF(size).drawFrame(5.0, 0.0, Palette::White);


	if (m_health > 0)
		m_health = Min(1.0, m_health + 0.0015);
}

void PlanetHealthViewer::addDamage(double value)
{
	if (m_health > 0)
	{
		m_health -= value;

		if (m_health < 0)
			g_planetManagerPtr->destroy();
	}
}
