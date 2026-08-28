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
	SpotLight* spotlight = new SpotLight(glm::vec3(1.0f, 1.0f, 1.0f));
	PointLight* pointLights[4] = {};
	pointLights[0] = new PointLight(glm::vec3(1.0, 0.0f, 1.0f));
	pointLights[1] = new PointLight(glm::vec3(1.0, 0.0f, 1.0f));
	pointLights[2] = new PointLight(glm::vec3(1.0, 0.0f, 1.0f));
	pointLights[3] = new PointLight(glm::vec3(1.0, 0.0f, 1.0f));


	GLfloat aspectRatio = 0.0f; 

	bool isSettingsWindowOpen = true;
	bool isWireframeEnabled = false;
	bool isVSyncActive = false;
	bool isRenderingMultipleCubes = true;

	ImVec4 directionalLightColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pointLightColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 spotlightColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 clearColour = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
		VertexBufferObject* VBO2 = new VertexBufferObject(pointLights[0]->getVerticies().data(), pointLights[0]->getVerticies().size());
		VertexBufferLayout* layout2 = new VertexBufferLayout;
		ElementBufferObject* EBO2 = new ElementBufferObject(pointLights[0]->getIndices().data(), pointLights[0]->getVerticies().size());


		layout1->pushElement<float>(3);
		layout1->pushElement<float>(3);
		layout1->pushElement<float>(2);

		layout2->pushElement<float>(3);

		layout2->pushElement<float>(3);

		LOG_ERRORS(VAO1->addBuffer(*VBO1, *layout1)); 
		LOG_ERRORS(VAO2->addBuffer(*VBO2, *layout2));
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

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f) };



		Shader* lightingShader = new Shader("DirectionalLight.vert", "DirectionalLight.frag");
		Shader* lightSourceShader = new Shader("lightCube.vert", "lightCube.frag"); 

		Texture* texture1 = new Texture("container2.png"); 
		Texture* texture2 = new Texture("container2_specular.png");
		texture1->bind(0); 
		texture2->bind(1);

		lightingShader->useShader(); 
		lightingShader->setUniform1i("u_material.diffuse", 0);
		lightingShader->setUniform1i("u_material.specular", 1);


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
				lightingShader->setUniformVector3("u_directionalLight.direction", directionalLight->getLightPosition().x, directionalLight->getLightPosition().y, directionalLight->getLightPosition().z);
				lightingShader->setUniformVector3("u_viewPosition", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
				lightingShader->setUniformVector3("u_directionalLight.ambient", directionalLight->getAmbientColour().r, directionalLight->getAmbientColour().g, directionalLight->getAmbientColour().b);
				lightingShader->setUniformVector3("u_directionalLight.diffuse", directionalLight->getDiffuseColour().r, directionalLight->getDiffuseColour().g, directionalLight->getDiffuseColour().b);
				lightingShader->setUniformVector3("u_directionalLight.specular", directionalLight->getSpecularColour().r, directionalLight->getSpecularColour().g, directionalLight->getSpecularColour().b);

				// Point Lights
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

				// Spotlight 
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

				// Boxes
				lightingShader->setUniform1f("u_material.shininess", static_cast<GLfloat>(directionalLight->getShininessValue()));
				lightingShader->setUniformMatrix4f("u_projection", projectionMatrix);
				lightingShader->setUniformMatrix4f("u_view", viewMatrix);
				lightingShader->setUniformMatrix4f("u_model", modelMatrix); 


				// =========================== Drawing the Boxes ===================================================== //

				if (isRenderingMultipleCubes)
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

				lightSourceShader->useShader();
				lightSourceShader->setUniformMatrix4f("u_projection", projectionMatrix);
				lightSourceShader->setUniformMatrix4f("u_view", viewMatrix);

				for (GLuint i = 0; i < (sizeof(pointLights) / sizeof(pointLights[0])); i++)
				{
					modelMatrix = glm::mat4(1.0f);
					modelMatrix = glm::translate(modelMatrix, pointLightPositions[i] * 250.0f);
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f)); // a smaller cube
					lightSourceShader->setUniformMatrix4f("u_model", modelMatrix);
					lightSourceShader->setUniformVector3("u_lightSourceColour", pointLights[i]->getLightColor().r, pointLights[i]->getLightColor().g, pointLights[i]->getLightColor().b);
					renderer->draw(*VAO2, *EBO2, *lightSourceShader); 
				}

				// =========================== Real-Time Updates and ImGUI ===================================================== //

				ImGui::Begin("Settings", &isSettingsWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

				lightingShader->useShader();

				if (ImGui::CollapsingHeader("Directional Lighting & Background", ImGuiTreeNodeFlags_DefaultOpen))
				{
					GLfloat sceneAmbientLightStrengthTemp = directionalLight->getAmbientIntensity();
					GLfloat sceneDiffuseLightStrengthTemp = directionalLight->getDiffuseIntensity();
					GLfloat sceneSpecularLightStrengthTemp = directionalLight->getSpecularIntensity();

					ImGui::SeparatorText("Colour");

					if (ImGui::ColorEdit3("Light Colour##DirectionalLight", (float*)&directionalLightColour), ImGuiColorEditFlags_DisplayRGB)
					{
						directionalLight->setLightColour(glm::vec3(directionalLightColour.x, directionalLightColour.y, directionalLightColour.z));
						directionalLight->setAmbientColour();
						directionalLight->setDiffuseColour();
						directionalLight->setSpecularColour();
					}

					if (ImGui::ColorEdit3("Background Colour", (float*)&clearColour, ImGuiColorEditFlags_DisplayRGB));
					{
						LOG_ERRORS(glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w));
					}

					ImGui::SeparatorText("Phong Lighting");

					if (ImGui::SliderFloat("Ambient Intensity##DirectionalLight", (float*)&sceneAmbientLightStrengthTemp, 0.013f, 1.0f, "%.3f"))
					{
						directionalLight->setAmbientIntensity(sceneAmbientLightStrengthTemp);
						directionalLight->setAmbientColour();
					}

					if (ImGui::SliderFloat("Diffuse Intensity##DirectionalLight", (float*)&sceneDiffuseLightStrengthTemp, 0.0f, 2.0f, "%.3f"))
					{
						directionalLight->setDiffuseIntensity(sceneDiffuseLightStrengthTemp);
						directionalLight->setDiffuseColour();
					}

					if (ImGui::SliderFloat("Specular Intensity##DirectionalLight", (float*)&sceneSpecularLightStrengthTemp, 0.0f, 1.0f, "%.3f"))
					{
						directionalLight->setSpecularIntensity(sceneSpecularLightStrengthTemp);
						directionalLight->setSpecularColour();
					}

					ImGui::NewLine();
				}

				if (ImGui::CollapsingHeader("Point Lighting", ImGuiTreeNodeFlags_DefaultOpen))
				{
					GLfloat pointLightAmbientLightStrengthTemp = pointLights[0]->getAmbientIntensity();
					GLfloat pointLightDiffuseLightStrengthTemp = pointLights[0]->getDiffuseIntensity();
					GLfloat pointLightSpecularLightStrengthTemp = pointLights[0]->getSpecularIntensity();

					GLfloat attenuationConstantTemp = pointLights[0]->getConstant();
					GLfloat attenuationLinearTemp = pointLights[0]->getLinear();
					GLfloat attenuationQuadraticTemp = pointLights[0]->getQuadratic();

					pointLightColour = ImVec4(pointLights[0]->getLightColor().r, pointLights[0]->getLightColor().g, pointLights[0]->getLightColor().b, 0.0f);

					ImGui::SeparatorText("Colour");

					if (ImGui::ColorEdit3("Light Colour##PointLight", (float*)&pointLightColour, ImGuiColorEditFlags_DisplayRGB))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setLightColour(glm::vec3(pointLightColour.x, pointLightColour.y, pointLightColour.z));
							pointLight->setDiffuseColour();
							pointLight->setAmbientColour();
							pointLight->setSpecularColour();
						}
					}

					ImGui::SeparatorText("Phong Lighting");

					if (ImGui::SliderFloat("Ambient Intensity##PointLight", (float*)&pointLightAmbientLightStrengthTemp, 0.0f, 2.0f, "%.3f"))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setAmbientIntensity(pointLightAmbientLightStrengthTemp);
							pointLight->setAmbientColour();
						}
					}
						
					if (ImGui::SliderFloat("Diffuse Intensity##PointLight", (float*)&pointLightDiffuseLightStrengthTemp, 0.0f, 2.0f, "%.3f"))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setDiffuseIntensity(pointLightDiffuseLightStrengthTemp);
							pointLight->setDiffuseColour();
						}
					}

					if (ImGui::SliderFloat("Specular Intensity##PointLight", (float*)&pointLightSpecularLightStrengthTemp, 0.0f, 1.0f, "%.3f"))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setSpecularIntensity(pointLightSpecularLightStrengthTemp);
							pointLight->setSpecularColour();
						}
					}

					ImGui::SeparatorText("Attenuation");

					if (ImGui::SliderFloat("Attenuation Constant##PointLight", (float*)&attenuationConstantTemp, 0.0f, 1.0f, "%.3f"))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setConstant(attenuationConstantTemp);
						}
					}

					if (ImGui::SliderFloat("Attenuation Linear##PointLight", (float*)&attenuationLinearTemp, 0.0014f, 0.7f, "%.4f", ImGuiSliderFlags_Logarithmic))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setLinear(attenuationLinearTemp);
						}
					}

					if (ImGui::SliderFloat("Attenuation Quadratic##PointLight", (float*)&attenuationQuadraticTemp, 0.000007f, 1.8f, "%.6f", ImGuiSliderFlags_Logarithmic))
					{
						for (PointLight* pointLight : pointLights)
						{
							pointLight->setQuadratic(attenuationQuadraticTemp);
						}
					}

					ImGui::NewLine();
				}

				if (ImGui::CollapsingHeader("Spotlight", ImGuiTreeNodeFlags_DefaultOpen))
				{
					GLfloat spotlightAmbientLightStrengthTemp = spotlight->getAmbientIntensity();
					GLfloat spotlightDiffuseLightStrengthTemp = spotlight->getDiffuseIntensity();
					GLfloat spotlightSpecularLightStrengthTemp = spotlight->getSpecularIntensity();

					GLfloat attenuationConstantTemp = spotlight->getConstant();
					GLfloat attenuationLinearTemp = spotlight->getLinear();
					GLfloat attenuationQuadraticTemp = spotlight->getQuadratic();

					GLfloat innerCutOffTemp = glm::degrees(glm::acos(spotlight->getInnerCutOff()));
					GLfloat outerCutOffTemp = glm::degrees(glm::acos(spotlight->getOuterCutOff()));

					ImGui::SeparatorText("Colour");

					if (ImGui::ColorEdit3("Light Colour##Spotlight", (float*)&spotlightColour, ImGuiColorEditFlags_DisplayRGB))
					{
						spotlight->setLightColour(glm::vec3(spotlightColour.x, spotlightColour.y, spotlightColour.z));
						spotlight->setDiffuseColour();
						spotlight->setAmbientColour();
						spotlight->setSpecularColour();
					}

					ImGui::SeparatorText("Phong Lighting");

					if (ImGui::SliderFloat("Ambient Intensity##Spotlight", (float*)&spotlightAmbientLightStrengthTemp, 0.0f, 2.0f, "%.3f"))
					{
						spotlight->setAmbientIntensity(spotlightAmbientLightStrengthTemp);
						spotlight->setAmbientColour();
					}

					if (ImGui::SliderFloat("Diffuse Intensity##Spotlight", (float*)&spotlightDiffuseLightStrengthTemp, 0.0f, 2.0f, "%.3f"))
					{
						spotlight->setDiffuseIntensity(spotlightDiffuseLightStrengthTemp);
						spotlight->setDiffuseColour();
					}

					if (ImGui::SliderFloat("Specular Intensity##Spotlight", (float*)&spotlightSpecularLightStrengthTemp, 0.0f, 1.0f, "%.3f"))
					{
						spotlight->setSpecularIntensity(spotlightSpecularLightStrengthTemp);
						spotlight->setSpecularColour();
					}

					ImGui::SeparatorText("Attenuation");

					if (ImGui::SliderFloat("Attenuation Constant##Spotlight", (float*)&attenuationConstantTemp, 0.0f, 1.0f, "%.3f"))
					{
						spotlight->setConstant(attenuationConstantTemp);
					}

					if (ImGui::SliderFloat("Attenuation Linear##Spotlight", (float*)&attenuationLinearTemp, 0.0014f, 0.7f, "%.4f", ImGuiSliderFlags_Logarithmic))
					{
						spotlight->setLinear(attenuationLinearTemp);
					}

					if (ImGui::SliderFloat("Attenuation Quadratic##Spotlight", (float*)&attenuationQuadraticTemp, 0.000007f, 1.8f, "%.6f", ImGuiSliderFlags_Logarithmic))
					{
						spotlight->setQuadratic(attenuationQuadraticTemp);
					}

					ImGui::SeparatorText("Spotlight Cone");

					if (ImGui::SliderFloat("Inner Cone Angle##Spotlight", (float*)&innerCutOffTemp, 0.0f, 90.0f, "%.2f"))
					{
						spotlight->setInnerCutOff(innerCutOffTemp);
					}

					if (ImGui::SliderFloat("Outer Cone Angle##Spotlight", (float*)&outerCutOffTemp, 0.0f, 90.0f, "%.2f"))
					{
						spotlight->setOuterCutOff(outerCutOffTemp);
					}

					ImGui::NewLine();
				}

				if (ImGui::CollapsingHeader("Camera"))
				{
					GLfloat fovTemp = camera->getFOV();
					if (ImGui::SliderFloat("Field of View (FOV)##Camera", &fovTemp, 1.0f, 45.0f, "%.2f"));
					{
						camera->setFOV(fovTemp);
					}

					GLfloat nearPlaneTemp = camera->getNearPlane();
					if (ImGui::SliderFloat("Near Plane##Camera", &nearPlaneTemp, 0.01f, 100.0f, "%.2f"));
					{
						camera->setNearPlane(nearPlaneTemp);
					}

					GLfloat farPlaneTemp = camera->getFarPlane();
					if (ImGui::SliderFloat("Far Plane##Camera", &farPlaneTemp, 100.0f, 5000.0f, "%.2f"));
					{
						camera->setFarPlane(farPlaneTemp);
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
					ImGui::Checkbox("Multiple Cubes##Rendering", &isRenderingMultipleCubes);

					if (ImGui::Checkbox("Wireframe##Rendering", &isWireframeEnabled))
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
				}

				if (ImGui::CollapsingHeader("Other Information"))
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

		lightingShader->stopUsingShader();
		lightSourceShader->stopUsingShader();

		// =========================== Memory Management - Deleting instantiated objects =====================================================

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
