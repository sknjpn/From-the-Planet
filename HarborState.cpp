#include "HarborState.h"
#include "PlanetManager.h"
#include "RoadAsset.h"
#include "Region.h"
#include "Road.h"
#include "AssetManager.h"
#include "TerrainAsset.h"

void HarborState::onInit()
{
	FacilityState::onInit();

	auto seaRoad = g_assetManagerPtr->getAsset<RoadAsset>(U"海路");
	for (const auto& fs : g_planetManagerPtr->m_facilityStates)
	{
		if (shared_from_this() != fs && dynamic_pointer_cast<HarborState>(fs))
		{
			const auto route1 = m_region.lock()->getRouteToWithSea(fs->m_region.lock());
			const auto route2 = fs->m_region.lock()->getRouteToWithSea(m_region.lock());
			for (const auto& r : route1) r->setRoadAsset(seaRoad);
			for (const auto& r : route2) r->setRoadAsset(seaRoad);
		}
	}
}
