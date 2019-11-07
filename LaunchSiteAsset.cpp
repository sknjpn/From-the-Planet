#include "LaunchSiteAsset.h"
#include "LaunchSiteState.h"

shared_ptr<FacilityState> LaunchSiteAsset::makeState()
{
	return MakeShared<LaunchSiteState>();
}
