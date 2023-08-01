// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // extender a janela e o contexto OpenGL dessa janela

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "CamContr.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}


//+Z
glm::vec3 position = glm::vec3(0, 0, 5);
// -Z
float horizontalAngle = 3.14f;
// angulo vertical da cam
float verticalAngle = 0.0f;
// campo de visão da cam
float initialFoV = 45.0f;

float speed = 3.0f;
float mouseSpeed = 0.005f;



void computeMatricesFromInputs() 
{

	// chamar o GLFWGETTIME ----------- teste --------- 
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// pegar posição do mouse
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// mouse posição para Reset
	glfwSetCursorPos(window, 700 / 2, 600 / 2);

	// orientação
	horizontalAngle += mouseSpeed * float(700 / 2 - xpos);
	verticalAngle += mouseSpeed * float(600 / 2 - ypos);

	
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Vetores Direita, Esquerda, Cima e Baixo
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	glm::vec3 up = glm::cross(right, direction);

	// Movimento
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(position,position + direction,up);
	lastTime = currentTime;
}
