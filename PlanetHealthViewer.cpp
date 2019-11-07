#include "PlanetHealthViewer.h"
#include "PlanetManager.h"

void PlanetHealthViewer::update()
{
	static double d = 1.0;

	d = Math::Lerp(d, g_planetManagerPtr->m_health, 0.1);
	auto size = getViewerSize();
	RectF(size).draw(Palette::Black);
	RectF(size.x * d, size.y).draw(GetBarColor(d));
	RectF(size).drawFrame(5.0, 0.0, Palette::White);
}
