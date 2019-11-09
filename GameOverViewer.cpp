#include "GameOverViewer.h"
#include "PlanetViewer.h"
#include "PlanetManager.h"

void GameOverViewer::update()
{
	RectF(getViewerSize()).draw(ColorF(0.0, 0.75));

	{
		static Font font(64, Typeface::Heavy);

		font(U"脱出する前に惑星が崩壊してしまった").drawAt((getViewerSize() / 2.0).movedBy(0, -16));
	}
	{
		static Font font(20, Typeface::Heavy);

		font(U"もう一度プレイするにはEnterキーを押してください").drawAt(getViewerSize().x / 2, 108);
	}

	if (KeyEnter.down())
	{
		g_planetManagerPtr->reset();
		Viewer::GetRootViewer()->getChildViewer<PlanetViewer>()->destroy();
		Viewer::GetRootViewer()->addChildViewer<PlanetViewer>();
	}
}