#pragma once

#include <GL/glew.h>

namespace graphics::opengl {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        void enableAttrib(GLint id);
        void bind();
        void unbind();

    private:
        GLuint m_id;
    };
}