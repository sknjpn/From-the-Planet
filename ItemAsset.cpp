#include "stdafx.h"
#include "ItemAsset.h"

void ItemAsset::load_this(const ptree& pt)
{
	m_texture = Texture(U"asset/models/items/" +  Unicode::Widen(pt.get<string>("texture")));
}
