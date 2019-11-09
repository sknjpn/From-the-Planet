#pragma once

#include "Viewer.h"

class ClearViewer :
	public Viewer
{
public:
	ClearViewer()
	{
		setViewerRect(Rect(1200, 132).setCenter(Scene::Center()));
	}

	void update() override;
};

