#include "FacilityAsset.h"
#include "ItemAsset.h"

void FacilityAsset::onDrawPopup() const
{
}

String FacilityAsset::getBuildText() const
{
	String result = U"必要資源\n";

	for (const auto& m : m_material.getItemList())
	{
		result += Format(U" ", m.first->getName(), U" ", m.second, U"個\n");
	}

	return result;
}

void FacilityAsset::load(const JSONValue& json)
{
	Asset::load(json);

	for (auto j : json[U"meshes"].arrayView())
	{
		auto& m = m_meshes.emplace_back();

		m.load(j);
	}

	m_material.load(json[U"material"]);
	m_constructinDamage = json[U"constructionDamage"].get<double>();

	m_audio = FileSystem::ParentPath(getFilepath()) + json[U"sound"].getString();
}
