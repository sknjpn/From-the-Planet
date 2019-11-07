#include "FacilitiesListViewer.h"
#include "AssetManager.h"
#include "FacilityAsset.h"
#include "RoadAsset.h"
#include "FacilityConstructionPopup.h"

void FacilitiesListViewer::update()
{
	setDrawPos(5, 5);

	static Font font(20, Typeface::Bold);

	auto rs = g_assetManagerPtr->getAssets<RoadAsset>();
	for (const auto& fa : rs)
	{
		if (fa->getName() == U"海路") continue;
		auto f = font(fa->getName());
		auto r = Rect(120, 40);

		if (r.leftClicked())
		{
			m_selectedRoadAsset = fa;
			m_selectedFacilityAsset = nullptr;
		}

		auto color = fa->m_colorInside;
		if (m_selectedRoadAsset == fa) r.draw(color.lerp(Palette::White, 0.75)).drawFrame(2.0, ColorF(1.0, 1.0));
		else r.draw(color.lerp(Palette::White, r.mouseOver() ? 0.5 : 0.25)).drawFrame(2.0, ColorF(1.0, 1.0));

		for (auto p : step(Size(-5, -5), Size(10, 10)))
			f.drawAt(r.center().movedBy(Vec2(p) / 5));
		f.drawAt(r.center(), Palette::Black);

		moveDrawPos(0, 48);
	}

	auto fas = g_assetManagerPtr->getAssets<FacilityAsset>();
	for (const auto& fa : fas)
	{
		auto f = font(fa->getName());
		auto r = Rect(120, 40);

		if (r.leftClicked())
		{
			m_selectedRoadAsset = nullptr;
			m_selectedFacilityAsset = fa;
		}

		if (r.contains(Cursor::PreviousPosF()))
		{
			if (r.mouseOver())
			{
				if (!m_mouseOverTimer.isRunning()) m_mouseOverTimer.start();
				if (m_mouseOverTimer.sF() > 0.5 && !hasChildViewer<FacilityConstructionPopup>())
					addChildViewer<FacilityConstructionPopup>(fa);
			}
			else
			{
				m_mouseOverTimer.reset();

				if (auto fcp = getChildViewer<FacilityConstructionPopup>())
					fcp->destroy();
			}
		}

		auto color = fa->getMeshes().front().m_color;
		if (m_selectedFacilityAsset == fa) r.draw(color.lerp(Palette::White, 0.75)).drawFrame(2.0, ColorF(1.0, 1.0));
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
