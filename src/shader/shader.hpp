#pragma once
#include <glad/glad.h>

class Shader {
public:
    Shader(GLuint program_id);
    GLuint get_id();
    void use();
private:
    GLuint id;
};
