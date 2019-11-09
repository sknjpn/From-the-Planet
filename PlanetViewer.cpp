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

	if (auto phv = getChildViewer<PlanetHealthViewer>()) phv->setExpectedDecrease(0.0);

	if (auto flv = getChildViewer<FacilitiesListViewer>())
	{
		const auto mr = g_planetManagerPtr->m_mouseOverRegion;
		const auto sr = g_planetManagerPtr->m_selectedRegion;
		const Vec2 mrp = mr ? TranslateWorldToScreen(m_camera.getMat4x4(), mr->getPosition()) : Vec2::Zero();
		const Vec2 srp = sr ? TranslateWorldToScreen(m_camera.getMat4x4(), sr->getPosition()) : Vec2::Zero();

		if (mr && (flv->m_selectedFacilityAsset || flv->m_selectedRoadAsset))
		{
			for (double th = 0; th < Math::TwoPi; th += 0.1)
				Line(mrp.movedBy(0, -48), mrp).movedBy(Vec2::Right(3).rotated(th)).drawArrow(12, Vec2(20, 20), Palette::Black);
		}

		// 建物設置
		if (flv->m_selectedFacilityAsset && mr)
		{
			if (!mr->getFacilityState() && mr->getTerrainAsset()->m_buildAvailable && flv->m_selectedFacilityAsset->canConstructAt(mr))
			{
				Line(mrp.movedBy(0, -48), mrp).drawArrow(12, Vec2(20, 20), Palette::Green);

				getChildViewer<PlanetHealthViewer>()->setExpectedDecrease(flv->m_selectedFacilityAsset->getConstructinDamage());

				if (MouseL.down())
				{
					g_planetManagerPtr->makeFacility(flv->m_selectedFacilityAsset, mr);

					m_fCont.playOneShot(0.5 * masterVolume);

					// update
					for (const auto& fs : g_planetManagerPtr->m_facilityStates) fs->updateConnected();
				}
			}
			else Line(mrp.movedBy(0, -48), mrp).drawArrow(12, Vec2(20, 20), Palette::Red);
		}

		// 道路敷設
		if (flv->m_selectedRoadAsset && mr)
		{
			if (sr)
			{
				const auto func = [](const shared_ptr<Road>& r) { return !r->getFr()->getTerrainAsset()->m_isSea && !r->getTo()->getTerrainAsset()->m_isSea; };
				g_planetManagerPtr->bakeCostMap(sr, func);
				const auto route = g_planetManagerPtr->getRoute(mr, sr);

				for (double th = 0; th < Math::TwoPi; th += 0.1)
					Line(srp.movedBy(0, -48), srp).movedBy(Vec2::Right(3).rotated(th)).drawArrow(12, Vec2(20, 20), Palette::Black);

				if (route)
				{
					double damage = 0.0;
					for (const auto& road : route)
					{
						if (road->getRoadAsset() != flv->m_selectedRoadAsset)
							damage += flv->m_selectedRoadAsset->getConstructinDamage();

						auto p0 = road->getFr()->getPosition();
						auto p1 = road->getTo()->getPosition();
						auto c = (p0 + p1) / 2.0;
						auto d = (p1 - p0).cross(p1).normalized() * 1.5;
						Mesh(Array<Vec3>({ p0 + d, p0 - d, p1 - d, p1 + d }), ColorF(1.0, 0.5), 4.0).drawFill(m_camera.getMat4x4());
					}

					Line(mrp.movedBy(0, -48), mrp).drawArrow(12, Vec2(20, 20), Palette::Green);
					Line(srp.movedBy(0, -48), srp).drawArrow(12, Vec2(20, 20), Palette::Green);

					getChildViewer<PlanetHealthViewer>()->setExpectedDecrease(damage);

					if (MouseL.up())
					{
						for (const auto& r : route)
						{
							r->setRoadAsset(flv->m_selectedRoadAsset);
							r->getOppositeRoad()->setRoadAsset(flv->m_selectedRoadAsset);
						}
						m_rCont.playOneShot(0.5 * masterVolume);
						g_planetManagerPtr->addDamage(damage);

						// update
						for (const auto& fs : g_planetManagerPtr->m_facilityStates) fs->updateConnected();
					}
				}
				else
				{
					Line(mrp.movedBy(0, -48), mrp).drawArrow(12, Vec2(20, 20), Palette::Red);
					Line(srp.movedBy(0, -48), srp).drawArrow(12, Vec2(20, 20), Palette::Red);
				}
			}
			else
			{
				if (mr->getTerrainAsset()->m_buildAvailable) Line(mrp.movedBy(0, -48), mrp).drawArrow(12, Vec2(20, 20), Palette::Green);
				else Line(mrp.movedBy(0, -48), mrp).drawArrow(12, Vec2(20, 20), Palette::Red);
			}
		}

		if (g_planetManagerPtr->m_destroy >= 0) flv->destroy();
	}

	g_planetManagerPtr->updateMouseOver(m_camera);

	g_planetManagerPtr->drawFacilities(m_camera);

	{
		static Font font(80, Typeface::Bold);
		font(Format(g_planetManagerPtr->m_sw.s()) + U"秒").draw(Vec2(10, 5));
	}

}