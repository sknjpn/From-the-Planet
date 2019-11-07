#include "FactoryAsset.h"
#include "FactoryState.h"
#include "AssetManager.h"
#include "ItemAsset.h"

shared_ptr<FacilityState> FactoryAsset::makeState()
{
	return MakeShared<FactoryState>();
}

String FactoryAsset::getBuildText() const
{
	String result;

	for (const auto& i : m_import.getItemList())
		result += Format(U" ", i.first->getName(), U"を", i.second, U"個\n");

	result += Format(U"を使って", m_export->getName(), U"を作る工場\n");

	result += FacilityAsset::getBuildText();

	return result;
}

void FactoryAsset::load(const JSONValue& json)
{
	FacilityAsset::load(json);

	m_import.load(json[U"import"]);
	m_export = g_assetManagerPtr->getAsset<ItemAsset>(json[U"export"].getString());
}
