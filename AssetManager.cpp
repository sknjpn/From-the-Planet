#include "Asset.h"
#include "AssetManager.h"

unique_ptr<AssetManager>	g_assetManagerPtr;

void AssetManager::init()
{
	auto dc = FileSystem::DirectoryContents(U"asset/models", true);
	for (const auto& c : dc)
	{
		if (FileSystem::IsDirectory(c) || FileSystem::Extension(c) != U"json") continue;

		JSONReader json(c);
		if (auto type = json[U"type"].getOpt<String>())
		{
			auto a = makeAsset(type.value());
			a->setName(json[U"name"].getString());
			a->setFilepath(c);
		}
	}

	for (const auto& m : m_assets)
	{
		Logger << m->getName();

		JSONReader json(m->getFilepath());
		m->load(json);
	}
}

shared_ptr<Asset> AssetManager::getAsset(const String& name) const
{
	for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
		if ((*it)->getName() == name) return dynamic_pointer_cast<Asset>(*it);

	throw Error(U"存在しない名前のモデルを参照しました");
}
