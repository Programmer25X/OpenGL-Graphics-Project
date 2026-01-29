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

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
GLfloat lastXPosition = 400.0f;
GLfloat lastYPosition = 300.0f;
GLfloat yaw = -90.0f;
float pitch = 0.0f;

GraphicsEngine::GraphicsEngine(EngineWindow* pWindow)
{
	engineWindow = pWindow;
}

GraphicsEngine::~GraphicsEngine()
{

}


void GraphicsEngine::run()
{
	GLfloat aspectRatio = 0.0f; 
	bool isLightingOn = true;
	bool fillPolygons = true;
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
	// glfwSetCursorPosCallback(engineWindow->getWindow(), mouse_callback(*engineWindow->getWindow(), lastXPosition, lastYPosition));
	glfwSwapInterval(1); // Syncs to frame rate (FPS)


	gladLoadGL(); // Loads GLAD to configure OpenGL

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "ERROR: Failiure to intialise GLAD" << std::endl;
	}

	Renderer::setupBlendFunctions();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();


	{
		GLint bufferWidth;
		GLint bufferHeight;
		

		glfwGetFramebufferSize(engineWindow->getWindow(), &bufferWidth, &bufferHeight);

		LOG_ERRORS(glViewport(0,0, bufferWidth, bufferHeight)); // Specifies the size of the viewport

		Renderer renderer; 
		Camera camera(engineWindow->getWindow()); 

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
		// Texture texture2("awesomeface.png");

		texture1.bind(0); 
	   // texture2.bind(1);

		shaderProgram.setUniform1i("texture1", 0);
		// shaderProgram.setUniformMatrix4f("u_MVP", mvpMatrix); 
		// shaderProgram.setUniform1i("texture2", 1);


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
				glm::mat4 viewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraFront(), camera.getCameraUp());
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
	GLfloat xPosition = xPositionIn;
	GLfloat yPosition = yPositionIn; 

	GLfloat xOffset = static_cast<GLfloat>(xPosition - lastXPosition);
	GLfloat yOffset = static_cast<GLfloat>(lastXPosition - yPosition);

	GLfloat mouseSensitity = 0.1f;
	lastXPosition = static_cast<GLfloat>(xPosition);
	lastYPosition = static_cast<GLfloat>(yPosition);

	xOffset *= mouseSensitity;
	yOffset *= mouseSensitity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 direction(0.0f, 0.0f, 0.0f);
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = cos(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

