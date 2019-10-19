#include "FactoryAsset.h"
#include "FactoryState.h"

shared_ptr<FacilityState> FactoryAsset::makeState()
{
	return MakeShared<FactoryState>();
}
