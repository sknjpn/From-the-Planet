#pragma once

#include "FacilityState.h"

class FactoryState
	: public FacilityState
{
public:
	void	update() override;
	void	onConstructed() override;
};
