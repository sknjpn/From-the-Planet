#include "AssetManager.h"
#include "PlanetManager.h"
#include "TitleViewer.h"

void Main()
{
	Window::Resize(1920, 1080);
	//Window::SetFullscreen(true);
	Window::Maximize();
	Window::SetStyle(WindowStyle::Sizable);
	Window::SetTitle(U"From the Planet");

	g_assetManagerPtr = MakeUnique<AssetManager>();
	g_planetManagerPtr = MakeUnique<PlanetManager>();
	g_assetManagerPtr->init();

	Viewer::GetRootViewer()->addChildViewer<TitleViewer>();
	Viewer::Run();
}
