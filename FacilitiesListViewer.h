﻿#pragma once

#include "Viewer.h"

class FacilitiesListViewer :
	public Viewer
{
	int		m_selectedIndex = -1;

public:
	FacilitiesListViewer()
	{
		setDrawRect(Scene::Size().x - 170, 40, 130, 200);
	}
	
	void	update() override;
	void	init() override;

	int		getSelectedIndex() const { return m_selectedIndex; }
	void	resetSelectedIndex() { m_selectedIndex = 0; }
};