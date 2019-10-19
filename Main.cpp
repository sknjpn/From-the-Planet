﻿#include "Planet.h"
#include "Facility.h"
#include "Road.h"
#include "Region.h"

void Main()
{
	// GeneratePositions(2048);

	Window::Resize(1280, 720);
	Scene::SetBackground(Palette::Black);// ColorF(0.05, 0.3, 0.7));

	constexpr double fov = 45_deg;
	Vec3 eyePositon(0, 200, 0);
	BasicCamera3D camera(Scene::Size(), fov, eyePositon, Vec3::Zero());

	Planet planet;

	planet.loadRegions(U"regions.json");
	/*planet.generateRegions(2048);
	planet.connectRegions();
	planet.makeChips();
	*/
	Array<Facility> facility(50);

	for (auto& f : facility)
		f.m_region = planet.m_regions.choice();

	planet.m_roads.resize(64);
	for (auto& r : planet.m_roads) { r = MakeShared<Road>(); r->m_fr = planet.m_regions.choice(); r->m_to = r->m_fr.lock()->m_connecteds.choice().lock(); }

	while (System::Update())
	{
		eyePositon = Cylindrical(Arg::r = 300, Arg::phi = Scene::Time() * 10_deg, Arg::y = 0);

		camera.setView(eyePositon, Vec3::Zero());

		planet.drawRegions(camera);
		planet.drawRoads(camera);

		if (KeyEnter.down())
			for (auto& f : facility)
				f.m_region = planet.m_regions.choice();

		for (auto& f : facility)
			f.draw(camera);
	}
}
