#pragma once

#include "Viewer.h"

class FacilitiesListViewer :
	public Viewer
{
	size_t	m_selectedIndex = -1;

public:
	FacilitiesListViewer()
	{
		setDrawRect(Scene::Size().x - 200, 40, 130, 130);
	}
	
	void	update() override;
	void	init() override;

	size_t	getSelectedIndex() const { return m_selectedIndex; }
};
