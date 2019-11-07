#include "QuarryAsset.h"
#include "QuarryState.h"
#include "AssetManager.h"
#include "ItemAsset.h"

shared_ptr<FacilityState> QuarryAsset::makeState()
{
	return MakeShared<QuarryState>();
}

String QuarryAsset::getBuildText() const
{
	return FacilityAsset::getBuildText();
}

void QuarryAsset::load(const JSONValue& json)
{
	FacilityAsset::load(json);

	m_export = g_assetManagerPtr->getAsset<ItemAsset>(json[U"export"].getString());
}
