#pragma once

#include "Model.h"

class ItemAsset;

class Storage
	: public Model
	, private Array<pair<shared_ptr<ItemAsset>, int>>
{
public:
	// operator
	Storage	operator+(const Storage& s) const { return Storage(*this) += s; }
	Storage	operator-(const Storage& s) const { return Storage(*this) -= s; }
	bool operator>=(const Storage& s) const;
	bool operator<=(const Storage& s) const;
	Storage& operator+=(const Storage& s) noexcept;
	Storage& operator-=(const Storage& s) noexcept;

	// item
	const Array<pair<shared_ptr<ItemAsset>, int>>& getItemList() const { return *this; }
	void	addItem(const shared_ptr<ItemAsset>& model, int size = 1);
	void	pullItem(const shared_ptr<ItemAsset>& model, int size = 1);
	int		numItem(const shared_ptr<ItemAsset>& model) const;

	// JSON
	void	load(const JSONValue& json) override;

	void	clear() { Array<pair<shared_ptr<ItemAsset>, int>>::clear(); }
	bool	isEmpty()const { return Array<pair<shared_ptr<ItemAsset>, int>>::isEmpty(); }
};
