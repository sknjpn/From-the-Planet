#pragma once

#include "Asset.h"

class RoadAsset :
	public Asset
{
	double m_speed;

public:
	Color	m_colorInside;
	Color	m_colorOutside;

	double getSpeed() const { return m_speed; }

	// JSON
	void	load(const JSONValue& json) override;
};

