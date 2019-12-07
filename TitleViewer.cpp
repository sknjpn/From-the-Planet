#include "TitleViewer.h"
#include "PlanetViewer.h"
#include "SoundManager.h"

TitleViewer::TitleViewer()
{
	g_soundManagerPtr->playMusic(U"asset/Devastates_stageselect.mp3");
}

void TitleViewer::update()
{
	// Title
	{
		static Font font(192, Typeface::Heavy);

		for (double th = 0.0; th < Math::TwoPi; th += 0.2)
			font(U"From The Planet").drawAt(Vec2(getViewerSize().x / 2, getViewerSize().y * 1.0 / 4.0) + Vec2::Right(12).rotated(th), Palette::Gray);

		font(U"From The Planet").drawAt(getViewerSize().x / 2, getViewerSize().y * 1.0 / 4.0, Palette::White);
	}

	// テキスト
	{
		static Font font(32, Typeface::Bold);

		font(U"プレイするにはEnterキーを押してください").drawAt(getViewerSize().x / 2, getViewerSize().y * 3.0 / 4.0);
	}

	// 動作
	if (KeyEnter.down())
	{
		EasyViewer::GetRootViewer()->addChildViewer<PlanetViewer>();
		destroy();
	}
}
