#include "ItemAsset.h"

void ItemAsset::load(const JSONValue& json)
{
	Asset::load(json);

	Image image(U"asset/models/items/" + json[U"texture"].getString());
	for (auto& p : image) p = p == Palette::White ? Color(0, 0) : p;
	m_texture = Texture(image);
}
