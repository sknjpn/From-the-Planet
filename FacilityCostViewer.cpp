#include "FacilityCostViewer.h"
#include "FacilityAsset.h"
#include "ItemAsset.h"

void FacilityCostViewer::update()
{
	for (const auto& m : m_facilityAsset->getMaterial().getItemList())
	{
		m.first->getTexture().draw();
	}
}
