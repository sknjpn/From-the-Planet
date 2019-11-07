#include "FacilityAsset.h"
#include "ItemAsset.h"

String FacilityAsset::getBuildText() const
{
	String result = U"必要資源\r";

	for (const auto& m : m_material.getItemList())
	{
		result += U" ";
		result += m.first->getName();
		result += U" ";
		result += m.second;
		result += U"個\r";
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
