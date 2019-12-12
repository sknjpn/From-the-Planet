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

	// Cost Map
	const auto func = [](const shared_ptr<Road>& r) { return
		(r->getFr()->getTerrainAsset()->m_isSea && r->getTo()->hasHarbor()) ||
		(r->getFr()->hasHarbor() && r->getTo()->getTerrainAsset()->m_isSea) ||
		(r->getFr()->getTerrainAsset()->m_isSea && r->getTo()->getTerrainAsset()->m_isSea);
	};

	g_planetManagerPtr->bakeCostMap(m_region.lock(), func);

	const auto seaRoad = g_assetManagerPtr->getAsset<RoadAsset>(U"海路");
	for (const auto& fs : g_planetManagerPtr->m_facilityStates)
	{
		if (fs->m_region.lock()->getFrom() && dynamic_pointer_cast<HarborState>(fs))
		{
			const auto route = g_planetManagerPtr->getRoute(fs->m_region.lock(), m_region.lock());
			for (const auto& r : route)
			{
				r->setRoadAsset(seaRoad);
				r->getOppositeRoad()->setRoadAsset(seaRoad);
			}
		}
	}
}

void HarborState::onConstructed()
{
	for (const auto& fs : g_planetManagerPtr->m_facilityStates)
		fs->updateConnected();
}
