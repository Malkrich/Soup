#pragma once

#include "Renderer/TextureCubemap.h"

namespace Soup
{

  class OpenGLTextureCubemap : public TextureCubemap
  {
  public:
    OpenGLTextureCubemap(const TextureCubemapSpecifications& specs);
    virtual ~OpenGLTextureCubemap();

    static Ref<OpenGLTextureCubemap> Copy(const TextureCubemapSpecifications& specs, uint32_t textureID);

    virtual uint32_t GetHandle() const override { return m_TextureID; }

    virtual uint32_t GetWidth() const override { return m_Specs.Width; }
    virtual uint32_t GetHeight() const override { return m_Specs.Height; }

    void Bind(uint32_t slot = 0) const;

    virtual const TextureCubemapSpecifications& GetSpecifications() const override { return m_Specs; }

    virtual void RegenerateMipmaps() override;

  private:
    void Invalidate();

  private:
    uint32_t m_TextureID = 0;

    TextureCubemapSpecifications m_Specs;
  };

}
