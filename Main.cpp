
#include "GraphicsEngine.h"
#include "Shader.h"
#include "Window.h"

int main()
{
	EngineWindow* engineWindow = new EngineWindow("C++ Graphics Engine");
	GraphicsEngine* graphicsEngine = new GraphicsEngine(engineWindow);
	graphicsEngine->run(); 

	delete engineWindow;
	delete graphicsEngine; 
	return 0;
}