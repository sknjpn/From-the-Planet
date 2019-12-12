#pragma once

#include "EasyViewer.h"

class ClearViewer :
	public EasyViewer
{
public:
	ClearViewer()
	{
		setViewerRectInLocal(Rect(1200, 132).setCenter(Scene::Center()));
	}

	void update() override;
};

