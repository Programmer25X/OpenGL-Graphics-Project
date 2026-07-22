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
#include "Objects.h"
#include "Lighting.h"

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
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);


GraphicsEngine::GraphicsEngine(EngineWindow* pWindow)
{
	engineWindow = pWindow;
}

void GraphicsEngine::run()
{
	LogoCube cube;
	Lighting lightCube(glm::vec3(1.0f, 1.0f, 1.0f)); 	

	GLfloat aspectRatio = 0.0f; 
	bool isAmbientLightingOn = true;
	bool fillPolygons = true;
	bool renderMultipleCubes = true;
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	ImVec4 sceneLightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	GLfloat sceneAmbientLightStrength = lightCube.getAmbientStrength();
	GLfloat sceneDiffuseLightStrength = lightCube.getDiffuseStrength(); 
	GLfloat shininessValue = lightCube.getShininessValue(); 


	GLint bufferWidth;
	GLint bufferHeight;

	Renderer renderer;



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
	glfwSetScrollCallback(engineWindow->getWindow(), scroll_callback);
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
		VertexBufferObject VBO1(cube.getVerticies().data(), cube.getVerticies().size());
		VertexBufferLayout layout1;
		ElementBufferObject EBO1(cube.getIndices().data(), cube.getIndices().size());

		VertexArrayObject VAO2;
		VertexBufferObject VBO2(lightCube.getVerticies().data(), lightCube.getVerticies().size());
		VertexBufferLayout layout2;
		ElementBufferObject EBO2(lightCube.getIndices().data(), lightCube.getVerticies().size());

		layout1.pushElement<float>(3);
		layout1.pushElement<float>(3);
		layout1.pushElement<float>(2);

		layout2.pushElement<float>(3);

		LOG_ERRORS(VAO1.addBuffer(VBO1, layout1)); 
		LOG_ERRORS(VAO2.addBuffer(VBO2, layout2));


		engineWindow->setAspectRatio(bufferWidth, bufferHeight); 
		const GLfloat HALF_BUFFER_WIDTH = static_cast<GLfloat>(bufferWidth) * 0.5f / engineWindow->getAspectRatio();
		const GLfloat HALF_BUFFER_HEIGHT = static_cast<GLfloat>(bufferHeight) * 0.5f / engineWindow->getAspectRatio();
		const GLfloat NEAR_PLANE = 0.1f;
		const GLfloat FAR_PLANE = 10000.0f;



	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f) };



		Shader cubeShader("basic_default.vert", "basic_default.frag");
		Shader lightShader("lightCube.vert", "lightCube.frag"); 

		Texture texture1("container2.png"); 
		Texture texture2("container2_specular.png");
		texture1.bind(0); 
		texture2.bind(1);
		cubeShader.useShader(); 
		cubeShader.setUniform1i("u_material.diffuse", 0);
		cubeShader.setUniform1i("u_material.specular", 1);


		while (!glfwWindowShouldClose(engineWindow->getWindow()))
		{
			renderer.clear(); 

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				// =========================== MVP Pipeline ================================================================ //

				glm::mat4 projectionMatrix = camera.getProjectionMatrix(static_cast<const GLfloat>(bufferWidth), static_cast<const GLfloat>(bufferHeight));
				glm::mat4 viewMatrix = camera.getViewMatrix();
				glm::mat4 modelMatrix = glm::mat4(1.0f);

				lightCube.setLightPosition(glm::vec3(1.0f + sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f) * 1.0f, lightCube.getLightPosition().z));


				// =========================== Generating the main cube ===================================================== //

				cubeShader.useShader();
				cubeShader.setUniformVector3("u_light.direction", lightCube.getLightPosition().x, lightCube.getLightPosition().y, lightCube.getLightPosition().z);
				cubeShader.setUniformVector3("u_viewPosition", camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
				
				cubeShader.setUniformVector3("u_light.ambient", lightCube.getAmbientColor().r, lightCube.getAmbientColor().g, lightCube.getAmbientColor().b);
				cubeShader.setUniformVector3("u_light.diffuse", lightCube.getDiffuseColor().r, lightCube.getDiffuseColor().g, lightCube.getDiffuseColor().b);
				cubeShader.setUniformVector3("u_light.specular", 1.0f, 1.0f, 1.0f);

				cubeShader.setUniform1f("u_material.shininess", static_cast<GLfloat>(lightCube.getShininessValue()));

				cubeShader.setUniformMatrix4f("u_projection", projectionMatrix);
				cubeShader.setUniformMatrix4f("u_view", viewMatrix);
				cubeShader.setUniformMatrix4f("u_model", modelMatrix);


				if (renderMultipleCubes)
				{
					for (unsigned int i = 0; i < 10; i++)
					{
						modelMatrix = glm::mat4(1.0f);
						modelMatrix = glm::translate(modelMatrix, cubePositions[i] * 250.0f);
						float angle = 20.0f * i;
						modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
						modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
						cubeShader.setUniformMatrix4f("u_model", modelMatrix);

						renderer.draw(VAO1, EBO1, cubeShader);
					}
				}
				else
				{
					modelMatrix = glm::mat4(1.0f);
					modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
					cubeShader.setUniformMatrix4f("u_model", modelMatrix);
 
					renderer.draw(VAO1, EBO1, cubeShader);
				}


				// =========================== Generating the light source ===================================================== //

				lightShader.useShader();

				lightShader.setUniformMatrix4f("u_projection", projectionMatrix);
				lightShader.setUniformMatrix4f("u_view", viewMatrix);
				modelMatrix = glm::mat4(1.0f);
				modelMatrix = glm::translate(modelMatrix, lightCube.getLightPosition());
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f)); // a smaller cube

				lightShader.setUniformMatrix4f("u_model", modelMatrix);

				renderer.draw(VAO2, EBO2, lightShader);

				// =========================== Real-time updates and ImGUI ===================================================== //

				ImGui::Begin("Graphics Engine");

				{	
					cubeShader.useShader();

					ImGui::ColorEdit3("Light Colour", (float*)&sceneLightColor);
					ImGui::SliderFloat("Ambient Light Intensity", (float*)&sceneAmbientLightStrength, 0.0f, 1.0f);
					ImGui::SliderFloat("Diffuse Light Intensity", (float*)&sceneDiffuseLightStrength, 0.0f, 2.0f);
					ImGui::SliderFloat("Shininess Value", (float*)&shininessValue, 0.01f, 32.0f);

					lightCube.setLightColor(glm::vec3(sceneLightColor.x, sceneLightColor.y, sceneLightColor.z));

					lightCube.setAmbientStrength(sceneAmbientLightStrength);
					lightCube.setAmbientColor();
					cubeShader.setUniformVector3("u_light.ambient", lightCube.getAmbientColor().r, lightCube.getAmbientColor().g, lightCube.getAmbientColor().b);

					lightCube.setDiffuseStrength(sceneDiffuseLightStrength);
					lightCube.setDiffuseColor();
					cubeShader.setUniformVector3("u_light.diffuse", lightCube.getDiffuseColor().r, lightCube.getDiffuseColor().g, lightCube.getDiffuseColor().b);

					lightCube.setShininessValue(shininessValue);
					cubeShader.setUniform1f("u_material.shininess", lightCube.getShininessValue()); 
				}

				ImGui::ColorEdit3("Clear Color", (float*)&clearColor); 
				LOG_ERRORS(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w)); 

				ImGui::Checkbox("Fill Polygons", &fillPolygons);
				ImGui::Checkbox("Render Multiple Cubes", &renderMultipleCubes); 

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

		cubeShader.stopUsingShader();
		lightShader.stopUsingShader();
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

		camera.processMouseMovements(xOffset, yOffset); 
	}
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(static_cast<GLfloat>(yOffset)); 
}
