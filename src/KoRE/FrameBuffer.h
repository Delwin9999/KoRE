/*
  Copyright (c) 2012 The KoRE Project

  This file is part of KoRE.

  KoRE is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  KoRE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KoRE.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SRC_KORE_FRAMEBUFFER_H_
#define SRC_KORE_FRAMEBUFFER_H_

<<<<<<< HEAD
=======
#include <string>

>>>>>>> hax
#include "KoRE/Log.h"
#include "KoRE/Common.h"
#include "KoRE/ShaderData.h"
#include "KoRE/Texture.h"
#include "KoRE/TextureSampler.h"
<<<<<<< HEAD
#include "KoRE/OperationOwner.h"

namespace kore {
  class FrameBuffer {
  public:
    FrameBuffer(void);
    virtual ~FrameBuffer(void);

    static const FrameBuffer BACKBUFFER;

    inline const GLuint getHandle() const {return _handle;}
    const TexturePtr getTexture(const std::string& name) const;
=======
#include "KoRE/BaseResource.h"

namespace kore {
  class FrameBuffer : public BaseResource {
  public:
    FrameBuffer(const std::string& name);
    virtual ~FrameBuffer(void);

    static const FrameBuffer* BACKBUFFER;

    inline const GLuint getHandle() const {return _handle;}
    const Texture* getTexture(const std::string& name) const;

    /*! \brief Change the name of the framebuffer. It is applied if the new 
    *          name does not exists. ResourceManager will be notified.
    *   \param name The new name.
    */
    void setName(const std::string& name);

    /// The name of a Framebuffer is unique
    inline const std::string getName() const {return _name;}
>>>>>>> hax

    /*! \brief Add a texture as an attatchment to the framebuffer.
    *   \param ptr The pointer to the texture to add as attatchment.
    *   \param attatchment The OpenGL attatchment-point to attatch.
<<<<<<< HEAD
                            the texture to (e.g. GL_COLOR_ATTATCHMENT0).
    */
    void addTextureAttachment(const TexturePtr& ptr, GLuint attatchment);
=======
    *                       the texture to (e.g. GL_COLOR_ATTATCHMENT0).
    */
    void addTextureAttachment(const Texture* ptr, GLuint attatchment);
>>>>>>> hax

    /*! \brief Request creation of a texture with the provided properties and
    *          attatch it to the FrameBuffer.
    *   \param properties The texture-properties to create the texture from.
    *   \param name       The name of the texture-attatchment and the texture.
    *   \param attatchment The OpenGL attatchment-point to attatch
<<<<<<< HEAD
                           the texture to (e.g. GL_COLOR_ATTATCHMENT0).
=======
    *                      the texture to (e.g. GL_COLOR_ATTATCHMENT0).
>>>>>>> hax
    */
    void addTextureAttachment(const STextureProperties& properties,
                              const std::string& name,
                              const GLuint attatchment);

    /*! \brief Checks this FBO for "Framebuffer completeness"
    *          The results of this check are written to the Log.
    * \return True, if this FBO succeeds the check, false otherwise */
    bool checkFBOcompleteness();

    /*! \brief Destroys this FrameBuffer internally. Deletes all Texture-
    *          Attatchments and sets the handle to "0". This FrameBuffer
    *          essentially becomes the BackBuffer. */
    void destroy();

  private:
    /// Internal constructor - used for creating a FrameBuffer with a
    /// specific handle (e.g. 0 for Backbuffer)
    FrameBuffer(GLuint handle);
<<<<<<< HEAD
    std::vector<ShaderData> _textureOutputs;
    std::vector<const TexturePtr> _textures;
    std::vector<STextureInfo*> _textureInfos;
    GLuint _handle;
  };
  typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;
=======
    std::string _name;
    std::vector<ShaderData> _textureOutputs;
    std::vector<const Texture*> _textures;
    std::vector<STextureInfo*> _textureInfos;
    GLuint _handle;
  };
>>>>>>> hax
};
#endif  // SRC_KORE_FRAMEBUFFER_H_
