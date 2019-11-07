#include "LaunchSiteAsset.h"
#include "LaunchSiteState.h"

shared_ptr<FacilityState> LaunchSiteAsset::makeState()
{
	return MakeShared<LaunchSiteState>();
}

String LaunchSiteAsset::getBuildText() const
{
	String result;

	result += Format(U"完成すると宇宙へとロケットを飛ばし惑星から脱出できる\n");

	result += FacilityAsset::getBuildText();

	return result;
}
