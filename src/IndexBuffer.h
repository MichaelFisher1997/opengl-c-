#pragma once

class IndexBuffer 
{
public:
  IndexBuffer(const unsigned int* data, unsigned int count);
  ~IndexBuffer();

  // Disallow copy operations
  IndexBuffer(const IndexBuffer&) = delete;
  IndexBuffer& operator=(const IndexBuffer&) = delete;

  // Enable move operations
  IndexBuffer(IndexBuffer&& other) noexcept;
  IndexBuffer& operator=(IndexBuffer&& other) noexcept;

  void Bind() const;
  void Unbind() const;

  inline unsigned int GetCount() const { return m_Count; }
private:
    unsigned int m_RendererID = 0;
    unsigned int m_Count = 0;
};
