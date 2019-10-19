#pragma once

# include "Viewer.h"

class Planet;

class PlanetViewer
	: public Viewer
{
	shared_ptr<Planet> m_planet;
	double m_fov = 45_deg;
	Vec3 m_eyePosition;
	BasicCamera3D m_camera;

public:
	PlanetViewer()
		: m_eyePosition(0, 200, 0)
		, m_camera(Scene::Size(), m_fov, m_eyePosition, Vec3::Zero())
	{}

	void	init() override;
	void	update() override;
};
