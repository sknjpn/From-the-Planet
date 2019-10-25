#include "SystemManager.h"
#include "ViewerManager.h"
#include "PlanetViewer.h"

void Main()
{
	Window::Resize(1024, 768);
	//Window::SetFullscreen(true);
	g_systemManagerPtr = MakeUnique<SystemManager>();
	g_viewerManagerPtr->makeViewer<PlanetViewer>();

	while (System::Update())
		g_systemManagerPtr->update();
}
