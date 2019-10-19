#include "Storage.h"

#include "AssetManager.h"
#include "ItemAsset.h"

bool Storage::operator>=(const Storage& s) const
{
	for (const auto& m : s)
		if (numItem(m.first) < m.second) return false;

	return true;
}

bool Storage::operator<=(const Storage& s) const
{
	for (const auto& m : *this)
		if (m.second > s.numItem(m.first)) return false;

	return true;
}

Storage& Storage::operator+=(const Storage& s) noexcept
{
	for (const auto& m : s)
		addItem(m.first, m.second);

	return *this;
}

Storage& Storage::operator-=(const Storage& s) noexcept
{
	for (const auto& m : s)
		pullItem(m.first, m.second);

	return *this;
}

void Storage::addItem(const shared_ptr<ItemAsset>& model, int size)
{
	auto it = find_if(begin(), end(), [&model](const auto& m) { return m.first == model; });

	if (it == end()) emplace_back(model, size);
	else (*it).second += size;
}

void Storage::pullItem(const shared_ptr<ItemAsset>& model, int size)
{
	auto it = find_if(begin(), end(), [&model](const auto& m) { return m.first == model; });

	if (it == end()) throw Error(U"全く存在しないItemの削除を試みました");
	else
	{
		if (((*it).second -= size) < 0) throw Error(U"存在しない量のItemの削除を試みました");
		else if ((*it).second == 0) erase(it);
	}
}

int Storage::numItem(const shared_ptr<ItemAsset>& model) const
{
	auto it = find_if(begin(), end(), [&model](const auto& m) { return m.first == model; });

	if (it == end()) return 0;
	else return (*it).second;
}

void Storage::load_this(const ptree& pt)
{
	// items
	for (auto m : pt.get_child("items"))
	{
		auto name = m.second.get<string>("name");

		const auto& model = g_assetManagerPtr->getAsset<ItemAsset>(name);

		emplace_back(model, m.second.get<int>("size"));
	}

	Model::load_this(pt);
}
