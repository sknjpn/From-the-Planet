#include "stdafx.h"
#include "TerrainAsset.h"

void TerrainAsset::load(const JSONValue& json)
{
	m_color = json[U"color"].get<Color>();
	m_buildAvailable = json[U"buildAvailable"].get<bool>();
}
