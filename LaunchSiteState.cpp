#include "LaunchSiteState.h"
#include "PlanetManager.h"
#include "ClearViewer.h"
#include "PlanetViewer.h"
#include "FacilitiesListViewer.h"

void LaunchSiteState::onConstructed()
{
	m_audio.playOneShot(0.5 * masterVolume, 1.0);

	Viewer::GetRootViewer()
		->getChildViewer<PlanetViewer>()
		->addChildViewer<ClearViewer>();

	Viewer::GetRootViewer()
		->getChildViewer<PlanetViewer>()
		->getChildViewer<FacilitiesListViewer>()->destroy();

	Twitter::OpenTweetWindow(U"From the Planetを" + Format(g_planetManagerPtr->m_sw.s()) + U"秒でクリアしたよ！#FromThePlanet");
}
