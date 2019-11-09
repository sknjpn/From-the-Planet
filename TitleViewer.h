#pragma once

#include "Viewer.h"

class TitleViewer :
	public Viewer
{
	Audio	m_audio;

public:
	TitleViewer()
		: m_audio(U"asset/Devastates_stageselect.mp3")
	{
		m_audio.setLoop(true);
		m_audio.setVolume(1.0);
		m_audio.play();
	}

	void	update() override;
};

