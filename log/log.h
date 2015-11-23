//
// Created by alvaregd on 23/11/15.
//

#ifndef OPENGL_CONCEPTS_LOG_H
#define OPENGL_CONCEPTS_LOG_H


#define GL_LOG_FILE "/home/alvaregd/Documents/Games/tutorial/gl.log"

bool restart_gl_log();



bool gl_log_err(const char* message, ...);
bool gl_log (const char* message,...);
//program logs
void _print_shader_info_log(GLuint shader_index);

void _print_programme_info_log(GLuint program);
bool is_valid (GLuint programme);
void print_all(GLuint program);
void log_gl_params ();


#endif //OPENGL_CONCEPTS_LOG_H


