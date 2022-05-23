//Libraries
#include <iostream>
#include <cstdlib>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> //Transformation
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <camera.h>

using namespace std;

//Shader
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif // ! GLSL


namespace {
    const char* const WINDOW_TITLE = "Jose Talamantes"; // Macro for window title

    //Setting variables for the width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Stores the GL data relative to a given mesh
    struct GLMesh {
        //Handle for the vertex array object
        GLuint keyboardVao;
        GLuint planeVao;
        GLuint keysVao;
        GLuint cylinderVao;
        //Handles for the vertex buffer objects
        GLuint keyboardVbo;
        GLuint planeVbo;
        GLuint keysVbo;
        GLuint cylinderVbo;
        //Number of vertices of the mesh
        GLuint nVertices;
        GLuint nPlaneVertices;
    };

    //Main GLFW window
    GLFWwindow* gWindow = nullptr;
    //Triangle mesh data
    GLMesh gMesh;
    //Texture program
    GLuint gTextureId;
    GLuint gTexturePlane;
    GLuint gTextureCarpet;
    GLuint gTexturekeys;
    GLuint gTextureCylinder;
    GLuint gTextureController;
    GLuint gTextureControllerGrip;
    glm::vec2 gUVScale(5.0f, 5.0f);
    GLint gTexWrapMode = GL_REPEAT;

    // Shader programs
    GLuint gProgramId;
    GLuint gLightProgramId;
    GLuint gPlaneProgramId;
    GLuint gCarpetProgramId;
    GLuint gKeysProgramId;
    GLuint gCylinderProgramId;
    GLuint gControllerProgramId;
    GLuint gControllerGripProgramId;


