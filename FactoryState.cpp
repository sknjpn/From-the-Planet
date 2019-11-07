#include "FactoryState.h"
#include "FactoryAsset.h"
#include "PlanetHealthViewer.h"

void FactoryState::update()
{
	FacilityState::update();
	auto asset = dynamic_pointer_cast<FactoryAsset>(m_facilityAsset);

	if (m_isActive && m_export.isEmpty() && m_storage >= asset->m_import)
	{
		m_progress += 0.01;
		if (m_progress >= 1.0)
		{
			m_progress -= Random(0.9, 1.0);
			m_export.addItem(asset->m_export);
			m_storage -= asset->m_import;
			m_need += asset->m_import;

			g_planetManagerPtr->addDamage(0.01);

			m_audio.playOneShot(m_volume, 1.0);
		}
	}
}

void FactoryState::onConstructed()
{
	auto asset = dynamic_pointer_cast<FactoryAsset>(m_facilityAsset);

	// 必要なものの登録
	Storage impt;
	
	for (int i = 0; i < 5; i++) impt += asset->m_import;

	m_need = impt;
}
