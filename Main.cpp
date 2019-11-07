#include "PlanetViewer.h"
#include "AssetManager.h"
#include "PlanetManager.h"

void Main()
{
	Window::Resize(1920, 1080);
	//Window::SetFullscreen(true);
	Window::Maximize();
	Window::SetTitle(U"From the Planet");

	g_assetManagerPtr = MakeUnique<AssetManager>();
	g_planetManagerPtr = MakeUnique<PlanetManager>();
	g_assetManagerPtr->init();

	Viewer::GetRootViewer()->addChildViewer<PlanetViewer>();
	Viewer::Run();
}
