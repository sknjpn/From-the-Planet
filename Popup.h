#pragma once

#include "EasyViewer.h"

class Popup
	: public EasyViewer
{
public:
	Popup(const Vec2& viewerSize);

	void	init() override;
};

