#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

int main() {

    //start the glContext and O/S window using the GLFW helper library

    if (!glfwInit()) {
        fprintf(stderr, "error: could not start GLFW3\n");
        return 1;
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR:Could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    //start glew extension handle
    glewExperimental = GL_TRUE;
    glewInit();

    //get version info

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);

    printf("Rendere: %s\n", renderer);
    printf("OpenGL Version Supported %s\n", version);

    //only draw if shape is close to the viewer
    glEnable(GL_DEPTH_TEST); //enable depth testing
    glDepthFunc(GL_LESS); //smaller value is closer


    glfwTerminate();
    cout << "Hello, World!" << endl;
    return 0;
}