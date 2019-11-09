#include "QuarryState.h"
#include "QuarryAsset.h"
#include "PlanetManager.h"
#include "NaturalResourceAsset.h"
#include "Region.h"

void QuarryState::update()
{
	FacilityState::update();
	auto asset = dynamic_pointer_cast<QuarryAsset>(m_facilityAsset);

	if (m_isActive && m_export.isEmpty())
	{
		m_progress += 0.01;
		if (m_progress >= 1.0)
		{
			m_progress -= Random(0.9, 1.0);
			m_export.addItem(asset->m_export);

			g_planetManagerPtr->addDamage(0.005);

			m_audio.playOneShot(masterVolume * 0.5);
		}
	}
}

void QuarryState::onInit()
{
	FacilityState::onInit();

	Array<shared_ptr<Region>> regions;
	regions.emplace_back(m_region);

	for (int i = 0; i < regions.size(); ++i)
	{
		const auto region = regions[i];

		for (const auto& r : region->getConnecteds())
			if (r.lock()->getTerrainAsset() == m_region.lock()->getTerrainAsset() && !regions.any([r](const auto& it) { return r.lock() == it; }))
				regions.emplace_back(r);
	}

	for (const auto& r : regions)
		r->setNaturalResourceAsset(dynamic_pointer_cast<QuarryAsset>(m_facilityAsset)->m_export);
}
