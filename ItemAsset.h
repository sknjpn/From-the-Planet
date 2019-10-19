#pragma once

#include "Asset.h"

class ItemAsset :
	public Asset
{
	Texture	m_texture;

public:
	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
};

