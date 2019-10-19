#include "stdafx.h"
#include "ItemModel.h"

ItemModel::ItemModel(const FilePath& filepath)
{
	JSONReader json(filepath);
	m_name = json[U"name"].get<String>();
	m_texture = Texture(FileSystem::ParentPath(filepath) + json[U"image"].get<String>());
}