    //Camera settings
    Camera gCamera(glm::vec3(0.0f, 3.0f, 4.5f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    //Timing settings
    float gDeltaTime = 0.0f;
    float gLastFrame = 0.0f;

    //Keyboard position and scale
    glm::vec3 gKeyboardPosition(0.0f, 2.2f, 2.0f);
    glm::vec3 gKeyboardScale(2.0f, 1.0f, 2.0f);

    //Plane position and scale
    glm::vec3 gCarpetPosition(0.0f, 0.85f, 2.0f);
    glm::vec3 gCarpetScale(10.0f, 1.0f, 17.5f);

    //Plane position and scale
    glm::vec3 gPlanePosition(0.0f, 2.1f, 2.0f);
    glm::vec3 gPlaneScale(7.0f, 1.0f, 12.5f);
    glm::vec3 gBackLeftPosition(-2.0f, 1.5f, 0.85f);
    glm::vec3 gFrontLeftScale(2.0f, 2.0f, 1.0f);
    glm::vec3 gFrontLeftRotate(0.0f, 0.0f, 1.0f);
    glm::vec3 gBackRightPosition(1.99f, 1.5f, 0.85f);
    glm::vec3 gBackRightScale(2.0f, 2.0f, 1.0f);
    glm::vec3 gBackRightRotate(0.0f, 0.0f, 1.0f);
    glm::vec3 gFrontLeftPosition(-2.0f, 1.5f, 3.15f);
    glm::vec3 gFrontRightPosition(1.99f, 1.5f, 3.15f);


    //Keys position and scale
    glm::vec3 gKeyPosition(0.5f, 2.2f, 1.0f);
    glm::vec3 gKeyScale(1.0f, 1.0f, 1.0f);

    //Controller position and scale
    glm::vec3 gControllerPosition(0.5f, 2.2f, 3.0f);
    glm::vec3 gControllerScale(0.5f, 0.5f, 0.8f);
    glm::vec3 gControllerLeftPosition(0.4f, 2.2f, 3.05f);
    glm::vec3 gControllerLeftScale(0.3f, 0.5f, 0.4f);
    glm::vec3 gControllerLeftRotate(0.0f, 1.0f, 0.0f);
    glm::vec3 gControllerRightPosition(0.6f, 2.2f, 3.05f);
    glm::vec3 gControllerRightScale(0.3f, 0.5f, 0.4f);
    glm::vec3 gControllerRightRotate(0.0f, 1.0f, 0.0f);

    //Cylinder position and scale
    glm::vec3 gCylinderPosition(0.0f, 2.2f, 2.0f);
    glm::vec3 gCylinderScale(2.0f);

    //Light color
    glm::vec3 gObjectColor(1.f, 0.2f, 0.0f);
    glm::vec3 gLightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 gKeyLightColor(1.0f, 1.0f, 1.0f);

    //Light position and scale
    glm::vec3 gLightPosition(0.0f, 4.5f, 3.0f);
    glm::vec3 gLightScale(0.3f);
    glm::vec3 gSecondLightPosition(0.0f, 4.5f, 3.0f);

}

//Initialize the program
bool UInitialize(int, char* [], GLFWwindow** window);
//Sets the size of the window
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
//Mouse functions
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
//Redraws graphic on the window
void UCreateMesh(GLMesh& mesh);
void UCreateMeshCylinder(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
//Sets texture
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);
//Renders the graphics
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

//Vertex Shader Source Code
const GLchar* keyboardVertexShaderSource = GLSL(440,
    //Vertex data from Vertex Attrib Pointer 0
    layout(location = 0) in vec3 position;
//Color data from Vertex Attrib Pointer 1
layout(location = 1) in vec3 normal;
//Texture data from Vertex Attrib Pointer 2
layout(location = 2) in vec2 textureCoordinate;

//Outgoing normals to fragment shader
out vec3 vertexNormal;
//Outgoing color to fragment shader
out vec3 vertexFragmentPos;
//Outgoing texture to fragment shader
out vec2 vertexTextureCoordinate;

//Global variables for the transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    //Turn vertices into clip coordinates
    gl_Position = projection * view * model * vec4(position, 1.0f);

    //Gets fragment position in world space
    vertexFragmentPos = vec3(model * vec4(position, 1.0f));

    //Get normal vectors in world space only and exclude normal properties
    vertexNormal = mat3(transpose(inverse(model))) * normal;
    vertexTextureCoordinate = textureCoordinate;
}
);


//Fragment Shader Source Code
const GLchar* keyboardFragmentShaderSource = GLSL(440,

    //Incoming normals
    in vec3 vertexNormal;
//Incoming fragment position
in vec3 vertexFragmentPos;
in vec2 vertexTextureCoordinate;

//Variable to hold incoming color data from vertex shader
out vec4 fragmentColor;

//Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 keyLightColor;
uniform vec3 lightPos;
uniform vec3 viewPosition;
uniform sampler2D uTexture;
uniform vec2 uvScale;

void main() {
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    //Calculate ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    //Calculate diffuse lighting
    vec3 norm = normalize(vertexNormal);
    vec3 lightDirection = normalize(lightPos - vertexFragmentPos);
    float impact = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = impact * lightColor;

    //Calculate specular lighting
    float specularIntensity = 0.1f;
    float highlightSize = 16.0f;
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    //Calculate specular component
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    //Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

    //Calculate phong result
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

    float secondSpecularIntensity = 1.0f;
    vec3 secondAmbient = ambientStrength * keyLightColor;
    vec3 SecondLightDirection = normalize(vec3(0.0f, 6.0f, 3.0f) - vertexFragmentPos);
    impact = max(dot(norm, SecondLightDirection), 0.0);
    diffuse = impact * keyLightColor;
    viewDir = normalize(viewPosition - vertexFragmentPos);
    reflectDir = reflect(-SecondLightDirection, norm);
    specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);

    vec3 secondSpecular = secondSpecularIntensity * specularComponent * keyLightColor;
    vec3 phongTwo = (secondAmbient + diffuse + secondSpecular) * textureColor.xyz;

    //Send lighting results to GPU
    fragmentColor = vec4(phong + phongTwo, 1.0);
}
);

