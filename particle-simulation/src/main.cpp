#include "ParticleSimulationApp.h"

int main()
{
	ParticleSimulationApp app;
	app.initApp();
	app.getRoot()->startRendering();
	app.closeApp();
}