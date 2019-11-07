#include "FacilityConstructionPopup.h"
#include "FacilityAsset.h"

FacilityConstructionPopup::FacilityConstructionPopup(const shared_ptr<FacilityAsset>& facilityAsset)
	: Popup(Vec2(320, 240))
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
		moveDrawPos(0, 20);
	}

	// テキスト
	{
		static Font font(18, Typeface::Bold);

		moveDrawPos(5, 0);

		font(m_facilityAsset->getBuildText()).draw();

		moveDrawPos(-5, 0);
	}
}
