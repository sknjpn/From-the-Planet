#include "Asset.h"

void Asset::load_this(const ptree& pt)
{
	m_name = pt.get<string>("name");
}
