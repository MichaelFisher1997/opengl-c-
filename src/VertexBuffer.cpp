#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER,
                      m_RendererID)); // select buffer called 'buffer'
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data,
                      GL_STATIC_DRAW)); // assigne buffer size, static as we use
                                        // many times, but does not change
}

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

void VertexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER,
                      m_RendererID)); // select buffer called 'buffer'
}

void VertexBuffer::Unbind() const {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // select buffer called 'buffer'
}
