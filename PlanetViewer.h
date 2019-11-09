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
	Audio	m_audio;
	Audio	m_fCont;
	Audio	m_rCont;

public:
	PlanetViewer()
		: m_camera(Scene::Size(), m_fov, getEyePosition(), Vec3::Zero())
		, m_audio(U"asset/星の胎動.mp3")
		, m_fCont(U"asset/models/facilities/sound/sword-clash2.mp3")
		, m_rCont(U"asset/models/facilities/sound/punch-real1.mp3")
	{
		m_audio.setLoop(true);
		m_audio.setVolume(0.8);
		m_audio.play();
	}

	void	init() override;
	void	update() override;
	Vec3	getEyePosition() const { return Vec3(Cos(m_azimuth) * Cos(m_inclination), Sin(m_inclination), Sin(m_azimuth) * Cos(m_inclination)) * m_radius; }
};
