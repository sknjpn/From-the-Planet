#include "SystemManager.h"

#include "AssetManager.h"
#include "PlanetManager.h"

unique_ptr<SystemManager>	g_systemManagerPtr;

SystemManager::SystemManager()
{
	g_assetManagerPtr = MakeUnique<AssetManager>();
	g_viewerManagerPtr = MakeUnique<ViewerManager>();
	g_planetManagerPtr = MakeUnique<PlanetManager>();

	g_assetManagerPtr->init();
}

void	SystemManager::update()
{
	g_viewerManagerPtr->update();
}
