﻿#include "FacilitiesListViewer.h"
#include "AssetManager.h"
#include "FacilityAsset.h"

void FacilitiesListViewer::update()
{
	setDrawPos(5, 5);

	static Font font(20, Typeface::Bold);

	{
		auto f = font(U"道路");
		auto r = Rect(120, 40);

		if (r.leftClicked()) m_selectedIndex = -2;

		auto color = Palette::Gray;
		if (m_selectedIndex == -2) r.draw(color.lerp(Palette::White, 0.75)).drawFrame(2.0, ColorF(1.0, 1.0));
		else r.draw(color.lerp(Palette::White, r.mouseOver() ? 0.5 : 0.25)).drawFrame(2.0, ColorF(1.0, 1.0));

		for (auto p : step(Size(-5, -5), Size(10, 10)))
			f.drawAt(r.center().movedBy(Vec2(p) / 5));
		f.drawAt(r.center(), Palette::Black);

		moveDrawPos(0, 48);
	}

	auto fas = g_assetManagerPtr->getAssets<FacilityAsset>();
	for (int i = 0; i < fas.size(); ++i)
	{
		auto& fa = fas[i];
		auto f = font(fa->getName());
		auto r = Rect(120, 40);

		if (r.leftClicked()) m_selectedIndex = i; 

		auto color = fa->getMeshes().front().m_color;
		if (m_selectedIndex == i) r.draw(color.lerp(Palette::White, 0.75)).drawFrame(2.0, ColorF(1.0, 1.0));
		else r.draw(color.lerp(Palette::White, r.mouseOver() ? 0.5 : 0.25)).drawFrame(2.0, ColorF(1.0, 1.0));

		for (auto p : step(Size(-5, -5), Size(10, 10)))
			f.drawAt(r.center().movedBy(Vec2(p) / 5));
		f.drawAt(r.center(), Palette::Black);

		moveDrawPos(0, 48);
	}
}

void FacilitiesListViewer::init()
{

}
