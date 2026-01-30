#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GraphicsEngine.h"
#include "Renderer.h"
#include "Shader.h"
#include "Window.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"


GLfloat lastXPosition = 800.0f / 2.0f;
GLfloat lastYPosition = 600.0f / 2.0f;
GLboolean firstMouseInput = GL_TRUE;

Camera camera;

static void mouse_callback(GLFWwindow* window, double xPositionIn, double yPositionIn);


GraphicsEngine::GraphicsEngine(EngineWindow* pWindow)
{
	engineWindow = pWindow;
}

void GraphicsEngine::run()
{
	GLfloat aspectRatio = 0.0f; 
	bool isLightingOn = true;
	bool fillPolygons = true;
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	GLint bufferWidth;
	GLint bufferHeight;

	Renderer renderer;


	GLfloat verticies[] =
	{

	-100.0f, -100.0f, -100.0f,  0.0f, 0.0f,
	 100.0f, -100.0f, -100.0f,  1.0f, 0.0f,
	 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	-100.0f,  100.0f, -100.0f,  0.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,  0.0f, 0.0f,

	-100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
	 100.0f, -100.0f,  100.0f,  1.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 1.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 1.0f,
	-100.0f,  100.0f,  100.0f,  0.0f, 1.0f,
	-100.0f, -100.0f,  100.0f,  0.0f, 0.0f,

	-100.0f,  100.0f,  100.0f,  1.0f, 0.0f,
	-100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	-100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
	-100.0f,  100.0f,  100.0f,  1.0f, 0.0f,

	 100.0f,  100.0f,  100.0f,  1.0f, 0.0f,
	 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	 100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 0.0f,

	-100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,  1.0f, 1.0f,
	 100.0f, -100.0f,  100.0f,  1.0f, 0.0f,
	 100.0f, -100.0f,  100.0f,  1.0f, 0.0f,
	-100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
	-100.0f, -100.0f, -100.0f,  0.0f, 1.0f,

	-100.0f,  100.0f, -100.0f,  0.0f, 1.0f,
	 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 0.0f,
	-100.0f,  100.0f,  100.0f,  0.0f, 0.0f,
	-100.0f,  100.0f, -100.0f,  0.0f, 1.0f,

	};

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f)
	};

	GLuint indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	// Inform GLFW what version of OpenGL is being used

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (engineWindow->getWindow()  == nullptr)
	{
		std::cerr << "Error: Failure to Create Window" << std::endl;
		glfwTerminate();
		return;
	}


	glfwMakeContextCurrent(engineWindow->getWindow());
	glfwGetFramebufferSize(engineWindow->getWindow(), &bufferWidth, &bufferHeight);
	glfwSetCursorPosCallback(engineWindow->getWindow(), mouse_callback);
	glfwSwapInterval(1); // Syncs to frame rate (FPS)


	glfwSetInputMode(engineWindow->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "ERROR: Failiure to intialise GLAD" << std::endl;
	}

	Renderer::setupBlendFunctions();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();


	{
		LOG_ERRORS(glViewport(0,0, bufferWidth, bufferHeight)); // Specifies the size of the viewport

		ImGui_ImplGlfw_InitForOpenGL(engineWindow->getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 130");
		ImGui::StyleColorsDark();

		VertexArrayObject VAO1;
		VertexBufferObject VBO1(verticies, sizeof(verticies));
		VertexBufferLayout layout;
		ElementBufferObject EBO1(indices, sizeof(indices));

		layout.pushElement<float>(3);
		layout.pushElement<float>(2); 
		LOG_ERRORS(VAO1.addBuffer(VBO1, layout)); 


		// =========================== MVP Pipeline ===================================================== /

		engineWindow->setAspectRatio(bufferWidth, bufferHeight); 
		const GLfloat halfBufferWidth = static_cast<GLfloat>(bufferWidth) * 0.5f / engineWindow->getAspectRatio();
		const GLfloat halfBufferHeight = static_cast<GLfloat>(bufferHeight) * 0.5f / engineWindow->getAspectRatio();
		const GLfloat nearPlane = 0.1f;
		const GLfloat farPlane = 1000.0f;

	//  glm::mat4 projectionMatrix = glm::ortho(-halfBufferWidth, halfBufferWidth, -halfBufferHeight, halfBufferHeight, -1.0f, 1.0f);
	//	glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(bufferWidth) / static_cast<GLfloat>(bufferHeight), nearPlane, farPlane);
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	  glm::mat4 mvpMatrix =  projectionMatrix * viewMatrix * modelMatrix;

		// =============================================================================================== // 

		Shader shaderProgram("basic_default.vert", "basic_default.frag");

		Texture texture1("Logo.png"); 
		texture1.bind(0); 
		shaderProgram.setUniform1i("texture1", 0);

		LOG_ERRORS(VAO1.unbind());
		LOG_ERRORS(VBO1.unbind());
		LOG_ERRORS(EBO1.unbind());

		glm::vec3 vector(halfBufferWidth, halfBufferHeight, 0); // Sets X, Y and Z axis for movement


		while (!glfwWindowShouldClose(engineWindow->getWindow()))
		{
			renderer.clear(); 

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(bufferWidth) / static_cast<GLfloat>(bufferHeight), nearPlane, farPlane);
				glm::mat4 viewMatrix = camera.getViewMatrix();
				glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

				modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
				glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

				shaderProgram.useShader();
				shaderProgram.setUniformMatrix4f("u_MVP", mvpMatrix);

				renderer.draw(VAO1, EBO1, shaderProgram);

				ImGui::Begin("Graphics Engine");                          

				ImGui::SliderFloat("Translation.X", &vector.x, -halfBufferWidth, halfBufferWidth);     
				ImGui::SliderFloat("Translation.Y", &vector.y, -halfBufferHeight, halfBufferHeight);
				ImGui::ColorEdit3("Clear Color", (float*)&clearColor); 
				LOG_ERRORS(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w)); 


				ImGui::Checkbox("Lighting", &isLightingOn);
				ImGui::Checkbox("Fill Polygons", &fillPolygons);

				if (fillPolygons)
				{
					LOG_ERRORS(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
				}
				else 
				{
					LOG_ERRORS(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
				}
		
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			camera.processCameraInputs(0.05f, engineWindow->getWindow());
			LOG_ERRORS(engineWindow->processInput(engineWindow->getWindow()));
			LOG_ERRORS(glfwSwapBuffers(engineWindow->getWindow())); // Swap front buffer and back buffer

			glfwPollEvents(); // Handle all GLFW events
	}

		shaderProgram.stopUsingShader();
}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(engineWindow->getWindow()); // Deletes the window
	glfwTerminate(); // Terminates the program
}

void mouse_callback(GLFWwindow* window, double xPositionIn, double yPositionIn)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) // Allows the user to pan the camera when holding the right mouse button
	{
		GLfloat xPosition = static_cast<GLfloat>(xPositionIn);
		GLfloat yPosition = static_cast<GLfloat>(yPositionIn); 

		if (firstMouseInput)
		{
			lastXPosition = xPosition;
			lastYPosition = yPosition;
			firstMouseInput = GL_FALSE;
		}

		GLfloat xOffset = xPosition - lastXPosition;
		GLfloat yOffset = lastYPosition - yPosition;

		lastXPosition = xPosition;
		lastYPosition = yPosition;

		camera.processMouseMovements(xOffset, yOffset); // 
	}

}