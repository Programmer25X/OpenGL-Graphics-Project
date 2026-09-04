#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>

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


struct RenderingSettings
{
	bool isSettingsWindowOpen = true;
	bool isWireframeEnabled = false;
	bool isVSyncActive = false;
	bool isRenderingMultipleCubes = true;
};


struct DirectionalLightSettings
{
	bool enabled = true;
	GLfloat ambientIntensity = 0.0f;
	GLfloat diffuseIntensity = 0.0f;
	GLfloat specularIntensity = 0.0f;
};

struct PointLightSettings
{
	bool enabled = true;
	GLfloat ambientIntensity = 0.0f;
	GLfloat diffuseIntensity = 0.0f;
	GLfloat specularIntensity = 0.0f;
	GLfloat attenuationConstant = 0.0f;
	GLfloat attenuationLinear = 0.0f;
	GLfloat attenuationQuadratic = 0.0f;
};


struct SpotlightSettings
{
	bool enabled = true;
	GLfloat ambientIntensity = 0.0f;
	GLfloat diffuseIntensity = 0.0f;
	GLfloat specularIntensity = 0.0f;
	GLfloat attenuationConstant = 0.0f;
	GLfloat attenuationLinear = 0.0f;
	GLfloat attenuationQuadratic = 0.0f;
	GLfloat innerCutOff = 0.0f;
	GLfloat outerCutOff = 0.0f;
};

struct ColourSettings
{
	ImVec4 directionalLightColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pointLightColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 spotlightColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 clearColour = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
};

struct CameraSettings
{
	GLfloat farPlane = 0.0f;
	GLfloat nearPlane = 0.0f;
	GLfloat fieldOfView = 0.0f;
	GLfloat speed = 0.0f; 
};

GLfloat lastXPosition = 800.0f / 2.0f;
GLfloat lastYPosition = 600.0f / 2.0f;
GLboolean firstMouseInput = GL_TRUE;

BasicCube* cube = nullptr;

DirectionalLight* directionalLight = nullptr;
SpotLight* spotlight = nullptr;
PointLight* pointLights[4] = {};

Renderer* renderer = nullptr;

VertexArrayObject* VAO1 = nullptr;
VertexBufferObject* VBO1 = nullptr;
VertexBufferLayout* layout1 = nullptr;
ElementBufferObject* EBO1 = nullptr;

VertexArrayObject* VAO2 = nullptr;
VertexBufferObject* VBO2 = nullptr;
VertexBufferLayout* layout2 = nullptr;
ElementBufferObject* EBO2 = nullptr;

Shader* lightingShader = nullptr;
Shader* lightSourceShader = nullptr;

Texture* texture1 = nullptr;
Texture* texture2 = nullptr;

Camera* camera = nullptr;

RenderingSettings renderingSettings = {};
DirectionalLightSettings directionalLightSettings = {};
SpotlightSettings spotlightSettings = {};
PointLightSettings pointLightSettings = {}; 
ColourSettings colourSettings = {};
CameraSettings cameraSettings = {}; 

static void mouse_callback(GLFWwindow* window, double xPositionIn, double yPositionIn);
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);



GraphicsEngine::GraphicsEngine(EngineWindow* pWindow)
{
	engineWindow = pWindow;
}

GraphicsEngine::~GraphicsEngine()
{
	lightingShader->stopUsingShader();
	lightSourceShader->stopUsingShader();

	delete(texture1);
	delete(texture2);

	delete(lightingShader);
	delete(lightSourceShader);
	delete(renderer);

	delete(cube);
	delete(directionalLight);

	for (PointLight* pointLight : pointLights)
	{
		delete pointLight;
	}

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

void GraphicsEngine::run()
{
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

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f) };

	GLfloat aspectRatio = 0.0f;
	GLint bufferWidth = 0.0f;
	GLint bufferHeight = 0.0f;


