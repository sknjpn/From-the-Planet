#include "LaunchSiteState.h"
#include "PlanetHealthViewer.h"
#include "PlanetManager.h"

void LaunchSiteState::onConstructed()
{
	g_viewerManagerPtr->getViewer<PlanetHealthViewer>()->addDamage(1.5);

	m_audio.playOneShot(0.5, 1.0);

	Twitter::OpenTweetWindow(U"From the Planetを" + Format(g_planetManagerPtr->m_sw.s()) + U"秒でクリアしたよ！#FromThePlanet");
}
