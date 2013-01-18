/*
  Copyright � 2012 The KoRE Project

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

#include <vector>

#include "core/rendermanager.h"
#include "core/log.h"

kore::RenderManager* kore::RenderManager::getInstance(void) {
  static kore::RenderManager theInstance;
  return &theInstance;
}

kore::RenderManager::RenderManager(void) {
}

kore::RenderManager::~RenderManager(void) {
}

void kore::RenderManager::renderMesh
(const std::shared_ptr<Mesh>& mesh,
const std::shared_ptr<Shader>& shader,
const std::shared_ptr<Camera>& camera) {
    const std::vector<kore::ShaderInput>& vAttributes = shader->getAttributes();
    for (unsigned int i = 0; i < vAttributes.size(); ++i) {
        const kore::ShaderInput& shaderAtt = vAttributes[i];
        const kore::MeshAttributeArray* meshAtt =
            mesh->getAttributeByName(shaderAtt.name);

        if (!meshAtt) {
            Log::getInstance()->write("[ERROR] Mesh %s does not have an"
                                      "Attribute %s",
                                      mesh->getName().c_str(),
                                      shaderAtt.name.c_str());
            return;
        }

        glEnableVertexAttribArray(shaderAtt.location);
        glVertexAttribPointer(shaderAtt.location, meshAtt->numComponents,
                              meshAtt->componentType, GL_FALSE,
                              0, meshAtt->data);
    }

    shader->applyShader();
    // Update uniforms
    GLint iView =
        glGetUniformLocation(shader->getProgramLocation(), "view");

    GLint iProj =
        glGetUniformLocation(shader->getProgramLocation(), "projection");

    GLint iModel =
        glGetUniformLocation(shader->getProgramLocation(), "model");

    glUniformMatrix4fv(iView, 1, GL_FALSE, glm::value_ptr(camera->getView()));

    glUniformMatrix4fv(iProj, 1, GL_FALSE,
                        glm::value_ptr(camera->getProjection()));

    glUniformMatrix4fv(iModel, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

if (mesh->hasIndices()) {
   glDrawElements(mesh->getPrimitiveType(), mesh->getIndices().size(),
       GL_UNSIGNED_INT, &mesh->getIndices()[0]);
} else {
     glDrawArrays(mesh->getPrimitiveType(), 0, mesh->getNumVertices());
 }
}

const glm::ivec2& kore::RenderManager::getRenderResolution() const {
    return _renderResolution;
}

void kore::RenderManager::
    setRenderResolution(const glm::ivec2& newResolution) {
    _renderResolution = newResolution;
    resolutionChanged();
}

void kore::RenderManager::renderFrame(void) {
  for (unsigned int i = 0; i < _operations.size(); i++) {
    _operations[i]->execute();
  }
}

void kore::RenderManager::resolutionChanged() {
    // Update all resolution-dependant resources here
    // (e.g. GBuffer-Textures...)
}
