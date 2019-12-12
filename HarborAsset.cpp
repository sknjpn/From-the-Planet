#include "HarborAsset.h"
#include "HarborState.h"
#include "Region.h"
#include "TerrainAsset.h"
#include "FacilityDescPopup.h"

shared_ptr<FacilityState> HarborAsset::makeState()
{
	return MakeShared<HarborState>();
}

bool HarborAsset::canConstructAt(const shared_ptr<Region>& region) const
{
	for (const auto& r : region->getConnecteds())
		if (r.lock()->getTerrainAsset()->m_isSea) return true;
	
	return false;
}

void HarborAsset::initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const
{
}

void HarborAsset::updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const
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
			auto f = font(U"海路を利用して\n遠隔地に早く資源を届けます");
			f.draw();
			popup->moveDrawPos(0, f.region().h);
		}

		{
			auto f = font(U"沿岸にのみ建設可");
			f.draw(Vec2::Zero(), Palette::Red);
			popup->moveDrawPos(0, f.region().h);
		}
	}
}

void HarborAsset::load(const JSONValue& json)
{
	FacilityAsset::load(json);
}