// ================================ Initalising GLAD and GLFW ========================================= //

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


	// ================================ Creating ImGui Context ========================================= //

	Renderer::setupBlendFunctions();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	styleSettingsMenu();

	LOG_ERRORS(glViewport(0,0, bufferWidth, bufferHeight)); // Specifies the size of the viewport

	ImGui_ImplGlfw_InitForOpenGL(engineWindow->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");

	{
		createEntities();

		engineWindow->setAspectRatio(bufferWidth, bufferHeight); 

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


				// =========================== Setting the Uniforms for the Lights and Boxes ====================================================== //

				lightingShader->useShader();

				// Directional Light
				if (directionalLightSettings.enabled)
				{
					lightingShader->setUniformVector3("u_directionalLight.direction", directionalLight->getLightPosition().x, directionalLight->getLightPosition().y, directionalLight->getLightPosition().z);
					lightingShader->setUniformVector3("u_viewPosition", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
					lightingShader->setUniformVector3("u_directionalLight.ambient", directionalLight->getAmbientColour().r, directionalLight->getAmbientColour().g, directionalLight->getAmbientColour().b);
					lightingShader->setUniformVector3("u_directionalLight.diffuse", directionalLight->getDiffuseColour().r, directionalLight->getDiffuseColour().g, directionalLight->getDiffuseColour().b);
					lightingShader->setUniformVector3("u_directionalLight.specular", directionalLight->getSpecularColour().r, directionalLight->getSpecularColour().g, directionalLight->getSpecularColour().b);
				}
				
				
				// Point Lights
				if (pointLightSettings.enabled)
				{
					for (GLint i = 0; i < (sizeof(pointLightPositions) / sizeof(pointLightPositions[i])); i++)
					{
						std::string pointLightUniform = std::string("u_pointLight[") + std::to_string(i) + std::string("].");
						lightingShader->setUniformVector3(pointLightUniform + "position", pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
						lightingShader->setUniformVector3(pointLightUniform + "ambient", pointLights[i]->getAmbientColour().r, pointLights[i]->getAmbientColour().g, pointLights[i]->getAmbientColour().b);
						lightingShader->setUniformVector3(pointLightUniform + "diffuse", pointLights[i]->getDiffuseColour().r, pointLights[i]->getDiffuseColour().g, pointLights[i]->getDiffuseColour().b);
						lightingShader->setUniformVector3(pointLightUniform + "specular", pointLights[i]->getSpecularColour().r, pointLights[i]->getSpecularColour().g, pointLights[i]->getSpecularColour().b);
						lightingShader->setUniform1f(pointLightUniform + "constant", pointLights[i]->getConstant());
						lightingShader->setUniform1f(pointLightUniform + "linear", pointLights[i]->getLinear());
						lightingShader->setUniform1f(pointLightUniform + "quadratic", pointLights[i]->getQuadratic());
					}
				}

				// Spotlight 
				if (spotlightSettings.enabled)
				{
					lightingShader->setUniformVector3("u_spotLight.position", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
					lightingShader->setUniformVector3("u_spotLight.direction", camera->getFront().x, camera->getFront().y, camera->getFront().z);
					lightingShader->setUniform1f("u_spotLight.innerCutOff", spotlight->getInnerCutOff());
					lightingShader->setUniform1f("u_spotLight.outerCutOff", spotlight->getOuterCutOff());
					lightingShader->setUniformVector3("u_spotLight.ambient", spotlight->getAmbientColour().r, spotlight->getAmbientColour().g, spotlight->getAmbientColour().b);
					lightingShader->setUniformVector3("u_spotLight.diffuse", spotlight->getDiffuseColour().r, spotlight->getDiffuseColour().g, spotlight->getDiffuseColour().b);
					lightingShader->setUniformVector3("u_spotLight.specular", spotlight->getSpecularColour().r, spotlight->getSpecularColour().g, spotlight->getSpecularColour().b);
					lightingShader->setUniform1f("u_spotLight.constant", spotlight->getConstant());
					lightingShader->setUniform1f("u_spotLight.linear", spotlight->getLinear());
					lightingShader->setUniform1f("u_spotLight.quadratic", spotlight->getQuadratic());
				}

				// Boxes
				lightingShader->setUniform1f("u_material.shininess", static_cast<GLfloat>(directionalLight->getShininessValue()));
				lightingShader->setUniformMatrix4f("u_projection", projectionMatrix);
				lightingShader->setUniformMatrix4f("u_view", viewMatrix);
				lightingShader->setUniformMatrix4f("u_model", modelMatrix); 


				// =========================== Drawing the Boxes ===================================================== //

				if (renderingSettings.isRenderingMultipleCubes)
				{
					for (GLuint i = 0; i < (sizeof(cubePositions)/sizeof(cubePositions[0])); i++)
					{
						modelMatrix = glm::mat4(1.0f);
						modelMatrix = glm::translate(modelMatrix, cubePositions[i] * 200.0f);
						float angle = 20.0f * i;
						modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
						modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
						lightingShader->setUniformMatrix4f("u_model", modelMatrix);
						renderer->draw(*VAO1, *EBO1, *lightingShader);
					}
				}
				else
				{
					modelMatrix = glm::mat4(1.0f);
					modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
					lightingShader->setUniformMatrix4f("u_model", modelMatrix);
					renderer->draw(*VAO1, *EBO1, *lightingShader);
				}

				// =========================== Drawing the Light Sources ===================================================== //
				
				if (pointLightSettings.enabled)
				{
					lightSourceShader->useShader();
					lightSourceShader->setUniformMatrix4f("u_projection", projectionMatrix);
					lightSourceShader->setUniformMatrix4f("u_view", viewMatrix);

					for (GLuint i = 0; i < (sizeof(pointLights) / sizeof(pointLights[0])); i++)
					{
						modelMatrix = glm::mat4(1.0f);
						modelMatrix = glm::translate(modelMatrix, pointLightPositions[i] * 250.0f);
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
						lightSourceShader->setUniformMatrix4f("u_model", modelMatrix);
						lightSourceShader->setUniformVector3("u_lightSourceColour", pointLights[i]->getLightColour().r, pointLights[i]->getLightColour().g, pointLights[i]->getLightColour().b);
						renderer->draw(*VAO2, *EBO2, *lightSourceShader);
					}
				}


				// =========================== Real-Time Updates and ImGUI ===================================================== //

				ImGui::Begin("Settings", &renderingSettings.isSettingsWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

				lightingShader->useShader();

				if (ImGui::CollapsingHeader("Directional Lighting & Background"))
				{
					directionalLightSettings.ambientIntensity = directionalLight->getAmbientIntensity();
					directionalLightSettings.diffuseIntensity = directionalLight->getDiffuseIntensity();
					directionalLightSettings.specularIntensity = directionalLight->getSpecularIntensity();

					ImGui::SeparatorText("Colour");

					if (ImGui::ColorEdit3("Light Colour##DirectionalLight", (float*)&colourSettings.directionalLightColour), ImGuiColorEditFlags_DisplayRGB)
					{
						directionalLight->setLightColour(glm::vec3(colourSettings.directionalLightColour.x, colourSettings.directionalLightColour.y, colourSettings.directionalLightColour.z));
						directionalLight->setAmbientColour();
						directionalLight->setDiffuseColour();
						directionalLight->setSpecularColour();
					}

					if (ImGui::ColorEdit3("Background Colour", (float*)&colourSettings.clearColour, ImGuiColorEditFlags_DisplayRGB));
					{
						LOG_ERRORS(glClearColor(colourSettings.clearColour.x, colourSettings.clearColour.y, colourSettings.clearColour.z, colourSettings.clearColour.w));
					}

					ImGui::SeparatorText("Phong Lighting");

					if (ImGui::SliderFloat("Ambient Intensity##DirectionalLight", (float*)&directionalLightSettings.ambientIntensity, 0.013f, 1.0f, "%.3f"))
					{
						directionalLight->setAmbientIntensity(directionalLightSettings.ambientIntensity);
						directionalLight->setAmbientColour();
					}

					if (ImGui::SliderFloat("Diffuse Intensity##DirectionalLight", (float*)&directionalLightSettings.diffuseIntensity, 0.0f, 2.0f, "%.3f"))
					{
						directionalLight->setDiffuseIntensity(directionalLightSettings.diffuseIntensity);
						directionalLight->setDiffuseColour();
					}

					if (ImGui::SliderFloat("Specular Intensity##DirectionalLight", (float*)&directionalLightSettings.specularIntensity, 0.0f, 1.0f, "%.3f"))
					{
						directionalLight->setSpecularIntensity(directionalLightSettings.specularIntensity);
						directionalLight->setSpecularColour();
					}

					ImGui::SeparatorText("Other");

					if (ImGui::Checkbox("Enabled##DirectionalLight", &directionalLightSettings.enabled))
					{
						lightingShader->setUniformBoolean("u_isDirectionalLightEnabled", directionalLightSettings.enabled);
					}

					ImGui::NewLine();
				}

				if (ImGui::CollapsingHeader("Point Lighting"))
				{
					pointLightSettings.ambientIntensity= pointLights[0]->getAmbientIntensity();
					pointLightSettings.diffuseIntensity = pointLights[0]->getDiffuseIntensity();
					pointLightSettings.specularIntensity = pointLights[0]->getSpecularIntensity();
					pointLightSettings.attenuationConstant = pointLights[0]->getConstant();
					pointLightSettings.attenuationLinear = pointLights[0]->getLinear();
					pointLightSettings.attenuationQuadratic = pointLights[0]->getQuadratic();

					colourSettings.pointLightColour = ImVec4(pointLights[0]->getLightColour().r, pointLights[0]->getLightColour().g, pointLights[0]->getLightColour().b, 0.0f);

					ImGui::SeparatorText("Colour");

					if (ImGui::ColorEdit3("Light Colour##PointLight", (float*)&colourSettings.pointLightColour, ImGuiColorEditFlags_DisplayRGB))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setLightColour(glm::vec3(colourSettings.pointLightColour.x, colourSettings.pointLightColour.y, colourSettings.pointLightColour.z));
							pointLight->setDiffuseColour();
							pointLight->setAmbientColour();
							pointLight->setSpecularColour();
						}
					}

					ImGui::SeparatorText("Phong Lighting");

					if (ImGui::SliderFloat("Ambient Intensity##PointLight", (float*)&pointLightSettings.ambientIntensity, 0.0f, 2.0f, "%.3f"))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setAmbientIntensity(pointLightSettings.ambientIntensity);
							pointLight->setAmbientColour();
						}
					}
						
					if (ImGui::SliderFloat("Diffuse Intensity##PointLight", (float*)&pointLightSettings.diffuseIntensity, 0.0f, 2.0f, "%.3f"))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setDiffuseIntensity(pointLightSettings.diffuseIntensity);
							pointLight->setDiffuseColour();
						}
					}

					if (ImGui::SliderFloat("Specular Intensity##PointLight", (float*)&pointLightSettings.specularIntensity, 0.0f, 1.0f, "%.3f"))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setSpecularIntensity(pointLightSettings.specularIntensity);
							pointLight->setSpecularColour();
						}
					}

					ImGui::SeparatorText("Attenuation");

					if (ImGui::SliderFloat("Attenuation Constant##PointLight", (float*)&pointLightSettings.attenuationConstant, 0.0f, 1.0f, "%.3f"))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setConstant(pointLightSettings.attenuationConstant);
						}
					}

					if (ImGui::SliderFloat("Attenuation Linear##PointLight", (float*)&pointLightSettings.attenuationLinear, 0.0014f, 0.7f, "%.4f", ImGuiSliderFlags_Logarithmic))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setLinear(pointLightSettings.attenuationLinear);
						}
					}

					if (ImGui::SliderFloat("Attenuation Quadratic##PointLight", (float*)&pointLightSettings.attenuationQuadratic, 0.000007f, 1.8f, "%.6f", ImGuiSliderFlags_Logarithmic))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setQuadratic(pointLightSettings.attenuationQuadratic);
						}
					}

					ImGui::SeparatorText("Other");

					if (ImGui::Checkbox("Enabled##PointLight", &pointLightSettings.enabled))
					{
						lightingShader->setUniformBoolean("u_isPointlLightEnabled", pointLightSettings.enabled);
					}

					ImGui::NewLine();
				}

				if (ImGui::CollapsingHeader("Spotlight"))
				{
					spotlightSettings.ambientIntensity = spotlight->getAmbientIntensity();
					spotlightSettings.diffuseIntensity = spotlight->getDiffuseIntensity();
					spotlightSettings.attenuationQuadratic = spotlight->getSpecularIntensity();
					spotlightSettings.attenuationConstant = spotlight->getConstant();
					spotlightSettings.attenuationLinear = spotlight->getLinear();
					spotlightSettings.attenuationQuadratic = spotlight->getQuadratic();
					spotlightSettings.innerCutOff = glm::degrees(glm::acos(spotlight->getInnerCutOff()));
					spotlightSettings.outerCutOff = glm::degrees(glm::acos(spotlight->getOuterCutOff()));

					ImGui::SeparatorText("Colour");

					if (ImGui::ColorEdit3("Light Colour##Spotlight", (float*)&colourSettings.spotlightColour, ImGuiColorEditFlags_DisplayRGB))
					{
						spotlight->setLightColour(glm::vec3(colourSettings.spotlightColour.x, colourSettings.spotlightColour.y, colourSettings.spotlightColour.z));
						spotlight->setDiffuseColour();
						spotlight->setAmbientColour();
						spotlight->setSpecularColour();
					}

					ImGui::SeparatorText("Phong Lighting");

					if (ImGui::SliderFloat("Ambient Intensity##Spotlight", (float*)&spotlightSettings.ambientIntensity, 0.0f, 2.0f, "%.3f"))
					{
						spotlight->setAmbientIntensity(spotlightSettings.ambientIntensity);
						spotlight->setAmbientColour();
					}

					if (ImGui::SliderFloat("Diffuse Intensity##Spotlight", (float*)&spotlightSettings.diffuseIntensity, 0.0f, 2.0f, "%.3f"))
					{
						spotlight->setDiffuseIntensity(spotlightSettings.diffuseIntensity);
						spotlight->setDiffuseColour();
					}

					if (ImGui::SliderFloat("Specular Intensity##Spotlight", (float*)&spotlightSettings.specularIntensity, 0.0f, 1.0f, "%.3f"))
					{
						spotlight->setSpecularIntensity(spotlightSettings.specularIntensity);
						spotlight->setSpecularColour();
					}

					ImGui::SeparatorText("Attenuation");

					if (ImGui::SliderFloat("Attenuation Constant##Spotlight", (float*)&spotlightSettings.attenuationConstant, 0.0f, 1.0f, "%.3f"))
					{
						spotlight->setConstant(spotlightSettings.attenuationConstant);
					}

					if (ImGui::SliderFloat("Attenuation Linear##Spotlight", (float*)&spotlightSettings.attenuationLinear, 0.0014f, 0.7f, "%.4f", ImGuiSliderFlags_Logarithmic))
					{
						spotlight->setLinear(spotlightSettings.attenuationLinear);
					}

					if (ImGui::SliderFloat("Attenuation Quadratic##Spotlight", (float*)&spotlightSettings.attenuationQuadratic, 0.000007f, 1.8f, "%.6f", ImGuiSliderFlags_Logarithmic))
					{
						spotlight->setQuadratic(spotlightSettings.attenuationQuadratic);
					}

					ImGui::SeparatorText("Spotlight Cone");

					if (ImGui::SliderFloat("Inner Cone Angle##Spotlight", (float*)&spotlightSettings.innerCutOff, 0.0f, 90.0f, "%.2f"))
					{
						spotlight->setInnerCutOff(spotlightSettings.innerCutOff);
					}

					if (ImGui::SliderFloat("Outer Cone Angle##Spotlight", (float*)&spotlightSettings.outerCutOff, 0.0f, 90.0f, "%.2f"))
					{
						spotlight->setOuterCutOff(spotlightSettings.outerCutOff);
					}

					ImGui::SeparatorText("Other");

					if (ImGui::Checkbox("Enabled##Spotlight", &spotlightSettings.enabled))
					{
						lightingShader->setUniformBoolean("u_isSpotlLightEnabled", spotlightSettings.enabled);
					}

					ImGui::NewLine();
				}

				if (ImGui::CollapsingHeader("Camera"))
				{
					if (ImGui::SliderFloat("Field of View (FOV)##Camera", &cameraSettings.fieldOfView, 1.0f, 45.0f, "%.2f"));
					{
						camera->setFOV(cameraSettings.fieldOfView);
					}

					if (ImGui::SliderFloat("Camera Speed##Camera", &cameraSettings.speed, 60.0f, 600.0f, "%.2f"));
					{
						camera->setCameraSpeed(cameraSettings.speed);
					}

					if (ImGui::SliderFloat("Near Plane##Camera", &cameraSettings.nearPlane, 0.01f, 100.0f, "%.2f"));
					{
						camera->setNearPlane(cameraSettings.nearPlane);
					}

					if (ImGui::SliderFloat("Far Plane##Camera", &cameraSettings.farPlane, 100.0f, 5000.0f, "%.2f"));
					{
						camera->setFarPlane(cameraSettings.farPlane);
					}

					ImGui::NewLine();
				}

				if (ImGui::CollapsingHeader("Boxes"))
				{
					GLuint shininessValueTemp = directionalLight->getShininessValue();
					if (ImGui::SliderInt("Shininess Value##Boxes", (GLint*)&shininessValueTemp, 1, 256))
					{
						directionalLight->setShininessValue(shininessValueTemp);
					}

					ImGui::NewLine();
				}

				if (ImGui::CollapsingHeader("Rendering"))
				{
					ImGui::Checkbox("Multiple Cubes##Rendering", &renderingSettings.isRenderingMultipleCubes);

					if (ImGui::Checkbox("Wireframe##Rendering", &renderingSettings.isWireframeEnabled))
					{
						if (!renderingSettings.isWireframeEnabled)
						{
							LOG_ERRORS(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
						}
						else
						{
							LOG_ERRORS(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
						}
					}

					ImGui::Checkbox("VSync", &renderingSettings.isVSyncActive);
				}

				if (ImGui::CollapsingHeader("Other Information"))
				{
					ImGui::Text("Application's Average FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				}
		
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			camera->processCameraInputs(engineWindow->getWindow());
			LOG_ERRORS(engineWindow->processInput(engineWindow->getWindow()));
			LOG_ERRORS(glfwSwapBuffers(engineWindow->getWindow())); // Swap front buffer and back buffer

			glfwPollEvents(); // Handle all GLFW events
	}

		lightingShader->stopUsingShader();
		lightSourceShader->stopUsingShader();

		// =========================== Memory Management - Deleting instantiated objects =====================================================
}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(engineWindow->getWindow()); // Deletes the window
	glfwTerminate(); // Terminates the program
}

/// <summary>
/// Creates the required objects and sets the lights' initial settings
/// </summary>
void GraphicsEngine::createEntities()
{
	// ==================== Creating Objects ================================ //

	cube = new BasicCube();

	directionalLight = new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f));
	spotlight = new SpotLight(glm::vec3(1.0f, 1.0f, 1.0f));

	for (GLuint i = 0; i < (sizeof(pointLights) / sizeof(pointLights[0])); i++)
	{
		pointLights[i] = new PointLight(glm::vec3(1.0, 0.0f, 1.0f));
	}

	renderer = new Renderer();

	VAO1 = new VertexArrayObject();
	VBO1 = new VertexBufferObject(cube->getVerticies().data(), cube->getVerticies().size());
	layout1 = new VertexBufferLayout();
	EBO1 = new ElementBufferObject(cube->getIndices().data(), cube->getIndices().size());

	VAO2 = new VertexArrayObject();
	VBO2 = new VertexBufferObject(pointLights[0]->  getVerticies().data(), pointLights[0]->getVerticies().size());
	layout2 = new VertexBufferLayout();
	EBO2 = new ElementBufferObject(pointLights[0]->getIndices().data(), pointLights[0]->getVerticies().size());

	lightingShader = new Shader("Shaders\\Light.vert", "Shaders\\Light.frag");
	lightSourceShader = new Shader("Shaders\\LightSource.vert", "Shaders\\LightSource.frag");

	texture1 = new Texture("Textures\\Images\\container2.png");
	texture2 = new Texture("Textures\\Images\\container2_specular.png");

	camera = new Camera();


	// ==================== Setting the Lights' Initial Values ================================ //

	directionalLightSettings.ambientIntensity = directionalLight->getAmbientIntensity();
	directionalLightSettings.diffuseIntensity = directionalLight->getDiffuseIntensity();
	directionalLightSettings.specularIntensity = directionalLight->getSpecularIntensity();
	directionalLight->setAmbientColour();
	directionalLight->setDiffuseColour();
	directionalLight->setSpecularColour();

	spotlightSettings.ambientIntensity = spotlight->getAmbientIntensity();
	spotlightSettings.diffuseIntensity = spotlight->getDiffuseIntensity();
	spotlightSettings.specularIntensity = spotlight->getSpecularIntensity();
	spotlightSettings.attenuationConstant = spotlight->getConstant();
	spotlightSettings.attenuationLinear = spotlight->getLinear();
	spotlightSettings.attenuationQuadratic = spotlight->getQuadratic();
	spotlightSettings.innerCutOff = glm::degrees(glm::acos(spotlight->getInnerCutOff()));
	spotlightSettings.outerCutOff = glm::degrees(glm::acos(spotlight->getOuterCutOff()));
	spotlight->setDiffuseColour();
	spotlight->setAmbientColour();
	spotlight->setSpecularColour();

	pointLightSettings.ambientIntensity = pointLights[0]->getAmbientIntensity();
	pointLightSettings.diffuseIntensity = pointLights[0]->getDiffuseIntensity();
	pointLightSettings.specularIntensity = pointLights[0]->getSpecularIntensity();
	pointLightSettings.attenuationConstant = pointLights[0]->getConstant();
	pointLightSettings.attenuationLinear = pointLights[0]->getLinear();
	pointLightSettings.attenuationQuadratic = pointLights[0]->getQuadratic();
	
	for (PointLight* pointLight : pointLights)
	{
		pointLight->setDiffuseColour();
		pointLight->setAmbientColour();
		pointLight->setSpecularColour();
	} 

	// ==================== Setting the Initial Colour Values ================================ //

	colourSettings.clearColour = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colourSettings.directionalLightColour = ImVec4(directionalLight->getLightColour().r, directionalLight->getLightColour().g, directionalLight->getLightColour().b, 1.0f);
	colourSettings.pointLightColour = ImVec4(pointLights[0]->getLightColour().r, pointLights[0]->getLightColour().g, pointLights[0]->getLightColour().b, 1.0f);
	colourSettings.spotlightColour = ImVec4(spotlight->getLightColour().r, spotlight->getLightColour().g, spotlight->getLightColour().b, 1.0f);

	// ==================== Setting the Camera's' Initial Values ================================ //

	cameraSettings.fieldOfView = camera->getFOV();
	cameraSettings.speed = camera->getCameraSpeed();
	cameraSettings.nearPlane = camera->getNearPlane();
	cameraSettings.farPlane = camera->getFarPlane();

	// ==================== Adding elements to layouts ================================ //

	layout1->pushElement<float>(3);
	layout1->pushElement<float>(3);
	layout1->pushElement<float>(2);
	layout2->pushElement<float>(3);

	// ==================== Adding Buffers ============================================== //

	LOG_ERRORS(VAO1->addBuffer(*VBO1, *layout1));
	LOG_ERRORS(VAO2->addBuffer(*VBO2, *layout2));

	// ==================== Binding and Setting Textures ================================ //

	texture1->bind(0);
	texture2->bind(1);
	lightingShader->useShader();
	lightingShader->setUniform1i("u_material.diffuse", 0);
	lightingShader->setUniform1i("u_material.specular", 1);
	lightingShader->stopUsingShader();
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
