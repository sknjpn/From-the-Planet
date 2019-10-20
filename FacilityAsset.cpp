#include "FacilityAsset.h"

void FacilityAsset::load_this(const ptree& pt)
{
	for (auto j1 : pt.get_child("meshes"))
	{
		auto& m = m_meshes.emplace_back();
		for (auto j2 : j1.second.get_child("vertices"))
			m.m_vertices.emplace_back(Parse<Vec3>(Unicode::Widen(j2.second.get_value<string>())));

		m.m_color = Parse<Color>(Unicode::Widen(j1.second.get<string>("color")));
		m.m_thickness = j1.second.get<double>("thickness");
	}
}
