#include "HarborAsset.h"
#include "HarborState.h"
#include "Region.h"
#include "TerrainAsset.h"

shared_ptr<FacilityState> HarborAsset::makeState()
{
	return MakeShared<HarborState>();
}

bool HarborAsset::canConstructAt(const shared_ptr<Region>& region) const
{
	for (const auto& r : region->getConnecteds())
		if (r.lock()->getTerrainAsset()->m_isSea) return true;
	
	return false;
}

String HarborAsset::getBuildText() const
{
	return FacilityAsset::getBuildText();
}
