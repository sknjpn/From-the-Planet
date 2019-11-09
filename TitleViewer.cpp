#include "TitleViewer.h"
#include "PlanetViewer.h"

void TitleViewer::update()
{
	// Title
	{
		static Font font(192, Typeface::Heavy);

		for (double th = 0.0; th < Math::TwoPi; th += 0.2)
			font(U"From The Planet").drawAt(Vec2(960, 135) + Vec2::Right(12).rotated(th), Palette::Gray);

		font(U"From The Planet").drawAt(960, 135, Palette::White);
	}

	// テキスト
	{
		static Font font(64, Typeface::Bold);

		font(U"Enterキーでスタート");
	}

	// 動作
	if (KeyEnter.down())
	{
		Viewer::GetRootViewer()->addChildViewer<PlanetViewer>();
		destroy();
	}
}
