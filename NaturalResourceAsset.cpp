#include "NaturalResourceAsset.h"

void NaturalResourceAsset::load(const JSONValue& json)
{
	ItemAsset::load(json);
	
	m_amount = json[U"amount"].get<double>();
	m_numSite = json[U"numSite"].get<int>();
}
