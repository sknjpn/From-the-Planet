#include "PlanetViewer.h"
#include "Planet.h"
#include "FacilityState.h"
#include "Road.h"
#include "Region.h"
#include "AssetManager.h"
#include "FactoryAsset.h"
#include "QuarryAsset.h"
#include "ViewerManager.h"
#include "FacilitiesListViewer.h"

void PlanetViewer::init()
{
	g_viewerManagerPtr->makeViewer<FacilitiesListViewer>();

	m_planet = MakeShared<Planet>();
	m_planet->loadRegions(U"asset/regions.json");
	/*m_planet->generateRegions(2048);
	m_planet->connectRegions();
	m_planet->makeChips();
	*/

	m_planet->m_roads.resize(64);
	for (auto& r : m_planet->m_roads) { r = MakeShared<Road>(); r->m_fr = m_planet->m_regions.choice(); r->m_to = r->m_fr.lock()->m_connecteds.choice().lock(); }

	// Factory
	for (int i = 0; i < 50; ++i)
		m_planet->makeFacility(g_assetManagerPtr->getAsset<FactoryAsset>("Factory"), m_planet->m_regions.choice());

	// Quarry
	for (int i = 0; i < 50; ++i)
		m_planet->makeFacility(g_assetManagerPtr->getAsset<QuarryAsset>("Quarry"), m_planet->m_regions.choice());
}

void PlanetViewer::update()
{
	if (KeyRight.pressed()) m_azimuth += 0.02;
	if (KeyLeft.pressed()) m_azimuth -= 0.02;
	if (KeyUp.pressed()) m_inclination = Min(m_inclination + 0.02, 60_deg); ;
	if (KeyDown.pressed()) m_inclination = Max(m_inclination - 0.02, -60_deg);
	m_camera.setView(getEyePosition(), Vec3::Zero());

	m_planet->drawRegions(m_camera);
	m_planet->drawRoads(m_camera);
	m_planet->drawFacilities(m_camera);


	if (g_viewerManagerPtr->getViewer<FacilitiesListViewer>()->getSelectedIndex() != -1)
	{
		for (auto& r : m_planet->m_regions)
		{
			if (r->m_isMouseover && MouseL.down())
			{
				size_t index = g_viewerManagerPtr->getViewer<FacilitiesListViewer>()->getSelectedIndex();
				g_viewerManagerPtr->getViewer<PlanetViewer>()->getPlanet()->makeFacility(g_assetManagerPtr->getAssets<FacilityAsset>()[index], r);
			}
		}
	}
}
