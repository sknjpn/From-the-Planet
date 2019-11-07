#pragma once

#include "ItemAsset.h"

class NaturalResourceAsset :
	public ItemAsset
{
	double	m_amount;
	double	m_numSite;

public:
	// Get
	double	getAmount() const { return m_amount; }
	double	getNumSite() const { return m_numSite; }

	// JSON
	void	load(const JSONValue& json) override;
};