//Vertex Shader Source Code
const GLchar* PlaneVertexShaderSource = GLSL(440,
    //Vertex data from Vertex Attrib Pointer 0
    layout(location = 0) in vec3 position;
    //Color data from Vertex Attrib Pointer 1
    layout(location = 1) in vec3 normal;
    //Texture data from Vertex Attrib Pointer 2
    layout(location = 2) in vec2 textureCoordinate;
    
    //Outgoing normals to fragment shader
    out vec3 vertexNormal;
    //Outgoing color to fragment shader
    out vec3 vertexFragmentPos;
    //Outgoing texture to fragment shader
    out vec2 vertexTextureCoordinate;
    
    //Global variables for the transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

void main() {
    //Turn vertices into clip coordinates
    gl_Position = projection * view * model * vec4(position, 1.0f);

    //Gets fragment position in world space
    vertexFragmentPos = vec3(model * vec4(position, 1.0f));

    //Get normal vectors in world space only and exclude normal properties
    vertexNormal = mat3(transpose(inverse(model))) * normal;
    vertexTextureCoordinate = textureCoordinate;
}
);

//Fragment Shader Source Code
const GLchar* PlaneFragmentShaderSource = GLSL(440,
    //Incoming normals
    in vec3 vertexNormal;
    //Incoming fragment position
    in vec3 vertexFragmentPos;
    in vec2 vertexTextureCoordinate;
    
    //Variable to hold incoming color data from vertex shader
    out vec4 fragmentColor;
    
    //Global variables for object color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 keyLightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D uTexture;
    uniform vec2 uvScale;

void main() {
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    //Calculate ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    //Calculate diffuse lighting
    vec3 norm = normalize(vertexNormal);
    vec3 lightDirection = normalize(lightPos - vertexFragmentPos);
    float impact = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = impact * lightColor;

    //Calculate specular lighting
    float specularIntensity = 0.1f;
    float highlightSize = 16.0f;
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    //Calculate specular component
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    //Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

    //Calculate phong result
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

    float secondSpecularIntensity = 1.0f;
    vec3 secondAmbient = ambientStrength * keyLightColor;
    vec3 SecondLightDirection = normalize(vec3(0.0f, 6.0f, 3.0f) - vertexFragmentPos);
    impact = max(dot(norm, SecondLightDirection), 0.0);
    diffuse = impact * keyLightColor;
    viewDir = normalize(viewPosition - vertexFragmentPos);
    reflectDir = reflect(-SecondLightDirection, norm);
    specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);

    vec3 secondSpecular = secondSpecularIntensity * specularComponent * keyLightColor;
    vec3 phongTwo = (secondAmbient + diffuse + secondSpecular) * textureColor.xyz;

    //Send lighting results to GPU
    fragmentColor = vec4(phong + phongTwo, 1.0);
}
);


//Light Shader Source Code
const GLchar* lightVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position;

//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
);


//Light Fragment Source Code
const GLchar* lightFragmentShaderSource = GLSL(440,

    out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(1.0f);
}
);


// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels) {
    for (int j = 0; j < height / 2; ++j) {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i) {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}


