#pragma once

#include "Asset.h"

class TerrainAsset :
	public Asset
{
public:
	Color	m_color;
	bool	m_buildAvailable;

	// JSON
	void	load(const JSONValue& json) override;
};

