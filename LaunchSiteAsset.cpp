#include "LaunchSiteAsset.h"
#include "LaunchSiteState.h"
#include "FacilityDescPopup.h"

shared_ptr<FacilityState> LaunchSiteAsset::makeState()
{
	return MakeShared<LaunchSiteState>();
}

void LaunchSiteAsset::initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const
{
}

void LaunchSiteAsset::updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const
{
	popup->moveDrawPos(5, 5);

	// タイトル
	{
		static Font font(20, Typeface::Bold);
		font(getName()).draw();

		popup->moveDrawPos(0, 32);
	}

	// 説明
	{
		static Font font(16, Typeface::Bold);

		{
			auto f = font(U"宇宙へと脱出するために\nロケットの発射をします");
			f.draw();
			popup->moveDrawPos(0, f.region().h + 8);
		}
	}
}

void LaunchSiteAsset::load(const JSONValue& json)
{
	FacilityAsset::load(json);

	m_soundPath = FileSystem::ParentPath(getFilepath()) + json[U"sound"].getString();
}
