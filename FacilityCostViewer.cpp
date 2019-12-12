#include "FacilityCostViewer.h"
#include "FacilityAsset.h"
#include "ItemAsset.h"

void FacilityCostViewer::update()
{
	RectF(getViewerSize()).drawFrame(1.0, Palette::White);

	static Font font(16, Typeface::Bold);

	moveDrawPos(5, 0);
	font(U"必要資源").draw();

	moveDrawPos(5, 24);
	for (const auto& m : m_facilityAsset->getMaterial().getItemList())
	{
		// 画像表示
		m.first->getTexture().resized(24, 24).draw();
		moveDrawPos(24, 0);

		// 名前と個数表示
		const auto f = font(m.first->getName(), U"x", m.second);
		f.draw();
		moveDrawPos(f.boundingRect().w, 0);
		
		moveDrawPos(8, 0);
	}
}
