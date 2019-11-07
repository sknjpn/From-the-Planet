#include "QuarryState.h"
#include "QuarryAsset.h"
#include "PlanetHealthViewer.h"
#include "ViewerManager.h"

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

			g_viewerManagerPtr->getViewer<PlanetHealthViewer>()->addDamage(0.004);

			m_audio.playOneShot(m_volume * 0.8, 1.0);
		}
	}
}
