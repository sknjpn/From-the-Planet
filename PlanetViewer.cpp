#include "PlanetViewer.h"
#include "Planet.h"
#include "FacilityState.h"
#include "Road.h"
#include "Region.h"
#include "AssetManager.h"
#include "FactoryAsset.h"
#include "QuarryAsset.h"

void PlanetViewer::init()
{
	m_planet = MakeShared<Planet>();
	m_planet->loadRegions(U"asset/regions.json");
	/*m_planet->generateRegions(2048);
	m_planet->connectRegions();
	m_planet->makeChips();
	*/

	m_planet->m_roads.resize(64);
	for (auto& r : m_planet->m_roads) { r = MakeShared<Road>(); r->m_fr = m_planet->m_regions.choice(); r->m_to = r->m_fr.lock()->m_connecteds.choice().lock(); }

	// Facility
}

void PlanetViewer::update()
{
	m_eyePosition = Cylindrical(Arg::r = 300, Arg::phi = Scene::Time() * 10_deg, Arg::y = 0);

	m_camera.setView(m_eyePosition, Vec3::Zero());

	m_planet->drawRegions(m_camera);
	m_planet->drawRoads(m_camera);
}
