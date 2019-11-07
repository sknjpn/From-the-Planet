#include "AssetManager.h"

#include "ItemAsset.h"

#include "FactoryAsset.h"
#include "QuarryAsset.h"
#include "LaunchSiteAsset.h"
#include "NaturalResourceAsset.h"

shared_ptr<Asset> AssetManager::makeAsset(const String& type)
{
	if (type == U"ItemAsset")		return makeAsset<ItemAsset>();
	if (type == U"FactoryAsset")		return makeAsset<FactoryAsset>();
	if (type == U"QuarryAsset")		return makeAsset<QuarryAsset>();
	if (type == U"LaunchSiteAsset")		return makeAsset<LaunchSiteAsset>();
	if (type == U"NaturalResourceAsset")		return makeAsset<NaturalResourceAsset>();

	throw Error(U"存在しないタイプのAssetを生成しようとしました\r Type:" + type);
}
