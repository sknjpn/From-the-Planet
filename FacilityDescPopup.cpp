#include "FacilityDescPopup.h"
#include "FacilityAsset.h"

void FacilityDescPopup::init()
{
	m_facilityAsset->initOnDescPopup(dynamic_pointer_cast<FacilityDescPopup>(shared_from_this()));
}

void FacilityDescPopup::update()
{
	RectF(getViewerSize()).drawFrame(1.0, Palette::White);

	m_facilityAsset->updateOnDescPopup(dynamic_pointer_cast<FacilityDescPopup>(shared_from_this()));
}
