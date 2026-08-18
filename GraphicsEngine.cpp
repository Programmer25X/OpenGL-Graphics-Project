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

Camera* camera = new Camera;

static void mouse_callback(GLFWwindow* window, double xPositionIn, double yPositionIn);
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);


GraphicsEngine::GraphicsEngine(EngineWindow* pWindow)
{
	engineWindow = pWindow;
}

void GraphicsEngine::run()
{
	BasicCube* cube = new BasicCube;
	DirectionalLight* directionalLight = new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f)); 
	// PointLight* pointLight = new PointLight(glm::vec3(1.0f, 1.0f, 1.0f));

	GLfloat aspectRatio = 0.0f; 

	bool isSettingsWindowOpen = true;
	bool isAmbientLightingOn = true;
	bool isWireframeEnabled = false;
	bool isVSyncActive = false;
	bool isRenderingMultipleCubes = true;

	ImVec4 sceneLightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	GLint bufferWidth;
	GLint bufferHeight;

	Renderer* renderer = new Renderer;



	// Inform GLFW what version of OpenGL is being used

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (engineWindow->getWindow() == nullptr)
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

	// ================================ Creating ImGui Context ========================================= //

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	styleSettingsMenu();


	{
		LOG_ERRORS(glViewport(0,0, bufferWidth, bufferHeight)); // Specifies the size of the viewport

		ImGui_ImplGlfw_InitForOpenGL(engineWindow->getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 130");
 
		VertexArrayObject* VAO1 = new VertexArrayObject;
		VertexBufferObject* VBO1 = new VertexBufferObject(cube->getVerticies().data(), cube->getVerticies().size());
		VertexBufferLayout* layout1 = new VertexBufferLayout;
		ElementBufferObject* EBO1 = new ElementBufferObject(cube->getIndices().data(), cube->getIndices().size());

		VertexArrayObject* VAO2 = new VertexArrayObject;
		VertexBufferObject* VBO2 = new VertexBufferObject(directionalLight->getVerticies().data(), directionalLight->getVerticies().size());
		VertexBufferLayout* layout2 = new VertexBufferLayout;
		ElementBufferObject* EBO2 = new ElementBufferObject(directionalLight->getIndices().data(), directionalLight->getVerticies().size());

		layout1->pushElement<float>(3);
		layout1->pushElement<float>(3);
		layout1->pushElement<float>(2);

		layout2->pushElement<float>(3);

		LOG_ERRORS(VAO1->addBuffer(*VBO1, *layout1)); 
		LOG_ERRORS(VAO2->addBuffer(*VBO2, *layout2));


		engineWindow->setAspectRatio(bufferWidth, bufferHeight); 
		const GLfloat HALF_BUFFER_WIDTH = static_cast<GLfloat>(bufferWidth) * 0.5f / engineWindow->getAspectRatio();
		const GLfloat HALF_BUFFER_HEIGHT = static_cast<GLfloat>(bufferHeight) * 0.5f / engineWindow->getAspectRatio();
		



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



		Shader* directionalLightShader = new Shader("DirectionalLight.vert", "DirectionalLight.frag");
		Shader* lightSourceShader = new Shader("lightCube.vert", "lightCube.frag"); 

		Texture* texture1 = new Texture("container2.png"); 
		Texture* texture2 = new Texture("container2_specular.png");
		texture1->bind(0); 
		texture2->bind(1);

		directionalLightShader->useShader(); 
		directionalLightShader->setUniform1i("u_material.diffuse", 0);
		directionalLightShader->setUniform1i("u_material.specular", 1);


		while (!glfwWindowShouldClose(engineWindow->getWindow()))
		{
			renderer->clear(); 

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				// =========================== MVP Pipeline ================================================================ //

				glm::mat4 projectionMatrix = camera->getProjectionMatrix(static_cast<const GLfloat>(bufferWidth), static_cast<const GLfloat>(bufferHeight), camera->getNearPlane(), camera->getFarPlane());
				glm::mat4 viewMatrix = camera->getViewMatrix();
				glm::mat4 modelMatrix = glm::mat4(1.0f);

				directionalLight->setLightPosition(glm::vec3(1.0f + sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f) * 1.0f, directionalLight->getLightPosition().z));


				// =========================== Generating the main cube ====================================================== //

				directionalLightShader->useShader();
				directionalLightShader->setUniformVector3("u_light.direction", directionalLight->getLightPosition().x, directionalLight->getLightPosition().y, directionalLight->getLightPosition().z);
				directionalLightShader->setUniformVector3("u_viewPosition", camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);
				
				directionalLightShader->setUniformVector3("u_light.ambient", directionalLight->getAmbientColor().r, directionalLight->getAmbientColor().g, directionalLight->getAmbientColor().b);
				directionalLightShader->setUniformVector3("u_light.diffuse", directionalLight->getDiffuseColor().r, directionalLight->getDiffuseColor().g, directionalLight->getDiffuseColor().b);
				directionalLightShader->setUniformVector3("u_light.specular", 1.0f, 1.0f, 1.0f);

				directionalLightShader->setUniform1f("u_material.shininess", static_cast<GLfloat>(directionalLight->getShininessValue()));

				directionalLightShader->setUniformMatrix4f("u_projection", projectionMatrix);
				directionalLightShader->setUniformMatrix4f("u_view", viewMatrix);
				directionalLightShader->setUniformMatrix4f("u_model", modelMatrix); 


				if (isRenderingMultipleCubes)
				{
					for (GLuint i = 0; i < (sizeof(cubePositions)/sizeof(cubePositions[i])); i++)
					{
						modelMatrix = glm::mat4(1.0f);
						modelMatrix = glm::translate(modelMatrix, cubePositions[i] * 250.0f);
						float angle = 20.0f * i;
						modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
						modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
						directionalLightShader->setUniformMatrix4f("u_model", modelMatrix);
						renderer->draw(*VAO1, *EBO1, *directionalLightShader);
					}
				}
				else
				{
					modelMatrix = glm::mat4(1.0f);
					modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
					directionalLightShader->setUniformMatrix4f("u_model", modelMatrix);
					renderer->draw(*VAO1, *EBO1, *directionalLightShader);
				}


				// =========================== Generating the light source ===================================================== //

				lightSourceShader->useShader();
				lightSourceShader->setUniformVector3("u_light.direction", directionalLight->getLightDirection().x, directionalLight->getLightDirection().y, directionalLight->getLightDirection().z);
				lightSourceShader->setUniformMatrix4f("u_projection", projectionMatrix);
				lightSourceShader->setUniformMatrix4f("u_view", viewMatrix);
				modelMatrix = glm::mat4(1.0f);
				modelMatrix = glm::translate(modelMatrix, directionalLight->getLightPosition());
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f)); // a smaller cube
				lightSourceShader->setUniformMatrix4f("u_model", modelMatrix);
				renderer->draw(*VAO2, *EBO2, *lightSourceShader);

				// =========================== Real-time updates and ImGUI ===================================================== //

				ImGui::Begin("Settings", &isSettingsWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

				directionalLightShader->useShader();

				if (ImGui::CollapsingHeader("Lighting & Background", ImGuiTreeNodeFlags_DefaultOpen))
				{
					GLfloat sceneAmbientLightStrength = directionalLight->getAmbientStrength();
					GLfloat sceneDiffuseLightStrength = directionalLight->getDiffuseStrength();

					if (ImGui::ColorEdit3("Light Colour", (float*)&sceneLightColor))
					{
						directionalLight->setLightColor(glm::vec3(sceneLightColor.x, sceneLightColor.y, sceneLightColor.z));
						directionalLight->setAmbientColor();
						directionalLight->setDiffuseColor();
					}
					if (ImGui::SliderFloat("Ambient Light Intensity", (float*)&sceneAmbientLightStrength, 0.0f, 1.0f))
					{
						directionalLight->setAmbientStrength(sceneAmbientLightStrength);
						directionalLight->setAmbientColor();
					}
					if (ImGui::SliderFloat("Diffuse Light Intensity", (float*)&sceneDiffuseLightStrength, 0.0f, 2.0f))
					{
						directionalLight->setDiffuseStrength(sceneDiffuseLightStrength);
						directionalLight->setDiffuseColor();
					}

					if(ImGui::ColorEdit3("Background Color", (float*)&clearColor));
					{
						LOG_ERRORS(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
					}
				}

				if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
				{
					float fovTemp = camera->getFOV();
					if (ImGui::SliderFloat("Field of View (FOV)", &fovTemp, 1.0f, 45.0f));
					{
						camera->setFOV(fovTemp);
					}
					float nearPlaneTemp = camera->getNearPlane();
					if (ImGui::SliderFloat("Near Plane", &nearPlaneTemp, 0.01f, 100.0f));
					{
						camera->setNearPlane(nearPlaneTemp);
					}
					float farPlaneTemp = camera->getFarPlane();
					if (ImGui::SliderFloat("Far Plane", &farPlaneTemp, 100.0f, 5000.0f));
					{
						camera->setFarPlane(farPlaneTemp);
					}
				}

				if (ImGui::CollapsingHeader("Boxing", ImGuiTreeNodeFlags_DefaultOpen))
				{
					GLuint shininessValue = directionalLight->getShininessValue();
					if (ImGui::SliderInt("Shininess Value", (GLint*)&shininessValue, 1, 256))
					{
						directionalLight->setShininessValue(shininessValue);
					}
				}

				if (ImGui::CollapsingHeader("Rendering", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (ImGui::Checkbox("Wireframe", &isWireframeEnabled))
					{
						if (!isWireframeEnabled)
						{
							LOG_ERRORS(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
						}
						else
						{
							LOG_ERRORS(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
						}
					}
					ImGui::Checkbox("VSync", &isVSyncActive);
					ImGui::Checkbox("Multiple Cubes", &isRenderingMultipleCubes);
				}

				if (ImGui::CollapsingHeader("Other Information", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Text("Application's Average FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				}
		
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			camera->processCameraInputs(0.05f, engineWindow->getWindow());
			LOG_ERRORS(engineWindow->processInput(engineWindow->getWindow()));
			LOG_ERRORS(glfwSwapBuffers(engineWindow->getWindow())); // Swap front buffer and back buffer

			glfwPollEvents(); // Handle all GLFW events
	}

		directionalLightShader->stopUsingShader();
		lightSourceShader->stopUsingShader();

		// =========================== Memory Management - Deleting instantiated objects =====================================================

		delete(texture1);
		delete(texture2);

		delete(directionalLightShader);
		delete(lightSourceShader);
		delete(renderer);

		delete(cube);
		delete(directionalLight);

		delete(VAO1);
		delete(VBO1);
		delete(EBO1);
		delete(layout1);

		delete(VAO2);
		delete(VBO2);
		delete(EBO2);
		delete(layout2);

		delete(camera); 
}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(engineWindow->getWindow()); // Deletes the window
	glfwTerminate(); // Terminates the program
}

void GraphicsEngine::styleSettingsMenu()
{
	ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = ImVec4(80.0f / 255.0f, 235.0f / 255.0f, 114.0f / 255.0f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_Header] = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(80.0f / 255.0f, 235.0f / 255.0f, 114.0f / 255.0f, 1.0f); 
}

/// <summary>
/// Function for panning the camera 
/// </summary>
/// <param name="window"></param>
/// <param name="xPositionIn"></param>
/// <param name="yPositionIn"></param>
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

		camera->processMouseMovements(xOffset, yOffset); 
	}
}

/// <summary>
/// Function for zooming the camera in and out
/// </summary>
/// <param name="window"></param>
/// <param name="xOffset"></param>
/// <param name="yOffset"></param>
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera->processMouseScroll(static_cast<GLfloat>(yOffset)); 
}
