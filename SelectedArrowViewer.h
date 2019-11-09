#pragma once

#include "Viewer.h"

class SelectedArrowViewer :
	public Viewer
{
public:
	SelectedArrowViewer()
	{
		setViewerSize(40, 40);
	}

	void	update() override
	{
		Line(0, 20, 40, 20).drawArrow(10, Vec2(20, 20), Palette::Yellow);
	}
};

