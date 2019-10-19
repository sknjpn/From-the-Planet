#include "SystemManager.h"
#include "ViewerManager.h"
#include "PlanetViewer.h"

void Main()
{
	g_systemManagerPtr = MakeUnique<SystemManager>();
	g_viewerManagerPtr->makeViewer<PlanetViewer>();

	while (System::Update())
		g_systemManagerPtr->update();
}
