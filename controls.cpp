// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::mat4 getModelMatrix() {
	return ModelMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( -700, 300, 10 );
glm::vec3 getCameraPosition() {
	return position;
}
// Initial horizontal angle : toward -Z
float horizontalAngle = 7.845f;
// Initial vertical angle : none
float verticalAngle = -0.40f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 250.0f; // 250 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP) == GLFW_PRESS){
		position += direction * deltaTime * speed;
		}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		position = glm::vec3(-700, 300, 10);
		horizontalAngle = 7.845f;
		verticalAngle = -0.40f;
		ModelMatrix = glm::mat4(1.f);
	}

	
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {

		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(3.f), glm::vec3(0.f, 1.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {

		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(3.f), glm::vec3(1.f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {

		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(3.f), glm::vec3(0.f, 0.f, 1.f));
	}
	

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(3.f), glm::vec3(0.f, 1.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, -1.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(3.f), glm::vec3(0.f, -1.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-5.f, 0.f, 0.f));
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 1500.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	
	// For the next frame, the "last time" will be "now"
	
	lastTime = currentTime;
}
