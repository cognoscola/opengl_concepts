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


    /** code goes here **/
    GLfloat points[] = {
            0.0f,  0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
            -0.5f, -0.5f,  0.0f
    };

    GLuint vbo = 0;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (points), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    const char* vertex_shader =
            "#version 400\n"
                    "in vec3 vp;"
                    "void main () {"
                    "  gl_Position = vec4 (vp, 1.0);"
                    "}";

    const char* fragment_shader =
            "#version 400\n"
                    "out vec4 frag_colour;"
                    "void main () {"
                    "  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
                    "}";

    GLuint vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);
    GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);


    GLuint shader_programme = glCreateProgram ();
    glAttachShader (shader_programme, fs);
    glAttachShader (shader_programme, vs);
    glLinkProgram (shader_programme);

    while (!glfwWindowShouldClose (window)) {
        // wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram (shader_programme);
        glBindVertexArray (vao);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays (GL_TRIANGLES, 0, 3);
        // update other events like input handling
        glfwPollEvents ();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers (window);
    }

    glfwTerminate();
    cout << "Hello, World!" << endl;
    return 0;
}