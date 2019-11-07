#include "PlanetViewer.h"
#include "PlanetManager.h"
#include "FacilityState.h"
#include "Road.h"
#include "Region.h"
#include "AssetManager.h"
#include "FactoryAsset.h"
#include "QuarryAsset.h"
#include "FacilitiesListViewer.h"
#include "PlanetHealthViewer.h"
#include "TerrainAsset.h"
#include "RoadAsset.h"

void PlanetViewer::init()
{
	addChildViewer<FacilitiesListViewer>();
	addChildViewer<PlanetHealthViewer>();

	g_planetManagerPtr->loadRegions(U"asset/regions.json");
	//g_planetManagerPtr->generateRegions(2048);
	//g_planetManagerPtr->connectRegions();
	//g_planetManagerPtr->makeChips();
}

void PlanetViewer::update()
{
	m_radius *= (1.00 - 0.05 * Mouse::Wheel());
	m_radius = Clamp<double>(m_radius, 150, 500);

	if (KeyRight.pressed() || KeyD.pressed()) m_azimuth += 0.02;
	if (KeyLeft.pressed() || KeyA.pressed()) m_azimuth -= 0.02;
	if (KeyUp.pressed() || KeyW.pressed()) m_inclination = Min(m_inclination + 0.02, 60_deg); ;
	if (KeyDown.pressed() || KeyS.pressed()) m_inclination = Max(m_inclination - 0.02, -60_deg);
	m_camera.setView(getEyePosition(), Vec3::Zero());

	g_planetManagerPtr->update();
	g_planetManagerPtr->drawRegions(m_camera);
	if (g_planetManagerPtr->m_destroy < 0) g_planetManagerPtr->drawRoads(m_camera);
	g_planetManagerPtr->drawFacilities(m_camera);

	// 建物設置
	if (auto flv = getChildViewer<FacilitiesListViewer>())
	{
		if (flv->getSelectedIndex() >= 0)
		{
			size_t index = flv->getSelectedIndex();
			if (auto r = g_planetManagerPtr->m_mouseOverRegion)
			{
				if (MouseL.down() && !r->getFacilityState() && r->getTerrainAsset()->m_buildAvailable && g_assetManagerPtr->getAssets<FacilityAsset>()[index]->canConstructAt(r))
				{
					g_planetManagerPtr->makeFacility(g_assetManagerPtr->getAssets<FacilityAsset>()[index], r);

					g_planetManagerPtr->addDamage(0.1);

					m_fCont.playOneShot(0.5);
					// update
					for (const auto& fs : g_planetManagerPtr->m_facilityStates) fs->updateConnected();
				}
			}
		}

		// 道路敷設
		if (flv->getSelectedIndex() == -2)
		{
			auto r1 = g_planetManagerPtr->m_mouseOverRegion;
			auto r2 = g_planetManagerPtr->m_selectedRegion;
			if (r1 && r2 && r1 != r2 && r1->hasConnection(r2) && r1->getTerrainAsset()->m_buildAvailable && r2->getTerrainAsset()->m_buildAvailable)
			{
				if(!r1->getRoad(r2)->getRoadAsset())
				{
					r1->getRoad(r2)->setRoadAsset(g_assetManagerPtr->getAsset<RoadAsset>(U"砂利道"));
					r2->getRoad(r1)->setRoadAsset(g_assetManagerPtr->getAsset<RoadAsset>(U"砂利道"));
					g_planetManagerPtr->m_selectedRegion = g_planetManagerPtr->m_mouseOverRegion;
					m_rCont.playOneShot(0.5);
					g_planetManagerPtr->addDamage(0.01);

					// update
					for (const auto& fs : g_planetManagerPtr->m_facilityStates) fs->updateConnected();
				}
			}
		}

		if (g_planetManagerPtr->m_destroy >= 0) flv->destroy();
	}

	{
		static Font font(80, Typeface::Bold);
		font(Format(g_planetManagerPtr->m_sw.s()) + U"秒").draw(Vec2(10, 5));
	}

}