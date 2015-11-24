#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <assert.h>
#include <cmath>
#include <utils/maths_funcs.h>
#include <utils/shaders.h>
#include <string.h>

#include "log/log.h"

using namespace std;

// keep track of window size for things like the viewport and the mouse cursor
int width = 640;
int height = 480;
GLFWwindow* window = NULL;

struct Camera{

    float speed = 1.0f;
    float yaw_speed = 10.0f;
    float pos[3] = {0.0f, 0.0f, 2.0f}; //don't start at zero or camera will be too closes
    float yaw =0.0f; //y-rotation in deg

};

static double elapsed_seconds;
static Camera camera;

int main() {

    assert(restart_gl_log());

    assert(start_gl());

    //only draw if shape is close to the viewer
    glEnable(GL_DEPTH_TEST); //enable depth testing
    glDepthFunc(GL_LESS); //smaller value is closer

    GLfloat points[] = {
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
    };

    GLfloat normals[] = {
            0.0f, 0.0f,  1.0f,
            0.0f, 0.0f,  1.0f,
            0.0f, 0.0f,  1.0f
    };

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint normal_vbo = 0;
    glGenBuffers(1, &normal_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    char file_vertex[100];
    strcpy(file_vertex, WORKSPACE_DIR);
    strcat(file_vertex, "shaders/test.vert");

    char file_fragment[100];
    strcpy(file_fragment, WORKSPACE_DIR);
    strcat(file_fragment, "shaders/test.frag");

    GLuint shader_programme = LoadShaders(file_vertex,file_fragment);
    GLuint uniformIndex[2];
    uniformIndex[0] = 0;
    uniformIndex[1] = 1;
    bindUniforms(shader_programme, uniformIndex, "vertex_position", "vertex_normal");
    createProgram(shader_programme);

    float near = 0.1f; //clipping plane
    float far = 100.f; //clipping plane
    float fov = 67.0f * ONE_DEG_IN_RAD;
    float aspect = (float) width/ (float) height;

    //creating a projection matrix
    float range = tan(fov * 0.5f) * near;
    float Sx = (2.0f * near) / (range * aspect + range * aspect);
    float Sy = near / range;
    float Sz = -(far + near) / (far - near);
    float Pz = -(2.0f * far * near) / (far - near);

    float proj_mat[] = {
            Sx, 0.0f, 0.0f, 0.0f,
            0.0f, Sy, 0.0f, 0.0f,
            0.0f, 0.0f, Sz, -1.0f,
            0.0f, 0.0f, Pz, 0.0f
    };


    //create the view matrix
    camera = {};
    camera.pos[0] = camera.pos[1]  = 0.0f;
    camera.pos[2] = 0.5f;

    mat4 T = translate(identity_mat4(), vec3(-camera.pos[0], -camera.pos[1], -camera.pos[2]));
    mat4 R = rotate_y_deg(identity_mat4(), -camera.yaw);
    mat4 view_mat = R * T;

    mat4 model_mat = identity_mat4();

    glUseProgram(shader_programme);
    int view_mat_location = glGetUniformLocation(shader_programme, "view_mat");
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
    int proj_mat_location = glGetUniformLocation(shader_programme, "projection_mat");
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat);
    int model_mat_location = glGetUniformLocation(shader_programme, "model_mat");
    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat.m);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    while (!glfwWindowShouldClose(window)) {
        _update_fps_counter(window);

        double current_seconds = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width, height);

        glUseProgram(shader_programme);

        model_mat.m[12] = sinf(current_seconds);
        glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat.m);

        glBindVertexArray(vao);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // update other events like input handling

        glfwPollEvents();
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, 1);
        }

        glfwSwapBuffers(window);

        elapsed_seconds = elapsed_seconds/10;
        bool cam_moved = false;
        if (glfwGetKey (window, GLFW_KEY_A)) {
            camera.pos[0] -= camera.speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_D)) {
            camera.pos[0] += camera.speed* elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_PAGE_UP)) {
            camera.pos[1] += camera.speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_PAGE_DOWN)) {
            camera.pos[1] -= camera.speed* elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_W)) {
            camera.pos[2] -= camera.speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_S)) {
            camera.pos[2] += camera.speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_LEFT)) {
            camera.yaw += camera.yaw_speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_RIGHT)) {
            camera.yaw -= camera.yaw_speed* elapsed_seconds;
            cam_moved = true;
        }
// update view matrix
        if (cam_moved) {
            mat4 T = translate (identity_mat4 (), vec3 (-camera.pos[0], -camera.pos[1], -camera.pos[2])); // cam translation
            mat4 R = rotate_y_deg (identity_mat4 (), -camera.yaw); //
            mat4 view_mat = R * T;
            glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
        }
    }


    glDeleteBuffers(1, &normal_vbo);
    glDeleteBuffers(1, &points_vbo);
    glDeleteProgram(shader_programme);
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    cout << "Cleaning up" << endl;
    return 0;
}


