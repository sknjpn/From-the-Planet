#include "FacilityConstructionPopup.h"
#include "FacilityCostViewer.h"
#include "FacilityDescPopup.h"
#include "FacilityAsset.h"

FacilityConstructionPopup::FacilityConstructionPopup(const shared_ptr<FacilityAsset>& facilityAsset)
	: Popup(Vec2(300, 200))
	, m_facilityAsset(facilityAsset)
{
}

void FacilityConstructionPopup::update()
{
	moveDrawPos(5, 5);

	// 見出し
	{
		static Font font(20, Typeface::Heavy);

		font(m_facilityAsset->getName()).draw();
		moveDrawPos(0, 30);
	}

	// テキスト
	{
		static Font font(16, Typeface::Bold);

		moveDrawPos(10, 0);

		font(m_facilityAsset->getBuildText()).draw();

		moveDrawPos(-10, 0);
	}
}

void FacilityConstructionPopup::init()
{
	Popup::init();

	const auto fcv = addChildViewer<FacilityCostViewer>(m_facilityAsset);
	fcv->setViewerPos(getViewerRect().pos.movedBy(0, 200 - 48));

	const auto fdp = addChildViewer<FacilityDescPopup>(m_facilityAsset);
	fdp->setViewerRect(getViewerRect().pos, Vec2(300, 200 - 48));
}
