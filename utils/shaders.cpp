//
// Created by alvaregd on 24/11/15.
//

#include "shaders.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//todo build a structure to hold stuff
static GLuint vertexShaderID;
static GLuint fragmentShaderID;
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

    //a place to hold the fragments
     vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
     fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);


    //load the vertex shader
    string vertexShaderCode;
    ifstream vertexShaderStream(vertex_file_path, ios::in);
    if (vertexShaderStream.is_open()) {
        string line = "";
        while (getline(vertexShaderStream, line)) {
            vertexShaderCode += "\n" + line;
        }
        vertexShaderStream.close();
    }else {
        printf("Impossible to open %s. Check directory ", vertex_file_path);
        getchar();
        return 0;
    }

    //load the fragment file
    string fragmentShaderCode;
    ifstream fragmentShaderStream(fragment_file_path, ios::in);
    if (fragmentShaderStream.is_open()) {
        string line = "";
        while (getline(fragmentShaderStream, line)) {
            fragmentShaderCode += "\n" + line;
        }
        fragmentShaderStream.close();
    }


    GLint result = GL_FALSE;
    int infoLogLength;

    //compile the vertex shader
    printf("Compiling Shader: %s\n", vertex_file_path);
    char const *vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
    glCompileShader(vertexShaderID);

    //check vertex shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL,&vertexShaderErrorMessage[0]);
        printf("%s\n", & vertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(fragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();

    return ProgramID;
}

void bindUniforms(GLuint program, uint* index, char* var_name, char * var_name2){

//    for (int i = 0; i < count; ++i) {
        glBindAttribLocation(program, index[0], var_name);
        glBindAttribLocation(program, index[1], var_name2);
//    }
}

void createProgram(GLuint programID){

    GLint result;
    int infoLogLength;

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(infoLogLength+1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

}