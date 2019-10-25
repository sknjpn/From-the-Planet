#include "FacilitiesListViewer.h"
#include "AssetManager.h"
#include "FacilityAsset.h"

void FacilitiesListViewer::update()
{
	Print << m_selectedIndex;
	setDrawPos(5, 5);
	if (MouseL.down()) m_selectedIndex = -1;

	static Font font(24, Typeface::Bold);

	{
		auto f = font(U"Road");
		auto r = Rect(120, 40);

		if (r.leftClicked()) m_selectedIndex = -2;

		if (m_selectedIndex == -2) r.draw(ColorF(Palette::Red, 1.0)).drawFrame(2.0, ColorF(1.0, 1.0));
		else r.draw(ColorF(1.0, r.mouseOver() ? 0.75 : 0.25)).drawFrame(2.0, ColorF(1.0, 1.0));
		
		f.drawAt(r.center());
		moveDrawPos(0, 48);
	}

	auto fas = g_assetManagerPtr->getAssets<FacilityAsset>();
	for (int i = 0; i < fas.size(); ++i)
	{
		auto& fa = fas[i];
		auto f = font(Unicode::Widen(fa->getName()));
		auto r = Rect(120, 40);

		if (r.leftClicked()) m_selectedIndex = i;

		if(m_selectedIndex == i) r.draw(ColorF(Palette::Red, 1.0)).drawFrame(2.0, ColorF(1.0, 1.0));
		else r.draw(ColorF(1.0, r.mouseOver() ? 0.75 : 0.25)).drawFrame(2.0, ColorF(1.0, 1.0));
		
		f.drawAt(r.center());

		moveDrawPos(0, 48);
	}
}

void FacilitiesListViewer::init()
{

}
