//example skeleton code 2019 winter comp371
//modified from http://learnopengl.com/

#include <GL/glew.h>	// include GL Extension Wrangler
#include <glfw/glfw3.h>	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "OBJloader.h"  //include the object loadr
#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image.h"
using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow *window;

Camera camera = Camera();
bool isPartB = false;
GLuint planeVAO;

vector<Light> lightsPartA = {
	Light(glm::vec3(0.2f, 0.05f, 0.05f), glm::vec3(10.0f, 15.0f, 5.0f)), 
	Light(glm::vec3(0.05f, 0.2f, 0.05f), glm::vec3(-10.0f, -15.0f, 5.0f)),
	Light(glm::vec3(0.05f, 0.05f, 0.2f), glm::vec3(0.0f, 15.0f, 5.0f)),
	Light(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 0.0f, 25.0f))
};

//taken from learnopengl.com
float planeVertices[] = {
    // positions            // normals         // texcoords
     25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
    -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

     25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
    -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
     25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
};

// 
//Struct that contains all variables and behaviors related to the object
struct {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); /*!< Position of the object */
	glm::vec3 scale = glm::vec3(0.25f, 0.25f, 0.25f); /*!< Current scale of the object */
	float pitch = 0.0f; /*!< Current pitch of the object */
	float yaw = 0.0f; /*!< Current yuw of the object */
	float roll = 0.0f; /*!< Current roll of the object */

	//! Reset the object to its initial configurations.
	void reset() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		pitch = 0.0f;
		yaw = 0.0f;
		roll = 0.0f;
	}
} object;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
//Keeps the previsou ypos of the cursor
	static double prev_ypos = -1;

	//Initialize the prev_ypos if first cursor_position_callback
	if(prev_ypos == -1)
		prev_ypos = ypos;

	//Move into/out of the scene (assumed that means chaning fov) only when GLFW_MOUSE_BUTTON_LEFT is press
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		double ypos_delta = prev_ypos - ypos;
		// Move the camera in/out of the scene proportionally.
		if (ypos_delta > 0) {
			camera.moveForward();
		} else if (ypos_delta < 0) {
			camera.moveBackward();
		}
	}
	
	// Keeping track of ypos
	prev_ypos = ypos;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwSetCursorPosCallback(window, cursor_position_callback);
	} else {
		glfwSetCursorPosCallback(window, NULL);
	}
}

// Is called whenever a key is pressed/released via GLFW
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// If the key is being release ignore it
	if(action == GLFW_RELEASE){
		return;
	}

	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_R) {
		camera.reset();
		object.reset();
	}
	else if (key == GLFW_KEY_W) {
		camera.moveForward();
	}
	else if (key == GLFW_KEY_S) {
		camera.moveBackward();
	}
	else if (key == GLFW_KEY_A) {
		camera.moveLeft();
	}
	else if (key == GLFW_KEY_D) {
		camera.moveRight();
	}
	else if (key == GLFW_KEY_UP) {
		camera.pitch(-5.0f);
	}
	else if (key == GLFW_KEY_DOWN) {
		camera.pitch(5.0f);
	}
	else if (key == GLFW_KEY_RIGHT) {
		camera.yaw(5.0f);
	}
	else if (key == GLFW_KEY_LEFT) {
		camera.yaw(-5.0f);
	}
	else if (key == GLFW_KEY_J) {
		object.position.x += 1.0f;
	}
	else if (key == GLFW_KEY_L) {
		object.position.x -= 1.0f;
	}
	else if (key == GLFW_KEY_I) {
		object.position.y += 1.0f;
	}
	else if (key == GLFW_KEY_K) {
		object.position.y -= 1.0f;
	}
	else if (key == GLFW_KEY_PAGE_UP) {
		object.position.z += 1.0f;
	}
	else if (key == GLFW_KEY_PAGE_DOWN) {
		object.position.z -= 1.0f;
	}
	else if (key == GLFW_KEY_O) {
		object.scale.x = object.scale.y = object.scale.z *= 1.10f;
	}
	else if (key == GLFW_KEY_P) {
		object.scale.x = object.scale.y = object.scale.z *= 0.9f;
	}
	else if (key == GLFW_KEY_B) {
		if(object.pitch + 1.0f > 360.0f){
			object.pitch = 0.0f;
		} else {
			object.pitch += 1.0f;
		}
	}
	else if (key == GLFW_KEY_N) {
		if(object.yaw + 1.0f > 360.0f){
			object.yaw = 0.0f;
		} else {
			object.yaw += 1.0f;
		}
	}
	else if (key == GLFW_KEY_E) {
		if(object.roll + 1.0f > 360.0f){
			object.roll = 0.0f;
		} else {
			object.roll += 1.0f;
		}
	}
	else if (key == GLFW_KEY_F1) {
		isPartB = false;
	}
	else if (key == GLFW_KEY_F2) {
		isPartB = true;
	}
}

int init() {
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//WINDOW
	window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 3", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	//Define the depth buffer and enable it
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}

