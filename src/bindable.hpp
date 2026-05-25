#pragma once
#include <glad/glad.h>

class Bindable {
public:
    virtual void bind(GLuint program) = 0;
};
