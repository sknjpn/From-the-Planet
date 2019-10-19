#pragma once

#include "Model.h"

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class Asset
	: public Model
{
	string	m_name;

public:
	// Set
	void	setName(const string& name) { m_name = name; }

	// Get
	const string& getName() const { return m_name; }
	const string& getFilepath() const { return m_filepath; }

	// JSON
	void	load_this(const ptree& pt);
	 void	load(const ptree& pt) override { load_this(pt); }
};
