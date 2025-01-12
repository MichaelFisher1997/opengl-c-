#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"
struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized)
        : type(type), count(count), normalized(normalized) {}

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return sizeof(GLfloat);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }
        return 0;
    }
};

class VertexBufferLayout {
public:
    VertexBufferLayout() : m_Stride(0) {}

    template<typename T>
    void Push(int count);

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }

private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};

// Explicit specializations
template<>
inline void VertexBufferLayout::Push<float>(int count) {
    m_Elements.push_back({ GL_FLOAT, static_cast<unsigned int>(count), GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(int count) {
    m_Elements.push_back({ GL_UNSIGNED_INT,static_cast<unsigned int>(count), GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(int count) {
    m_Elements.push_back({ GL_UNSIGNED_BYTE, static_cast<unsigned int>(count), GL_TRUE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
#endif // VERTEXBUFFERLAYOUT_H
