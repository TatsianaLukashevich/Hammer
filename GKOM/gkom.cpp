#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>

// Other includes
#include "Shader.h"
#include "Camera.h"

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
GLfloat lastlastFrame = 0.0f;

// Camera
Camera  camera(glm::vec3(0.75f, 0.5f, -2.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}


}


void do_move()
{
	// Camera controls
	if (keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// This function loads a texture from file
GLuint loadTexture(GLchar* path)
{
	// Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// freeing unnecessary texture stuff
	SOIL_free_image_data(image);
	return textureID;

}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animation", nullptr, nullptr);
	if (window == nullptr)
		throw exception("GLFW window not created");
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	
	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw exception("GLEW Initialization failed");

	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);


	// Build and compile our shader program
	Shader gkomShader("gkom.vs", "gkom.frag");
	

GLfloat base_verticies[] = {
	//base
	//back
	-0.4, 0.05, -0.2, 0, 0, -1, 1, 1,
	0.4, 0.05, -0.2, 0, 0, -1, 0, 1,
	0.4, 0, -0.2, 0, 0, -1, 0, 0,

	0.4, 0, -0.2, 0, 0, -1, 0, 0,
	-0.4, 0, -0.2, 0, 0, -1, 1, 0,
	-0.4, 0.05, -0.2, 0, 0, -1, 1, 1,

	//up
	0.4, 0.05, -0.2, 0, 1, 0, 0, 0,
	0.4, 0.05, 0.2, 0, 1, 0, 0, 1, 
	-0.4, 0.05, 0.2, 0, 1, 0, 1, 1,

	-0.4, 0.05, 0.2, 0, 1, 0, 1, 1,
	-0.4, 0.05, -0.2, 0, 1, 0, 1, 0,
	0.4, 0.05, -0.2, 0, 1, 0, 0, 0,

	//front
	-0.4, 0.05, 0.2, 0, 0, 1, 1, 1,
	0.4, 0.05, 0.2, 0, 0, 1, 0, 1,
	0.4, 0, 0.2, 0, 0, 1, 0, 0,

	0.4, 0, 0.2, 0, 0, 1, 0, 0,
	-0.4, 0, 0.2, 0, 0, 1, 1, 0,
	-0.4, 0.05, 0.2, 0, 0, 1, 1, 1,

	//right
	-0.4, 0.05, -0.2, -1, 0, 0, 0, 1,
	-0.4, 0.05, 0.2, -1, 0, 0, 1, 1,
	-0.4, 0, 0.2, -1, 0, 0, 1, 0,

	-0.4, 0, 0.2, -1, 0, 0, 1, 0,
	-0.4, 0, -0.2, -1, 0, 0, 0, 0,
	-0.4, 0.05, -0.2, -1, 0, 0, 0, 1,

	//left
	0.4, 0.05, -0.2, 1, 0, 0, 0, 1,
	0.4, 0.05, 0.2, 1, 0, 0, 1, 1,
	0.4, 0, 0.2, 1, 0, 0, 1, 0,

	0.4, 0, 0.2, 1, 0, 0, 1, 0,
	0.4, 0, -0.2, 1, 0, 0, 0, 0,
	0.4, 0.05, -0.2, 1, 0, 0, 0, 1,

	//yellow
	//back
	-0.35, 0.05, -0.1, 0, 0, -1, 0, 0, 
	-0.35, 0.45, -0.1, 0, 0, -1, 0, 1,
	-0.25, 0.45, -0.1, 0, 0, -1, 1, 1,

	-0.25, 0.45, -0.1, 0, 0, -1, 1, 1,
	-0.25, 0.05, -0.1, 0, 0, -1, 1, 0,
	-0.35, 0.05, -0.1, 0, 0, -1, 0, 0,

	//front
	-0.35, 0.05, 0.1, 0, 0, 1, 0, 0,
	-0.35, 0.45, 0.1, 0, 0, 1, 0, 1,
	-0.25, 0.45, 0.1, 0, 0, 1, 1, 1,

	-0.25, 0.45, 0.1, 0, 0, 1, 1, 1,
	-0.25, 0.05, 0.1, 0, 0, 1, 1, 0,
	-0.35, 0.05, 0.1, 0, 0, 1, 0, 0,

	//right
	-0.35, 0.45, -0.1, -1, 0, 0, 0, 1, 
	-0.35, 0.45, 0.1, -1, 0, 0, 1, 1,
	-0.35, 0.05, 0.1, -1, 0, 0, 1, 0,

	-0.35, 0.05, 0.1, -1, 0, 0, 1, 0,
	-0.35, 0.05, -0.1, -1, 0, 0, 0, 0,
	-0.35, 0.45, -0.1, -1, 0, 0, 0, 1,

	//left
	-0.25, 0.45, -0.1, 1, 0, 0, 0, 1,
	-0.25, 0.45, 0.1, 1, 0, 0, 1, 1,
	-0.25, 0.05, 0.1, 1, 0, 0, 1, 0,

	-0.25, 0.05, 0.1, 1, 0, 0, 1, 0,
	-0.25, 0.05, -0.1, 1, 0, 0, 0, 0,
	-0.25, 0.45, -0.1, 1, 0, 0, 0, 1,

	//up
	-0.35, 0.45, -0.1, 0, 1, 0, 0, 0, 
	-0.35, 0.45, 0.1, 0, 1, 0, 0, 1, 
	-0.25, 0.45, 0.1, 0, 1, 0, 1, 1,

	-0.25, 0.45, 0.1, 0, 1, 0, 1, 1,
	-0.25, 0.45, -0.1, 0, 1, 0, 1, 0, 
	-0.35, 0.45, -0.1, 0, 1, 0, 0, 0,


	//prostopadl pomar
	//back
	0.25, 0.05, -0.03, 0, 0, -1, 0, 0,
	0.25, 0.115, -0.03, 0, 0, -1, 0, 1,
	0.29, 0.115, -0.03, 0, 0, -1, 1, 1,

	0.29, 0.115, -0.03, 0, 0, -1, 1, 1,
	0.29, 0.05, -0.03, 0, 0, -1, 1, 0,
	0.25, 0.05, -0.03, 0, 0, -1, 0, 0,

	//front
	0.25, 0.05, 0.03, 0, 0, 1, 0, 0,
	0.25, 0.115, 0.03, 0, 0, 1, 0, 1,
	0.29, 0.115, 0.03, 0, 0, 1, 1, 1,

	0.29, 0.115, 0.03, 0, 0, 1, 1, 1,
	0.29, 0.05, 0.03, 0, 0, 1, 1, 0,
	0.25, 0.05, 0.03, 0, 0, 1, 0, 0,

	//left
	0.29, 0.05, -0.03, 1, 0, 0, 0, 0,
	0.29, 0.115, -0.03, 1, 0, 0, 0, 1,
	0.29, 0.115, 0.03, 1, 0, 0, 1, 1,

	0.29, 0.115, 0.03, 1, 0, 0, 1, 1,
	0.29, 0.05, 0.03, 1, 0, 0, 1, 0,
	0.29, 0.05, -0.03, 1, 0, 0, 0, 0,

	//right
	0.25, 0.05, -0.03, -1, 0, 0, 0, 0,
	0.25, 0.115, -0.03, -1, 0, 0, 0, 1,
	0.25, 0.115, 0.03, -1, 0, 0, 1, 1,

	0.25, 0.115, 0.03, -1, 0, 0, 1, 1,
	0.25, 0.05, 0.03, -1, 0, 0, 1, 0,
	0.25, 0.05, -0.03, -1, 0, 0, 0, 0,

	//up
	0.25, 0.115, -0.03, 0, 1, 0, 0, 0,
	0.25, 0.115, 0.03, 0, 1, 0, 0, 1,
	0.29, 0.115, 0.03, 0, 1, 0, 1, 1,

	0.29, 0.115, 0.03, 0, 1, 0, 1, 1,
	0.29, 0.115, -0.03, 0, 1, 0, 1, 0,
	0.25, 0.115, -0.03, 0, 1, 0, 0, 0,

};

	GLfloat hammer_verticies[] = {
		// Positions        // Normals   // Texture Coords
		
		//pink
		//back
		-0.25, 0.35, -0.05, 0, 0, -1, 0, 0,
		-0.25, 0.4, -0.05, 0, 0, -1, 0, 1,
		0.2, 0.4, -0.05, 0, 0, -1, 1, 1,

		0.2, 0.4, -0.05, 0, 0, -1, 1, 1,
		0.2, 0.35, -0.05, 0, 0, -1, 1, 0,
		-0.25, 0.35, -0.05, 0, 0, -1, 0, 0,

		//front
		-0.25, 0.35, 0.05, 0, 0, 1, 0, 0,
		-0.25, 0.4, 0.05, 0, 0, 1, 0, 1,
		0.2, 0.4, 0.05, 0, 0, 1, 1, 1,

		0.2, 0.4, 0.05, 0, 0, 1, 1, 1,
		0.2, 0.35, 0.05, 0, 0, 1, 1, 0,
		-0.25, 0.35, 0.05, 0, 0, 1, 0, 0,

		//up
		-0.25, 0.4, -0.05, 0, 1, 0, 0, 0,
		-0.25, 0.4, 0.05, 0, 1, 0, 0, 1,
		0.2, 0.4, 0.05, 0, 1, 0, 1, 1,

		0.2, 0.4, 0.05, 0, 1, 0, 1, 1,
		0.2, 0.4, -0.05, 0, 1, 0, 1, 0,
		-0.25, 0.4, -0.05, 0, 1, 0, 0, 0,

		//down
		-0.25, 0.35, -0.05, 0, -1, 0, 0, 0,
		-0.25, 0.35, 0.05, 0, -1, 0, 0, 1,
		0.2, 0.35, 0.05, 0, -1, 0, 1, 1,

		0.2, 0.35, 0.05, 0, -1, 0, 1, 1,
		0.2, 0.35, -0.05, 0, -1, 0, 1, 0,
		-0.25, 0.35, -0.05, 0, -1, 0, 0, 0,


		//hammer
		//back
		0.2, 0.3, -0.1, 0, 0, -1, 0, 0,
		0.2, 0.45, -0.1, 0, 0, -1, 0, 1,
		0.35, 0.45, -0.1, 0, 0, -1, 1, 1,

		0.35, 0.45, -0.1, 0, 0, -1, 1, 1,
		0.35, 0.3, -0.1, 0, 0, -1, 1, 0,
		0.2, 0.3, -0.1, 0, 0, -1, 0, 0,

		//front
		0.2, 0.3, 0.1, 0, 0, 1, 0, 0,
		0.2, 0.45, 0.1, 0, 0, 1, 0, 1,
		0.35, 0.45, 0.1, 0, 0, 1, 1, 1,

		0.35, 0.45, 0.1, 0, 0, 1, 1, 1,
		0.35, 0.3, 0.1, 0, 0, 1, 1, 0,
		0.2, 0.3, 0.1, 0, 0, 1, 0, 0,

		//left
		0.35, 0.3, -0.1, 1, 0, 0, 0, 0,
		0.35, 0.45, -0.1, 1, 0, 0, 0, 1,
		0.35, 0.45, 0.1, 1, 0, 0, 1, 1,

		0.35, 0.45, 0.1, 1, 0, 0, 1, 1,
		0.35, 0.3, 0.1, 1, 0, 0, 1, 0,
		0.35, 0.3, -0.1, 1, 0, 0, 0, 0,

		//right
		0.2, 0.3, -0.1, -1, 0, 0, 0, 0,
		0.2, 0.45, -0.1, -1, 0, 0, 0, 1,
		0.2, 0.45, 0.1, -1, 0, 0, 1, 1,

		0.2, 0.45, 0.1, -1, 0, 0, 1, 1,
		0.2, 0.3, 0.1, -1, 0, 0, 1, 0,
		0.2, 0.3, -0.1, -1, 0, 0, 0, 0,

		//up
		0.2, 0.45, -0.1, 0, 1, 0, 0, 0,
		0.2, 0.45, 0.1, 0, 1, 0, 0, 1,
		0.35, 0.45, 0.1, 0, 1, 0, 1, 1,

		0.35, 0.45, 0.1, 0, 1, 0, 1, 1,
		0.35, 0.45, -0.1, 0, 1, 0, 1, 0,
		0.2, 0.45, -0.1, 0, 1, 0, 0, 0,

		//down
		0.2, 0.3, -0.1, 0, -1, 0, 0, 0,
		0.2, 0.3, 0.1, 0, -1, 0, 0, 1,
		0.35, 0.3, 0.1, 0, -1, 0, 1, 1,

		0.35, 0.3, 0.1, 0, -1, 0, 1, 1,
		0.35, 0.3, -0.1, 0, -1, 0, 1, 0,
		0.2, 0.3, -0.1, 0, -1, 0, 0, 0,



		//maly prostopadl
		//back
		0.265, 0.13, -0.11, 0, 0, -1, 0, 0,
		0.265, 0.33, -0.11, 0, 0, -1, 0, 1,
		0.275, 0.33, -0.11, 0, 0, -1, 1, 1,

		0.275, 0.33, -0.11, 0, 0, -1, 1, 1,
		0.275, 0.13, -0.11, 0, 0, -1, 1, 0,
		0.265, 0.13, -0.11, 0, 0, -1, 0, 0,

		//front
		0.265, 0.13, -0.1, 0, 0, 1, 0, 0,
		0.265, 0.33, -0.1, 0, 0, 1, 0, 1,
		0.275, 0.33, -0.1, 0, 0, 1, 1, 1,

		0.275, 0.33, -0.1, 0, 0, 1, 1, 1,
		0.275, 0.13, -0.1, 0, 0, 1, 1, 0,
		0.265, 0.13, -0.1, 0, 0, 1, 0, 0,

		//left
		0.275, 0.13, -0.11, 1, 0, 0, 0, 0,
		0.275, 0.33, -0.11, 1, 0, 0, 0, 1,
		0.275, 0.33, -0.1, 1, 0, 0, 1, 1,

		0.275, 0.33, -0.1, 1, 0, 0, 1, 1,
		0.275, 0.13, -0.1, 1, 0, 0, 1, 0,
		0.275, 0.13, -0.11, 1, 0, 0, 0, 0,

		//right
		0.265, 0.13, -0.11, -1, 0, 0, 0, 0,
		0.265, 0.33, -0.11, -1, 0, 0, 0, 1,
		0.265, 0.33, -0.1, -1, 0, 0, 1, 1,

		0.265, 0.33, -0.1, -1, 0, 0, 1, 1,
		0.265, 0.13, -0.1, -1, 0, 0, 1, 0,
		0.265, 0.13, -0.11, -1, 0, 0, 0, 0,

		//up
		0.265, 0.33, -0.11, 0, 1, 0, 0, 0,
		0.265, 0.33, -0.1, 0, 1, 0, 0, 1,
		0.275, 0.33, -0.1, 0, 1, 0, 1, 1,

		0.275, 0.33, -0.1, 0, 1, 0, 1, 1,
		0.275, 0.33, -0.11, 0, 1, 0, 1, 0,
		0.265, 0.33, -0.11, 0, 1, 0, 0, 0,

		//down
		0.265, 0.13, -0.11, 0, -1, 0, 0, 0,
		0.265, 0.13, -0.1, 0, -1, 0, 0, 1,
		0.275, 0.13, -0.1, 0, -1, 0, 1, 1,

		0.275, 0.13, -0.1, 0, -1, 0, 1, 1,
		0.275, 0.13, -0.11, 0, -1, 0, 1, 0,
		0.265, 0.13, -0.11, 0, -1, 0, 0, 0,
	};

	// Set up cylinder points
	GLfloat cylinder_vertices[] = {
		// Positions        // Normals   // Texture Coords
		//cylinder
	//back
	//1
	0.245, 0.15, -0.1, 0, 0, -1, 0, 0,
	0.25, 0.185, -0.1, 0, 0, -1, 0, 1,
	0.27, 0.15, -0.1, 0, 0, -1, 1, 1,

	0.27, 0.15, -0.1, 0, 0, -1, 1, 1,
	0.25, 0.115, -0.1, 0, 0, -1, 1, 0,
	0.245, 0.15, -0.1, 0, 0, -1, 0, 0,

	//2
	0.25, 0.185, -0.1, 0, 0, -1, 0, 0,
	0.27, 0.2, -0.1, 0, 0, -1, 0, 1,
	0.27, 0.15, -0.1, 0, 0, -1, 1, 0,

	0.27, 0.15, -0.1, 0, 0, -1, 1, 0,
	0.29, 0.185, -0.1, 0, 0, -1, 1, 1,
	0.27, 0.2, -0.1, 0, 0, -1, 0, 1,

	//3 (copy1)
	0.295, 0.15, -0.1, 0, 0, -1, 0, 0,
	0.29, 0.185, -0.1, 0, 0, -1, 0, 1,
	0.27, 0.15, -0.1, 0, 0, -1, 1, 1,

	0.27, 0.15, -0.1, 0, 0, -1, 1, 1,
	0.29, 0.115, -0.1, 0, 0, -1, 1, 0,
	0.295, 0.15, -0.1, 0, 0, -1, 0, 0,

	//4 (copy 2)
	0.25, 0.115, -0.1, 0, 0, -1, 0, 0,
	0.27, 0.1, -0.1, 0, 0, -1, 0, 1,
	0.27, 0.15, -0.1, 0, 0, -1, 1, 0,

	0.27, 0.15, -0.1, 0, 0, -1, 1, 0,
	0.29, 0.115, -0.1, 0, 0, -1, 1, 1,
	0.27, 0.1, -0.1, 0, 0, -1, 0, 1,

	//front (copy back change z)
	//1
	0.245, 0.15, 0.1, 0, 0, 1, 0, 0,
	0.25, 0.185, 0.1, 0, 0, 1, 0, 1,
	0.27, 0.15, 0.1, 0, 0, 1, 1, 1,

	0.27, 0.15, 0.1, 0, 0, 1, 1, 1,
	0.25, 0.115, 0.1, 0, 0, 1, 1, 0,
	0.245, 0.15, 0.1, 0, 0, 1, 0, 0,

	//2
	0.25, 0.185, 0.1, 0, 0, 1, 0, 0,
	0.27, 0.2, 0.1, 0, 0, 1, 0, 1,
	0.27, 0.15, 0.1, 0, 0, 1, 1, 0,

	0.27, 0.15, 0.1, 0, 0, 1, 1, 0,
	0.29, 0.185, 0.1, 0, 0, 1, 1, 1,
	0.27, 0.2, 0.1, 0, 0, 1, 0, 1,

	//3 (copy1)
	0.295, 0.15, 0.1, 0, 0, 1, 0, 0,
	0.29, 0.185, 0.1, 0, 0, 1, 0, 1,
	0.27, 0.15, 0.1, 0, 0, 1, 1, 1,

	0.27, 0.15, 0.1, 0, 0, 1, 1, 1,
	0.29, 0.115, 0.1, 0, 0, 1, 1, 0,
	0.295, 0.15, 0.1, 0, 0, 1, 0, 0,

	//4 (copy 2)
	0.25, 0.115, 0.1, 0, 0, 1, 0, 0,
	0.27, 0.1, 0.1, 0, 0, 1, 0, 1,
	0.27, 0.15, 0.1, 0, 0, 1, 1, 0,

	0.27, 0.15, 0.1, 0, 0, 1, 1, 0,
	0.29, 0.115, 0.1, 0, 0, 1, 1, 1,
	0.27, 0.1, 0.1, 0, 0, 1, 0, 1,

	//cylinder prostopadl
	//1
	0.245, 0.15, -0.1, -1, 0, 0, 0, 0,
	0.25, 0.185, -0.1, -1, 0, 0, 0, 1,
	0.25, 0.185, 0.1, -1, 0, 0, 1, 1,

	0.25, 0.185, 0.1, -1, 0, 0, 1, 1,
	0.245, 0.15, 0.1, -1, 0, 0, 1, 0,
	0.245, 0.15, -0.1, -1, 0, 0, 0, 0,

	//2
	0.25, 0.185, -0.1, 0, 1, 0, 0, 0,
	0.27, 0.2, -0.1, 0, 1, 0, 0, 1,
	0.27, 0.2, 0.1, 0, 1, 0, 1, 1,

	0.27, 0.2, 0.1, 0, 1, 0, 1, 1,
	0.25, 0.185, 0.1, 0, 1, 0, 1, 0,
	0.25, 0.185, -0.1, 0, 1, 0, 0, 0,

	//3
	0.29, 0.185, -0.1, 0, 1, 0, 0, 0,
	0.27, 0.2, -0.1, 0, 1, 0, 0, 1,
	0.27, 0.2, 0.1, 0, 1, 0, 1, 1,

	0.27, 0.2, 0.1, 0, 1, 0, 1, 1,
	0.29, 0.185, 0.1, 0, 1, 0, 1, 0,
	0.29, 0.185, -0.1, 0, 1, 0, 0, 0,

	//4
	0.295, 0.15, -0.1, 1, 0, 0, 0, 0,
	0.29, 0.185, -0.1, 1, 0, 0, 0, 1,
	0.29, 0.185, 0.1, 1, 0, 0, 1, 1,

	0.29, 0.185, 0.1, 1, 0, 0, 1, 1,
	0.295, 0.15, 0.1, 1, 0, 0, 1, 0,
	0.295, 0.15, -0.1, 1, 0, 0, 0, 0,

	//5
	0.295, 0.15, -0.1, 1, 0, 0, 0, 0,
	0.29, 0.115, -0.1, 1, 0, 0, 0, 1,
	0.29, 0.115, 0.1, 1, 0, 0, 1, 1,

	0.29, 0.115, 0.1, 1, 0, 0, 1, 1,
	0.295, 0.15, 0.1, 1, 0, 0, 1, 0,
	0.295, 0.15, -0.1, 1, 0, 0, 0, 0,

	//6
	0.29, 0.115, -0.1, 0, -1, 0, 0, 0,
	0.27, 0.1, -0.1, 0, -1, 0, 0, 1,
	0.27, 0.1, 0.1, 0, -1, 0, 1, 1,

	0.27, 0.1, 0.1, 0, -1, 0, 1, 1,
	0.29, 0.115, 0.1, 0, -1, 0, 1, 0,
	0.29, 0.115, -0.1, 0, -1, 0, 0, 0,

	//7
	0.25, 0.115, -0.1, 0, -1, 0, 0, 0,
	0.27, 0.1, -0.1, 0, -1, 0, 0, 1,
	0.27, 0.1, 0.1, 0, -1, 0, 1, 1,

	0.27, 0.1, 0.1, 0, -1, 0, 1, 1,
	0.25, 0.115, 0.1, 0, -1, 0, 1, 0,
	0.25, 0.115, -0.1, 0, -1, 0, 0, 0,

	//8
	0.245, 0.15, -0.1, -1, 0, 0, 0, 0,
	0.25, 0.115, -0.1, -1, 0, 0, 0, 1,
	0.25, 0.115, 0.1, -1, 0, 0, 1, 1,

	0.25, 0.115, 0.1, -1, 0, 0, 1, 1,
	0.245, 0.15, 0.1, -1, 0, 0, 1, 0,
	0.245, 0.15, -0.1, -1, 0, 0, 0, 0,
	};


	// Positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(1.95f, 1.0f, 0.0f), //left
		glm::vec3(0.0f, 2.0f, 0.0f),  //up
		glm::vec3(0.0f, 0.05f, -1.35f), //back
	};

	GLfloat planeVertices[] = {
		// Positions   //Normals   // Texture Coords
		//down
		1, 0, 1, 0, 1, 0, 1, 0,
		-1, 0, 1, 0, 1, 0, 0, 0,
		-1, 0, -1, 0, 1, 0, 0, 1,

		1, 0, 1, 0, 1, 0, 1, 0,
		-1, 0, -1, 0, 1, 0, 0, 1,
		1, 0, -1, 0, 1, 0, 1, 1,
		
		//up
		1, 1, 1, 0, -1, 0, 1, 0,
		-1, 1, 1, 0, -1, 0, 0, 0,
		-1, 1, -1, 0, -1, 0, 0, 1,

		1, 1, 1, 0, -1, 0, 1, 0,
		-1, 1, -1, 0, -1, 0, 0, 1,
		1, 1, -1, 0, -1, 0, 1, 1,

		//left
		1, 0, -1, -1, 0, 0, 1, 0,
		1, 0, 1, -1, 0, 0, 0, 0,
		1, 1, -1, -1, 0, 0, 1, 1,

		1, 1, -1, -1, 0, 0, 1, 1,
		1, 1, 1, -1, 0, 0, 0, 1,
		1, 0, 1, -1, 0, 0, 0, 0,

		//front
		1, 0, 1, 0, 0, -1, 1, 0,
		-1, 0, 1, 0, 0, -1, 0, 0,
		-1, 1, 1, 0, 0, -1, 0, 1,

		1, 1, 1, 0, 0, -1, 1, 1,
		1, 0, 1, 0, 0, -1, 1, 0,
		-1, 1, 1, 0, 0, -1, 0, 1,

		//right
		-1, 1, 1, 1, 0, 0, 1, 1,
		-1, 1, -1, 1, 0, 0, 0, 1,
		-1, 0, 1, 1, 0, 0, 1, 0,

		-1, 0, 1, 1, 0, 0, 1, 0,
		-1, 1, -1, 1, 0, 0, 0, 1,
		-1, 0, -1, 1, 0, 0, 0, 0,

		//back
		-1, 0, -1, 0, 0, 1, 0, 0,
		1, 0, -1, 0, 0, 1, 1, 0,
		-1, 1, -1, 0, 0, 1, 0, 1,
		-1, 1, -1, 0, 0, 1, 0, 1,
		1, 1, -1, 0, 0, 1, 1, 1,
		1, 0, -1, 0, 0, 1, 1, 0,


	};

	// First, set the VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	

	// Setup plane VAO
	GLuint planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);

	// Setup cylinder VAO
	GLuint cylinderVAO, cylinderVBO;
	glGenVertexArrays(1, &cylinderVAO);
	glGenBuffers(1, &cylinderVBO);
	glBindVertexArray(cylinderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_vertices), &cylinder_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);

	// Setup hammer VAO
	GLuint hammerVAO, hammerVBO;
	glGenVertexArrays(1, &hammerVAO);
	glGenBuffers(1, &hammerVBO);
	glBindVertexArray(hammerVAO);
	glBindBuffer(GL_ARRAY_BUFFER, hammerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hammer_verticies), &hammer_verticies, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);

	// Setup base VAO
	GLuint baseVAO, baseVBO;
	glGenVertexArrays(1, &baseVAO);
	glGenBuffers(1, &baseVBO);
	glBindVertexArray(baseVAO);
	glBindBuffer(GL_ARRAY_BUFFER, baseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base_verticies), &base_verticies, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);

	// Load textures
	GLuint planeTexture = loadTexture("niebo.jpg");
	GLuint figureTexture = loadTexture("drewno.jpg");

	// Set texture units
	gkomShader.Use();
	glUniform1i(glGetUniformLocation(gkomShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(gkomShader.Program, "material.specular"), 1);


	int countframe = 0;
	int counta = 0, countb = 0;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_move();
	

		// Use cooresponding shader when setting uniforms/drawing objects
		gkomShader.Use();
		GLint viewPosLoc = glGetUniformLocation(gkomShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		// Set material properties
		glUniform1f(glGetUniformLocation(gkomShader.Program, "material.shininess"), 32.0f);

		// Here we set all the uniforms for the types of lights we have
		// Point light 1
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(gkomShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(gkomShader.Program, "pointLights[0].linear"), 0.09);
		glUniform1f(glGetUniformLocation(gkomShader.Program, "pointLights[0].quadratic"), 0.032);
		// Point light 2
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[1].diffuse"), 1.3f, 1.3f, 1.3f);
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(gkomShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(gkomShader.Program, "pointLights[1].linear"), 0.09);
		glUniform1f(glGetUniformLocation(gkomShader.Program, "pointLights[1].quadratic"), 0.032);
		// Point light 3
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[2].ambient"), 0.25f, 0.25f, 0.25f);
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(gkomShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(gkomShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(gkomShader.Program, "pointLights[2].linear"), 0.09);
		glUniform1f(glGetUniformLocation(gkomShader.Program, "pointLights[2].quadratic"), 0.032);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);



		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(gkomShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(gkomShader.Program, "view");
		GLint projLoc = glGetUniformLocation(gkomShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind planeMap
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planeTexture);

		// Draw the plane
		glm::mat4 model;
		glBindVertexArray(planeVAO);
		model = glm::mat4();
		model = glm::scale(model, glm::vec3(2, 2, 2)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Bind figureMap
		glBindTexture(GL_TEXTURE_2D, figureTexture);


		// Draw the base
		glBindVertexArray(baseVAO);
		model = glm::mat4();
		model = glm::scale(model, glm::vec3(2, 1.5, 2)); //(1, 0.66, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 90);
		glBindVertexArray(0);

		// Draw the hammer
		glBindVertexArray(hammerVAO);
		model = glm::mat4();
		model = glm::scale(model, glm::vec3(2, 1.5, 2)); 

		
		model = glm::rotate(model, 0.13f, glm::vec3(0.0f, 0.0f, 1.0f));
			counta = round(currentFrame);
			if (counta >= currentFrame)
			{
				countb = counta;
				currentFrame = currentFrame - 1;
				countb = round(currentFrame);
			}
			else
			{
				currentFrame = currentFrame + 1;
				countb = round(currentFrame);
			}
	
		if ((counta%2) && (!(countb%2)))
		{
			model = glm::rotate(model, -0.13f, glm::vec3(0.0f, 0.0f, 1.0f));
			
		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 108);
		glBindVertexArray(0);

	
		// Draw the cylinder
		glBindVertexArray(cylinderVAO);
		model = glm::mat4();
		model = glm::scale(model, glm::vec3(2, 1.5, 2)); 

			model = glm::rotate(model, 59.75f, glm::vec3(0.0f, 0.0f, 1.0f));//59,75
			model = glm::translate(model, glm::vec3(-0.545f, -0.29f, 0.0f));
		
			
			if ((counta % 2) && (!(countb % 2)))
			{
				model = glm::rotate(model, -59.75f, glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::translate(model, glm::vec3(-0.53f, -0.31f, 0.0f));
			}
	

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 96);
		
		glBindVertexArray(0);

			// Swap the screen buffers
		glfwSwapBuffers(window);
		countframe++;
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
