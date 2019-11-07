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
