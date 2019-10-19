#include "AssetManager.h"

#include "ItemAsset.h"

#include "FactoryAsset.h"
#include "QuarryAsset.h"

shared_ptr<Asset> AssetManager::makeAsset(const string& type)
{
	if (type == "ItemAsset")		return makeAsset<ItemAsset>();
	if (type == "FactoryAsset")		return makeAsset<FactoryAsset>();
	if (type == "QuarryAsset")		return makeAsset<QuarryAsset>();

	throw Error(U"存在しないタイプのAssetを生成しようとしました\r Type:" + Unicode::Widen(type));
}
