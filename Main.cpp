#include "SystemManager.h"

void Main()
{
	g_systemManagerPtr = MakeUnique<SystemManager>();

	while (System::Update())
		g_systemManagerPtr->update();
}
