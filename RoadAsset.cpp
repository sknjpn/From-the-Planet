#include "stdafx.h"
#include "RoadAsset.h"

void RoadAsset::load(const JSONValue& json)
{
	Asset::load(json);

	m_speed = json[U"speed"].get<double>();
	m_colorInside = json[U"colorInside"].get<Color>();
	m_colorOutside = json[U"colorOutside"].get<Color>();
}
