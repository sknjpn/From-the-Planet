#include "QuarryAsset.h"
#include "QuarryState.h"
#include "AssetManager.h"
#include "ItemAsset.h"
#include "TerrainAsset.h"
#include "AssetManager.h"
#include "NaturalResourceAsset.h"
#include "FacilityDescPopup.h"
#include "Region.h"

shared_ptr<FacilityState> QuarryAsset::makeState()
{
	return MakeShared<QuarryState>();
}

bool QuarryAsset::canConstructAt(const shared_ptr<Region>& region) const
{
	return (!region->getNaturalResourceAsset() || region->getNaturalResourceAsset() == m_export) &&
		region->getTerrainAsset() == g_assetManagerPtr->getAsset<TerrainAsset>(U"岩地");
}

void QuarryAsset::initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const
{
}

void QuarryAsset::updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const
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
			auto f = font(U"新たな資源を採掘します");
			f.draw();
			popup->moveDrawPos(0, f.region().h);
		}

		{
			auto f = font(U"岩地にのみ建設可");
			f.draw(Vec2::Zero(), Palette::Red);
			popup->moveDrawPos(0, f.region().h + 8);
		}

		{
			auto dp = popup->getDrawPos();

			font(U"出力:").draw();
			popup->moveDrawPos(40, 0);

			{
				// 画像表示
				m_export->getTexture().resized(24, 24).draw();
				popup->moveDrawPos(24, 0);

				// 名前と個数表示
				const auto f = font(m_export->getName());
				f.draw();
				popup->moveDrawPos(f.boundingRect().w, 0);
			}

			popup->setDrawPos(dp.movedBy(0, 24));
		}
	}
}

void QuarryAsset::load(const JSONValue& json)
{
	FacilityAsset::load(json);

	m_export = g_assetManagerPtr->getAsset<NaturalResourceAsset>(json[U"export"].getString());

	m_soundPath = FileSystem::ParentPath(getFilepath()) + json[U"sound"].getString();
}
