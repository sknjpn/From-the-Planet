#pragma once

#include "Asset.h"

class RoadAsset :
	public Asset
{
	Color	m_colorInside;
	Color	m_colorOutside;

	// JSON
	void	load(const JSONValue& json) override;
};

