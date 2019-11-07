#include "Asset.h"

void Asset::load(const JSONValue& json)
{
	Model::load(json);

	m_name = json[U"name"].get<String>();
}
