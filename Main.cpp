#include "AssetManager.h"
#include "PlanetManager.h"
#include "SoundManager.h"
#include "TitleViewer.h"
#include "SoundMixerViewer.h"

void Main()
{
	Window::Resize(1920, 1080);
	//Window::SetFullscreen(true);
	Window::Maximize();
	Window::SetStyle(WindowStyle::Sizable);
	Window::SetTitle(U"From the Planet");

	g_assetManagerPtr = MakeUnique<AssetManager>();
	g_planetManagerPtr = MakeUnique<PlanetManager>();
	g_soundManagerPtr = MakeUnique<SoundManager>();
	g_assetManagerPtr->init();

	EasyViewer::GetRootViewer()->addChildViewer<TitleViewer>();
	EasyViewer::GetRootViewer()->addChildViewer<SoundMixerViewer>();
	EasyViewer::Run();
}