void renderScene(Shader activeShader, GLuint activeVAO, int numberOfEdges) {
	// floor
    glm::mat4 model = glm::mat4(1.0f);
	activeShader.setMat4(model, "model");
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
	//Building a perspective view
 	model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Constructing the ctm (camera transformation matrix)
	model = glm::translate(model, object.position);
	model = glm::rotate(model, glm::radians(object.roll), glm::vec3(0,0,1))*glm::rotate(glm::mat4(1.0f), glm::radians(object.yaw), glm::vec3(0, 1, 0))* glm::rotate(glm::mat4(1.0f), glm::radians(object.pitch), glm::vec3(1,0,0));
	model = glm::scale(model, object.scale);

	// Create view and projection matrix
	view = glm::lookAt(camera.getPosition(), camera.getFront() + camera.getPosition(), camera.getUp());
	projection = glm::perspective(glm::radians(camera.getFov()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	//Passing the ctm (camera transformation matrix) to the shaders
	activeShader.setMat4(model, "model");
	activeShader.setMat4(view, "view");
	activeShader.setMat4(projection, "projection");

	// Change lighting depending on the camera position
	activeShader.setVec3(camera.getPosition(), "viewPosition");

	glBindVertexArray(activeVAO);
	glDrawElements(GL_TRIANGLES, numberOfEdges, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
// taken from learnopengl.com
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void createPlane() {
	// taken from learnopengl.com
    GLuint planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
	
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	if (init() != 0)
		return EXIT_FAILURE;
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	createPlane();

	// Build and compile our shader programs

	Shader shader = Shader("./shaders/vertex.shader", "./shaders/fragment.shader");
	Shader shadowShader = Shader("./shaders/shadowVertex.shader", "./shaders/shadowFragment.shader");
	Shader depthShader = Shader("./shaders/depthVertex.shader", "./shaders/depthFragment.shader");

	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	loadOBJ("./objects/heracles.obj", indices, vertices, normals, UVs); //read the vertices from the cat.obj file

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint verticesVBO;
	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	GLuint normalsVBO;
	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLuint texture;
	glGenBuffers(1, &texture);
	glBindBuffer(GL_ARRAY_BUFFER, texture);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	shader.use();
	// Passing the ambient, diffuse and specular coefficients
	shader.setFloat(0.25f, "ambientCoefficient");
	shader.setFloat(0.75f, "diffuseCoefficient");
	shader.setFloat(1.0f, "specularCoefficient");
	shader.setVec3(glm::vec3(1.0f, 1.0f, 1.0f), "objectColor");

	// Passing the various light for part A
	for(int i = 0; i < lightsPartA.size(); i++) {
		shader.setVec3(lightsPartA[i].getPosition(), ("lightsPartA["  + to_string(i) + "].position").c_str());
		shader.setVec3(lightsPartA[i].getColor(), ("lightsPartA["  + to_string(i) + "].color").c_str());
	}

	shadowShader.use();
	// Passing light for part B
	shadowShader.setVec3(glm::vec3(0.0f, 20.0f, 10.0f), "lightPosition");
	shadowShader.setVec3(glm::vec3(0.8f, 0.2f, 0.2f), "lightColor");
	// Passing the ambient, diffuse and specular coefficients
	shadowShader.setFloat(0.25f, "ambientCoefficient");
	shadowShader.setFloat(0.75f, "diffuseCoefficient");
	shadowShader.setFloat(1.0f, "specularCoefficient");

	// Prepare the framebuffer object to create shadow
  	GLuint shadowMapFBO;
    glGenFramebuffers(1, &shadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

	// Prepare the shadow textrue
    GLuint shadowMapTex;
    glGenTextures(1, &shadowMapTex);
	glBindTexture(GL_TEXTURE_2D, shadowMapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
             WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.0f, 0.0f, 0.0f, 0.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint marbleTexture = loadTexture("./textures/marble.jpg");

	// Activate the texture each shader will use
	shadowShader.use();
	shadowShader.setInt(0, "diffuseTexture");
	shadowShader.setInt(1, "shadowMap");

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer and depth buffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
		
		if(isPartB){
			float near_plane = 1.0f;
			float far_plane = 40.0f;
			float SHADOW_HEIGHT = 1024;
			float SHADOW_WIDTH = 1024;
			
			
			// Configure camera from light source location
			glm::mat4 lightProjection = glm::perspective(glm::radians(65.0f), (float)SHADOW_HEIGHT/ (float)SHADOW_WIDTH, near_plane, far_plane);
			glm::vec3 lightFront = glm::normalize(glm::vec3(0, 20, 10) - glm::vec3(0,0,0));
			glm::vec3 rightLight = glm::cross(glm::vec3(0,1,0), lightFront);
			glm::vec3 upLight = glm::cross(glm::normalize(rightLight), lightFront);
			glm::mat4 lightView = glm::lookAt(glm::vec3(0.0, 20.0, 10.0), glm::vec3(0.0f), upLight);
			glm::mat4 lightMatrix = lightProjection * lightView;
			
			// First Pass
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			depthShader.use();
			depthShader.setMat4(lightMatrix, "lightSpaceMatrix");
			glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    		glClear(GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, marbleTexture);
			renderScene(depthShader, VAO, indices.size());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// Needed to multiply by 2 for the retina display of a MacbookPro
			//glViewport(0, 0, WIDTH*2, HEIGHT*2);
			glViewport(0, 0, WIDTH, HEIGHT);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

			// Second Pass
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
			shadowShader.use();
			shadowShader.setMat4(lightMatrix, "lightSpaceMatrix");
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, marbleTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, shadowMapTex);
			renderScene(shadowShader, VAO, indices.size());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		} else {
			shader.use();
			renderScene(shader, VAO, indices.size());
		}

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}