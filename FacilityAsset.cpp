#include "FacilityAsset.h"

void FacilityAsset::load_this(const ptree& pt)
{
	for (auto j1 : pt.get_child("meshes"))
	{
		auto& m = m_meshes.emplace_back();
	
		m.load(j1.second);
	}
}
