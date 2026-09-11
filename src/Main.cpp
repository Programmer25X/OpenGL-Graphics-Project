
#include "GraphicsEngine.h"
#include "Shader.h"
#include "Window.h"

#include <memory>

int main()
{
	std::unique_ptr<EngineWindow> engineWindow = std::make_unique<EngineWindow>("C++ Graphics Engine");
	std::unique_ptr<GraphicsEngine> graphicsEngine = std::make_unique<GraphicsEngine>(engineWindow.get());
	graphicsEngine->run();  
	return 0;
}