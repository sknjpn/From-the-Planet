#include "FacilityConstructionPopup.h"
#include "FacilityAsset.h"

FacilityConstructionPopup::FacilityConstructionPopup(const shared_ptr<FacilityAsset>& facilityAsset)
	: Popup(Vec2(320, 240))
	, m_facilityAsset(facilityAsset)
{

}

void FacilityConstructionPopup::update()
{
	// 見出し
	{
		static Font font(24);

		font(m_facilityAsset->getName()).draw();
	}
}
