#pragma once

#include "Viewer.h"

class GameOverViewer
	: public Viewer
{
public:
	GameOverViewer()
	{
		setViewerRect(Rect(1200, 132).setCenter(Scene::Center()));
	}

	void update() override;
};

