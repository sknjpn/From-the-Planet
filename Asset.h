#pragma once

#include "Model.h"

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class Asset
	: public Model
{
	String	m_name;
	String	m_filepath;

public:
	// Set
	void	setName(const String& name) { m_name = name; }
	void	setFilepath(const String& filepath) { m_filepath = filepath; }

	// Get
	const String& getName() const { return m_name; }
	const String& getFilepath() const { return m_filepath; }

	// JSON
	void	load(const JSONValue& json) override;
};
