#pragma once

#include "Viewer.h"

class Popup
	: public Viewer
{
public:
	Popup(const Vec2& viewerSize);

	void	init() override;
};

