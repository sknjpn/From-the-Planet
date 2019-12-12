#include "LaunchSiteState.h"
#include "LaunchSiteAsset.h"
#include "PlanetManager.h"
#include "ClearViewer.h"
#include "PlanetViewer.h"
#include "FacilitiesListViewer.h"
#include "SoundManager.h"

void LaunchSiteState::onConstructed()
{
	g_soundManagerPtr->playSoundEffect(dynamic_pointer_cast<LaunchSiteAsset>(m_facilityAsset)->getSoundPath(), SoundType::Environment);

	EasyViewer::GetRootViewer()
		->getChildViewer<PlanetViewer>()
		->addChildViewer<ClearViewer>();

	EasyViewer::GetRootViewer()
		->getChildViewer<PlanetViewer>()
		->getChildViewer<FacilitiesListViewer>()->destroy();

	Twitter::OpenTweetWindow(U"From the Planetを" + Format(g_planetManagerPtr->m_sw.s()) + U"秒でクリアしたよ！#FromThePlanet");
}
