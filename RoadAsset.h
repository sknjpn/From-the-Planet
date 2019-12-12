#pragma once

#include "Asset.h"

class RoadAsset :
	public Asset
{
	double	m_speed;
	double	m_constructinDamage;

public:
	Color	m_colorInside;
	Color	m_colorOutside;

	double getSpeed() const { return m_speed; }
	double getConstructinDamage() const { return m_constructinDamage; }

	// JSON
	void	load(const JSONValue& json) override;
};

