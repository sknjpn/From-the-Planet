#include "QuarryAsset.h"
#include "QuarryState.h"

shared_ptr<FacilityState> QuarryAsset::makeState()
{
	return MakeShared<QuarryState>();
}
