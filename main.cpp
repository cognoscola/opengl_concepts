#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <assert.h>
#include <cmath>

#include "log/log.h"

using namespace std;

int g_gl_width = 640;
int g_gl_height = 480;


void glfw_error_callback(int error, const char* description) {
    gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
    g_gl_width = width;
    g_gl_height = height;
}



//tells us which line in which shader is causing the errollo

void _update_fps_count(GLFWwindow * window){

    static double previous_seconds = glfwGetTime();
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;

    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        double fps = (double) frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "opengl @fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}


int main() {

    assert(restart_gl_log());
    // log the glfw version
    gl_log("Starting GLFW\n%s\n", glfwGetVersionString());
    //register an error callback
    glfwSetErrorCallback(glfw_error_callback);

    //start the glContext and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "error: could not start GLFW3\n");
        return 1;
    }

    //note init libraries before setting hints
    glfwWindowHint (GLFW_SAMPLES, 4);

    //set to fulll screen mode
    GLFWmonitor *mon = glfwGetPrimaryMonitor();
    const GLFWvidmode *vmode = glfwGetVideoMode(mon);
    GLFWwindow *window = glfwCreateWindow(g_gl_width, g_gl_height, "Extended Gl Init", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR:Could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }


    glfwSetWindowSizeCallback(window, glfw_window_size_callback);
    glfwMakeContextCurrent(window);

    //start glew extension handle
    glewExperimental = GL_TRUE;
    glewInit();

    //get version info

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);

    printf("Rendere: %s\n", renderer);
    printf("OpenGL Version Supported %s\n", version);
    log_gl_params();

    //only draw if shape is close to the viewer
    glEnable(GL_DEPTH_TEST); //enable depth testing
    glDepthFunc(GL_LESS); //smaller value is closer
//    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);



    /** code goes here **/
    GLfloat points[] = {
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
    };

    GLfloat colours[] = {
            1.0f, 0.0f,  0.0f,
            0.0f, 1.0f,  0.0f,
            0.0f, 0.0f,  1.0f
    };



    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint colours_vbo = 0;
    glGenBuffers(1, &colours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    const char *vertex_shader =
            "#version 400\n"
                    "in vec3 vertex_position;"
                    "in vec3 vertex_colour;"

                    " out vec3 colour;"

                    "void main () {"
                    "  colour = vertex_colour;"
                    "  gl_Position =vec4 (vertex_position, 1.0);"
                    "}";

    const char *fragment_shader =
            "#version 400\n"
                    "in vec3 colour;"
                    "out vec4 frag_colour;"
                    "void main () {"
                    "  frag_colour = vec4 (colour,1.0);"
                    "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    //check shader error
    int params = -1;
    glGetShaderiv(vs, GL_COMPILE_STATUS ,&params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR GL shader index %i did not compile \n", vs);
        _print_shader_info_log(vs);
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);

    //
    glBindAttribLocation(shader_programme, 0, "vertex_position");
    glBindAttribLocation(shader_programme, 1, "vertex_colour");

    glLinkProgram(shader_programme);

    //check for shader linking errors
    params =-1;
    glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: could not link shader programmer GL index %u\n", shader_programme);
        _print_programme_info_log(shader_programme);
    }


    while (!glfwWindowShouldClose(window)) {

        //calculate movement


        _update_fps_count(window);
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // update other events like input handling
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, 1);
        }
    }

    glfwTerminate();
    cout << "Hello, World!" << endl;
    return 0;
}