int main(int argc, char* argv[]) {
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    //Creates the mesh
    UCreateMesh(gMesh);

    //Create the shader program
    if (!UCreateShaderProgram(keyboardVertexShaderSource, keyboardFragmentShaderSource, gProgramId))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(lightVertexShaderSource, lightFragmentShaderSource, gLightProgramId))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(PlaneVertexShaderSource, PlaneFragmentShaderSource, gPlaneProgramId))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(PlaneVertexShaderSource, PlaneFragmentShaderSource, gControllerProgramId))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(PlaneVertexShaderSource, PlaneFragmentShaderSource, gControllerGripProgramId))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(PlaneVertexShaderSource, PlaneFragmentShaderSource, gCarpetProgramId))
        return EXIT_FAILURE;

    //Load texture
    const char* texFilename = "../resources/textures/Controller.jpg";
    if (!UCreateTexture(texFilename, gTextureId)) {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    const char* texPlaneWood = "../resources/textures/Wood.jpg";
    if (!UCreateTexture(texPlaneWood, gTexturePlane)) {
        cout << "Failed to load texture " << texPlaneWood << endl;
        return EXIT_FAILURE;
    }

    const char* texGrip = "../resources/textures/BlackGrip.jpg";
    if (!UCreateTexture(texGrip, gTextureControllerGrip)) {
        cout << "Failed to load texture " << texGrip << endl;
        return EXIT_FAILURE;
    }

    const char* texController = "../resources/textures/Controller.jpg";
    if (!UCreateTexture(texController, gTextureController)) {
        cout << "Failed to load texture " << texController << endl;
        return EXIT_FAILURE;
    }

    const char* texCarpet = "../resources/textures/BlueCarpet.jpg";
    if (!UCreateTexture(texCarpet, gTextureCarpet)) {
        cout << "Failed to load texture " << texCarpet << endl;
        return EXIT_FAILURE;
    }

    glUseProgram(gPlaneProgramId);
    glUniform1i(glGetUniformLocation(gPlaneProgramId, "uTexture"), 0);

    glUseProgram(gProgramId);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexture"), 0);

    glUseProgram(gControllerProgramId);
    glUniform1i(glGetUniformLocation(gControllerProgramId, "uTexture"), 0);

    glUseProgram(gTextureControllerGrip);
    glUniform1i(glGetUniformLocation(gTextureControllerGrip, "uTexture"), 0);

    glUseProgram(gCarpetProgramId);
    glUniform1i(glGetUniformLocation(gCarpetProgramId, "uTexture"), 0);

    //Sets the background color of the window to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //Render loop
    while (!glfwWindowShouldClose(gWindow))
    {
        //Per-frame timing
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        //input
        UProcessInput(gWindow);

        //Render the frames
        URender();

        glfwPollEvents();
    }

    //Release mesh data
    UDestroyMesh(gMesh);

    //Release texture
    UDestroyTexture(gTextureId);

    //Release shader program
    UDestroyShaderProgram(gProgramId);
    UDestroyShaderProgram(gLightProgramId);

    //Terminates the program
    exit(EXIT_SUCCESS);
}


//Initiates GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window) {
    //Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // _APPLE_

    //GLFW window creation
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Initiates GLEW
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    //Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


