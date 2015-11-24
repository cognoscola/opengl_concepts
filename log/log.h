//
// Created by alvaregd on 23/11/15.
//

#ifndef OPENGL_CONCEPTS_LOG_H
#define OPENGL_CONCEPTS_LOG_H


#define GL_LOG_FILE "/home/alvaregd/Documents/Games/tutorial/gl.log"

extern  int width;
extern  int height;
extern  GLFWwindow *window;


bool restart_gl_log();
bool gl_log_err(const char* message, ...);
bool gl_log (const char* message,...);
bool start_gl();
void _print_shader_info_log(GLuint shader_index);
void _print_programme_info_log(GLuint program);
bool is_valid (GLuint programme);
void print_all(GLuint program);
void log_gl_params ();
void _update_fps_counter (GLFWwindow* window);

//glfw callbacks
void glfw_error_callback(int error, const char* description);
void glfw_window_size_callback(GLFWwindow* window, int width, int height);

GLuint create_programme_from_files (
        const char* vert_file_name, const char* frag_file_name
);

#endif //OPENGL_CONCEPTS_LOG_H


