#pragma once

namespace Soup
{
  enum class BufferElementType
  {
    Bool = 0,
    Int,
    Int2,
    Int3,
    Int4,
    Float,
    Float2,
    Float3,
    Float4,
    Mat2,
    Mat3,
    Mat4
  };

  struct BufferElement
  {
    BufferElement(const std::string& name, BufferElementType type)
      : Name(name)
      , Type(type)
    {
    }

    std::string Name;
    BufferElementType Type;
    bool Normalized = false;
    uint32_t Offset = 0;
  };

  class Layout
  {
  public:
    Layout() = default;
    Layout(const std::initializer_list<BufferElement>& elements);

    bool IsEmpty() const { return m_Elements.empty(); }

    uint32_t GetStride() const { return m_Stride; }

    std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

  private:
    void ComputeOffsetAndStride();

  private:
    uint32_t m_Stride = 0;
    std::vector<BufferElement> m_Elements;
  };

  class VertexBuffer
  {
  public:
    static Ref<VertexBuffer> Create(uint32_t size, const void* data);
    static Ref<VertexBuffer> Create(uint32_t size);
    static Ref<VertexBuffer> Create() { return Create(0); }

    virtual ~VertexBuffer() = default;

    virtual void SetData(uint32_t size, const void* data) = 0;

    virtual const Layout& GetGeometryLayout() const = 0;
    virtual void SetGeometryLayout(const Layout& layout) = 0;
    virtual const Layout& GetInstanceLayout() const = 0;
    virtual void SetInstanceLayout(const Layout& layout) = 0;
  };

}
