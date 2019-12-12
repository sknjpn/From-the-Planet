#pragma once

#include "ItemAsset.h"

class NaturalResourceAsset :
	public ItemAsset
{
	double	m_amount;
	double	m_numSite;
	Color	m_color;

public:
	// Get
	double	getAmount() const { return m_amount; }
	double	getNumSite() const { return m_numSite; }
	Color	getColor() const { return m_color; }

	// JSON
	void	load(const JSONValue& json) override;
};

