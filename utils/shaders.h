//
// Created by alvaregd on 24/11/15.
//

#ifndef OPENGL_CONCEPTS_SHADERS_H
#define OPENGL_CONCEPTS_SHADERS_H

#include <GL/glew.h>
#include <sys/types.h>

#define WORKSPACE_DIR "/home/alvaregd/Documents/Games/tutorial/"

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
void bindUniforms(GLuint program, uint* index, char* var_name, char * var_name2);
void createProgram(GLuint programID);

#endif //OPENGL_CONCEPTS_SHADERS_H
