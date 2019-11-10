#pragma once

# include "Viewer.h"

class PlanetManager;

class PlanetViewer
	: public Viewer
{
	double m_fov = 45_deg;
	double m_inclination = 0.0;
	double m_azimuth = 0.0;
	double m_radius = 300.0;
	BasicCamera3D m_camera;

public:
	void	init() override;
	void	update() override;
	Vec3	getEyePosition() const { return Vec3(Cos(m_azimuth) * Cos(m_inclination), Sin(m_inclination), Sin(m_azimuth) * Cos(m_inclination)) * m_radius; }
};
