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
	return FacilityAsset::getBuildText();
}

void FactoryAsset::load(const JSONValue& json)
{
	FacilityAsset::load(json);

	m_import.load(json[U"import"]);
	m_export = g_assetManagerPtr->getAsset<ItemAsset>(json[U"export"].getString());
}
