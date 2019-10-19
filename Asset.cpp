#include "Asset.h"

void Asset::setFilepath(const string& filepath)
{
	m_filepath = filepath;
}

void Asset::load_this(const ptree& pt)
{
	m_name = pt.get<string>("name");
}
