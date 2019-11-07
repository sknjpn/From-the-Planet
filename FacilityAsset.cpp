#include "FacilityAsset.h"

void FacilityAsset::load(const JSONValue& json)
{
	Asset::load(json);

	for (auto j : json[U"meshes"].arrayView())
	{
		auto& m = m_meshes.emplace_back();

		m.load(j);
	}

	m_material.load(json[U"material"]);

	m_audio = FileSystem::ParentPath(getFilepath()) + json[U"sound"].getString();
}
