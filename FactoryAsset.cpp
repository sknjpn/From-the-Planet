#include "FactoryAsset.h"
#include "FactoryState.h"
#include "AssetManager.h"
#include "FacilityDescPopup.h"
#include "ItemAsset.h"

shared_ptr<FacilityState> FactoryAsset::makeState()
{
	return MakeShared<FactoryState>();
}

void FactoryAsset::initOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const
{
}

void FactoryAsset::updateOnDescPopup(const shared_ptr<FacilityDescPopup>& popup) const
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
			auto f = font(U"資源を持ち込み、\n新たな資源を製作します");
			f.draw();
			popup->moveDrawPos(0, f.region().h + 8);
		}

		{
			auto dp = popup->getDrawPos();

			font(U"入力:").draw();
			popup->moveDrawPos(40, 0);

			for (const auto& item : m_import.getItemList())
			{
				// 画像表示
				item.first->getTexture().resized(24, 24).draw();
				popup->moveDrawPos(24, 0);

				// 名前と個数表示
				const auto f = font(item.first->getName(), U"x", item.second);
				f.draw();
				popup->moveDrawPos(f.region().w, 0);

				// 幅取り
				popup->moveDrawPos(8, 0);
			}

			popup->setDrawPos(dp.movedBy(0, 24));
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

void FactoryAsset::load(const JSONValue& json)
{
	FacilityAsset::load(json);

	m_import.load(json[U"import"]);
	m_export = g_assetManagerPtr->getAsset<ItemAsset>(json[U"export"].getString());
}
