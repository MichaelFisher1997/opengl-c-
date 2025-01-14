#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
    : m_Count(count) {
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));
  //
  // std::cout << "m_RendererID: " << &m_RendererID << " " << m_RendererID <<
  // std::endl;
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                      m_RendererID)); // select buffer called 'buffer'
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                      data,
                      GL_STATIC_DRAW)); // assigne buffer size, static as we use
                                        // many times, but does not change
}

// Destructor
IndexBuffer::~IndexBuffer() {
  if (m_RendererID != 0) { // Only delete if valid
    GLCall(glDeleteBuffers(1, &m_RendererID));
    std::cout << "IndexBuffer destroyed with RendererID: " << m_RendererID
              << std::endl;
  }
}

// Move Constructor
IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept
    : m_RendererID(other.m_RendererID), m_Count(other.m_Count) {
  other.m_RendererID = 0; // Invalidate the moved-from object
  other.m_Count = 0;
  std::cout << "IndexBuffer moved (constructor)" << std::endl;
}

// Move Assignment Operator
IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept {
  if (this != &other) {
    // Free existing resources
    if (m_RendererID != 0) {
      GLCall(glDeleteBuffers(1, &m_RendererID));
    }

    // Transfer ownership
    m_RendererID = other.m_RendererID;
    m_Count = other.m_Count;

    // Invalidate the moved-from object
    other.m_RendererID = 0;
    other.m_Count = 0;

    std::cout << "IndexBuffer moved (assignment)" << std::endl;
  }
  return *this;
}

void IndexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                      m_RendererID)); // select buffer called 'buffer'
}

void IndexBuffer::Unbind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                      0)); // select buffer called 'buffer'
}
