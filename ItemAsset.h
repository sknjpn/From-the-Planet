#pragma once

#include "Asset.h"

class ItemAsset :
	public Asset
{
	Texture	m_texture;

public:
	// Get
	const Texture& getTexture() const { return m_texture; }

	// JSON
	void	load(const JSONValue& json) override;
};

