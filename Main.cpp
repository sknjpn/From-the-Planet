#include "SystemManager.h"
#include "ViewerManager.h"
#include "PlanetViewer.h"

void Main()
{
	Window::Resize(1920, 1080);
	//Window::SetFullscreen(true);
	Window::Maximize();
	Window::SetTitle(U"From the Planet");
	g_systemManagerPtr = MakeUnique<SystemManager>();
	g_viewerManagerPtr->makeViewer<PlanetViewer>();

	while (System::Update())
		g_systemManagerPtr->update();
}