//Process input within GLFW
void UProcessInput(GLFWwindow* window) {
    static const float cameraSpeed = 2.5f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.ProcessKeyboard(UPWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.ProcessKeyboard(DOWNWARD, gDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
        gUVScale += 0.1f;
        cout << "Current scale (" << gUVScale[0] << ", " << gUVScale[1] << ")" << endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
        gUVScale -= 0.1f;
        cout << "Current scale (" << gUVScale[0] << ", " << gUVScale[1] << ")" << endl;
    }


}


//GLFW window size change
void UResizeWindow(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


//Mouse position callback
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (gFirstMouse) {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}


//Mouse Scroll callback
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    gCamera.ProcessMouseScroll(yoffset);
}

//Mouse button callback
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT: {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
                               break;
    case GLFW_MOUSE_BUTTON_MIDDLE: {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
                                 break;
    case GLFW_MOUSE_BUTTON_RIGHT: {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
                                break;
    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}


//Render the frames
void URender() {
    //Enable z-depth
    glEnable(GL_DEPTH_TEST);

    //Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Activation of VAO
    glBindVertexArray(gMesh.keyboardVao);

    //Pyramid
    //Shader program
    glUseProgram(gProgramId);

    //Transforms the camera: Moves camera back
    glm::mat4 model = glm::translate(gKeyboardPosition) * glm::scale(gKeyboardScale);

    //Camera view transformation
    glm::mat4 view = gCamera.GetViewMatrix();

    //Creates perspective projection
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    //Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //Reference matrix uniforms from the Cube Shader program for the cub color, light color, light position, and camera position
    GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    GLint secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    GLint lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    //Carpet/Plane
    glUseProgram(gCarpetProgramId);

    model = glm::translate(gCarpetPosition) * glm::scale(gCarpetScale);

    //Retrives and passes the program to Shader program
    modelLoc = glGetUniformLocation(gCarpetProgramId, "model");
    viewLoc = glGetUniformLocation(gCarpetProgramId, "view");
    projLoc = glGetUniformLocation(gCarpetProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    cameraPosition == gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //Bind textrures to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureCarpet);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    //Table top
    glUseProgram(gPlaneProgramId);

    model = glm::translate(gPlanePosition) * glm::scale(gPlaneScale);

    //Retrives and passes the program to Shader program
    modelLoc = glGetUniformLocation(gPlaneProgramId, "model");
    viewLoc = glGetUniformLocation(gPlaneProgramId, "view");
    projLoc = glGetUniformLocation(gPlaneProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    cameraPosition == gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //Bind textrures to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexturePlane);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    glUseProgram(gPlaneProgramId);

    model = glm::translate(gBackLeftPosition) * glm::scale(gFrontLeftScale) * glm::rotate(80.1f, gFrontLeftRotate);

    //Retrives and passes the program to Shader program
    modelLoc = glGetUniformLocation(gPlaneProgramId, "model");
    viewLoc = glGetUniformLocation(gPlaneProgramId, "view");
    projLoc = glGetUniformLocation(gPlaneProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    cameraPosition == gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //Bind textrures to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexturePlane);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);


    //Back right leg
    glUseProgram(gPlaneProgramId);

    model = glm::translate(gBackRightPosition) * glm::scale(gBackRightScale) * glm::rotate(80.1f, gFrontLeftRotate);

    //Retrives and passes the program to Shader program
    modelLoc = glGetUniformLocation(gPlaneProgramId, "model");
    viewLoc = glGetUniformLocation(gPlaneProgramId, "view");
    projLoc = glGetUniformLocation(gPlaneProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    cameraPosition == gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //Bind textrures to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexturePlane);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    //Front left leg
    glUseProgram(gPlaneProgramId);

    model = glm::translate(gFrontLeftPosition) * glm::scale(gBackRightScale) * glm::rotate(80.1f, gFrontLeftRotate);

    //Retrives and passes the program to Shader program
    modelLoc = glGetUniformLocation(gPlaneProgramId, "model");
    viewLoc = glGetUniformLocation(gPlaneProgramId, "view");
    projLoc = glGetUniformLocation(gPlaneProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    cameraPosition == gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //Bind textrures to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexturePlane);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    //Front right leg
    glUseProgram(gPlaneProgramId);

    model = glm::translate(gFrontRightPosition) * glm::scale(gBackRightScale) * glm::rotate(80.1f, gFrontLeftRotate);

    //Retrives and passes the program to Shader program
    modelLoc = glGetUniformLocation(gPlaneProgramId, "model");
    viewLoc = glGetUniformLocation(gPlaneProgramId, "view");
    projLoc = glGetUniformLocation(gPlaneProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    cameraPosition == gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //Bind textrures to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexturePlane);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    //Controller
    glUseProgram(gControllerProgramId);

    model = glm::translate(gControllerPosition) * glm::scale(gControllerScale);

    //Retrives and passes the program to Shader program
    modelLoc = glGetUniformLocation(gControllerProgramId, "model");
    viewLoc = glGetUniformLocation(gControllerProgramId, "view");
    projLoc = glGetUniformLocation(gControllerProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    cameraPosition == gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //Bind textrures to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureControllerGrip);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    //Left Grip
    glUseProgram(gControllerGripProgramId);

    model = glm::translate(gControllerLeftPosition) * glm::scale(gControllerLeftScale) * glm::rotate(45.0f, gControllerLeftRotate);

    //Retrives and passes the program to Shader program
    modelLoc = glGetUniformLocation(gControllerGripProgramId, "model");
    viewLoc = glGetUniformLocation(gControllerGripProgramId, "view");
    projLoc = glGetUniformLocation(gControllerGripProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    cameraPosition == gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //Bind textrures to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureControllerGrip);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    glUseProgram(gControllerGripProgramId);

    model = glm::translate(gControllerRightPosition) * glm::scale(gControllerRightScale) * glm::rotate(-45.0f, gControllerRightRotate);

    //Retrives and passes the program to Shader program
    modelLoc = glGetUniformLocation(gControllerGripProgramId, "model");
    viewLoc = glGetUniformLocation(gControllerGripProgramId, "view");
    projLoc = glGetUniformLocation(gControllerGripProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
    lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
    secondLightColorLoc = glGetUniformLocation(gProgramId, "keyLightColor");
    lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
    viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

    //Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(secondLightColorLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    cameraPosition == gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    //Bind textrures to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureControllerGrip);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    //Deactivates the Vertex Array Object
    glBindVertexArray(0);
    glUseProgram(0);

    //GLFW: Swap buffers and poll IO events
    glfwSwapBuffers(gWindow);
}


//Implements the UCreateMesh
void UCreateMesh(GLMesh& mesh) {
    //Position and Texture data
    GLfloat verts[] = {
        //Vertex            //Normal             //Textures
     //Back Face           
    -0.3f, -0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.3f, -0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.3f,  0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.3f,  0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.3f,  0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.3f, -0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    //Front Face        
    -0.3f, -0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.3f, -0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.3f,  0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.3f,  0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.3f,  0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.3f, -0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    //Left Face          
     -0.3f,  0.05f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     -0.3f,  0.05f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     -0.3f, -0.05f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.3f, -0.05f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.3f, -0.05f,  0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     -0.3f,  0.05f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     //Right Face         
      0.3f,  0.05f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
      0.3f,  0.05f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
      0.3f, -0.05f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
      0.3f, -0.05f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
      0.3f, -0.05f,  0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
      0.3f,  0.05f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

      //Bottom Face        
      -0.3f, -0.05f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
       0.3f, -0.05f, -0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
       0.3f, -0.05f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       0.3f, -0.05f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      -0.3f, -0.05f,  0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
      -0.3f, -0.05f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

      //Top Face           
       -0.3f,  0.05f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.3f,  0.05f, -0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.3f,  0.05f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.3f,  0.05f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       -0.3f,  0.05f,  0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
       -0.3f,  0.05f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.keyboardVao);
    glBindVertexArray(mesh.keyboardVao);

    //Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.keyboardVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.keyboardVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    //Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    //Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

//Implements the UCreateMeshPlane
void UCreateMeshPlane(GLMesh& mesh) {
    //Position and Color data
    GLfloat verts[] = {
        //Vertex            //Normal             //Textures
     //Back Face           
    -0.3f, -0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.3f, -0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.3f,  0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.3f,  0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.3f,  0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.3f, -0.05f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    //Front Face        
    -0.3f, -0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.3f, -0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.3f,  0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.3f,  0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.3f,  0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.3f, -0.05f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    //Left Face          
     -0.3f,  0.05f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     -0.3f,  0.05f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     -0.3f, -0.05f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.3f, -0.05f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.3f, -0.05f,  0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     -0.3f,  0.05f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     //Right Face         
      0.3f,  0.05f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
      0.3f,  0.05f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
      0.3f, -0.05f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
      0.3f, -0.05f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
      0.3f, -0.05f,  0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
      0.3f,  0.05f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

      //Bottom Face        
      -0.3f, -0.05f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
       0.3f, -0.05f, -0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
       0.3f, -0.05f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       0.3f, -0.05f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      -0.3f, -0.05f,  0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
      -0.3f, -0.05f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

      //Top Face           
       -0.3f,  0.05f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.3f,  0.05f, -0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.3f,  0.05f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.3f,  0.05f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       -0.3f,  0.05f,  0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
       -0.3f,  0.05f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nPlaneVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.planeVao);
    glBindVertexArray(mesh.planeVao);

    //Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.planeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.planeVbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    //Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    //Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);

}


void UDestroyMesh(GLMesh& mesh) {
    glDeleteVertexArrays(1, &mesh.keyboardVao);
    glDeleteBuffers(1, &mesh.keyboardVbo);
}


/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId) {
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image) {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        //Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    //Error loading the image
    return false;
}


void UDestroyTexture(GLuint textureId) {
    glGenTextures(1, &textureId);
}


// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId) {
    //Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    //Create a Shader program object.
    programId = glCreateProgram();

    //Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    //Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    //Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    //Check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId);

    //Check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    //Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);
    //Check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);

    return true;
}


void UDestroyShaderProgram(GLuint programId) {
    glDeleteProgram(programId);
}
