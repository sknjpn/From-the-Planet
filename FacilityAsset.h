#pragma once

#include "Asset.h"

class FacilityState;

class FacilityAsset :
	public Asset
{
	shared_ptr<FacilityState>	m_facilityState;

public:
	virtual shared_ptr<FacilityState>	makeState() = 0;

	template<typename T>
	shared_ptr<T>	getFacilityState() const { return dynamic_pointer_cast<T>(m_facilityState); }
};

