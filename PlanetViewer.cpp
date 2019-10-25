#include "PlanetViewer.h"
#include "PlanetManager.h"
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

	g_planetManagerPtr->loadRegions(U"asset/regions.json");
	//g_planetManagerPtr->generateRegions(2048);
	//g_planetManagerPtr->connectRegions();
	//g_planetManagerPtr->makeChips();

	g_planetManagerPtr->m_roads.resize(64);
	for (auto& r : g_planetManagerPtr->m_roads) { r = MakeShared<Road>(); r->m_fr = g_planetManagerPtr->m_regions.choice(); r->m_to = r->m_fr.lock()->m_connecteds.choice().lock(); }

	// Factory
	for (int i = 0; i < 50; ++i)
		g_planetManagerPtr->makeFacility(g_assetManagerPtr->getAsset<FactoryAsset>("Factory"), g_planetManagerPtr->m_regions.choice());

	// Quarry
	for (int i = 0; i < 50; ++i)
		g_planetManagerPtr->makeFacility(g_assetManagerPtr->getAsset<QuarryAsset>("Quarry"), g_planetManagerPtr->m_regions.choice());
}

void PlanetViewer::update()
{
	if (KeyRight.pressed()) m_azimuth += 0.02;
	if (KeyLeft.pressed()) m_azimuth -= 0.02;
	if (KeyUp.pressed()) m_inclination = Min(m_inclination + 0.02, 60_deg); ;
	if (KeyDown.pressed()) m_inclination = Max(m_inclination - 0.02, -60_deg);
	m_camera.setView(getEyePosition(), Vec3::Zero());

	g_planetManagerPtr->drawRegions(m_camera);
	g_planetManagerPtr->drawRoads(m_camera);
	g_planetManagerPtr->drawFacilities(m_camera);

	if (g_viewerManagerPtr->getViewer<FacilitiesListViewer>()->getSelectedIndex() >= 0)
	{
		if (auto r = g_planetManagerPtr->m_mouseOverRegion)
		{
			size_t index = g_viewerManagerPtr->getViewer<FacilitiesListViewer>()->getSelectedIndex();
			g_planetManagerPtr->makeFacility(g_assetManagerPtr->getAssets<FacilityAsset>()[index], r);
		}
	}

	if (g_viewerManagerPtr->getViewer<FacilitiesListViewer>()->getSelectedIndex() == -2)
	{
		auto r1 = g_planetManagerPtr->m_mouseOverRegion;
		auto r2 = g_planetManagerPtr->m_selectedRegion;
		if(r1 && r2 && r1 != r2)
		{
			if (!g_planetManagerPtr->m_roads.any([&r1, &r2](const auto& r) { return (r->m_fr.lock() == r1 && r->m_to.lock() == r2) || (r->m_fr.lock() == r1 && r->m_to.lock() == r2); }))
			{
				g_planetManagerPtr->m_roads.emplace_back(MakeShared<Road>(r1, r2));
			}
		}
	}
}
