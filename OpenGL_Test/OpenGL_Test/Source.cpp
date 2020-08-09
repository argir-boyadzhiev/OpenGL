#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "FPScamera.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

FPScamera camera;

float lastX = 400, lastY = 300;

bool firstMouse = true;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MEME FACE", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("3.3.shader.vs","3.3.shader.fs");
	
	Model backpack("C:/Users/User/source/repos/OpenGL/OpenGL/OpenGL_Test/OpenGL_Test/Models/Backpack/backpack.obj");

	glm::vec3 lightPosition(0.5, 1, -6);

	//shader
	ourShader.use();

	ourShader.setInt("material.diffuse", 0);
	ourShader.setInt("material.specular", 1);
	ourShader.setFloat("material.shininess", 32);
	ourShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	ourShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	ourShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	
	//render loop
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (lightPosition.z < 6) {
			lightPosition.z += 0.001;
		}
		else {
			lightPosition.z = -8;
		}

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		

		//matrix
		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		glm::mat4 view;
		view = camera.GetViewMatrix();
		

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		ourShader.use();
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);
		glm::vec3 nLightPosition = view * glm::vec4(lightPosition,1);
		ourShader.setVec3("light.position", glm::vec3(0, 0, 0));
		ourShader.setVec3("light.direction", glm::vec3(0, 0, -1));
		ourShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.setFloat("light.outerCutOff", glm::cos(glm::radians(15.5f)));
		ourShader.use();

		glm::mat4 model = glm::mat4(1.0f);
		ourShader.setMat4("model", model);

		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(view * model)));
		ourShader.setMat3("normalMatrix", normalMatrix);

		backpack.Draw(ourShader);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glfwTerminate();
	return 0;
}	

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

	camera.ProcessMouseMovement(xoffset, yoffset);

	lastX = xpos;
	lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}