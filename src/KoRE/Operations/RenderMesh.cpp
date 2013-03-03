/*
  Copyright © 2012 The KoRE Project

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

#include "KoRE/Operations/RenderMesh.h"
#include "KoRE/RenderManager.h"
#include "KoRE/GLerror.h"
#include <vector>

kore::RenderMesh::RenderMesh(void)
  : _meshComponent(NULL),
    _camera(NULL),
    _shader(NULL),
    kore::Operation() {
      _type = OP_RENDERMESH;
}

kore::RenderMesh::RenderMesh(const kore::MeshComponentPtr& mesh,
                                 const kore::CameraPtr& camera,
                                 const kore::ShaderPtr& shader)
                                 : _meshComponent(NULL),
                                 _camera(NULL),
                                 _shader(NULL),
                                 kore::Operation() {
  _type = OP_RENDERMESH;
  connect(mesh, camera, shader);
}

kore::RenderMesh::~RenderMesh(void) {
}


void kore::RenderMesh::connect(const kore::MeshComponentPtr& mesh,
                               const kore::CameraPtr& camera,
                               const kore::ShaderPtr& shader) {
  _meshComponent = mesh;
  _shader = shader;
  _camera = camera;
}

void kore::RenderMesh::execute(void) {
    GLerror::gl_ErrorCheckStart();
    const std::vector<kore::ShaderInput>& vAttributes =
                                                    _shader->getAttributes();
    const MeshPtr mesh = _meshComponent->getMesh();

    if (mesh == NULL) {
      return;
    }

    _renderManager->useShaderProgram(_shader->getProgramLocation());
    _renderManager->bindVBO(mesh->getVBO());

    if (mesh->usesIBO()) {
      _renderManager->bindIBO(mesh->getIBO());
    } else {
      _renderManager->bindIBO(0);
    }

    // Indices but no IBO
    if (mesh->hasIndices() && !mesh->usesIBO()) {
      glDrawElements(mesh->getPrimitiveType(), mesh->getIndices().size(),
                     GL_UNSIGNED_INT, &mesh->getIndices()[0]);
    }

    // Indices with IBO
    else if (mesh->hasIndices() && mesh->usesIBO()) {
      glDrawElements(mesh->getPrimitiveType(), mesh->getIndices().size(),
                     GL_UNSIGNED_INT, KORE_BUFFER_OFFSET(0));
    }

    // No Indices
    else if (!mesh->hasIndices()) {
      glDrawArrays(mesh->getPrimitiveType(), 0,
                   mesh->getNumVertices());
    }

  GLerror::gl_ErrorCheckFinish("RenderMeshOperation " + mesh->getName());
}

void kore::RenderMesh::update(void) {
}

void kore::RenderMesh::reset(void) {
}

bool kore::RenderMesh::isValid(void) {
  return false;
}

const kore::MeshComponentPtr& kore::RenderMesh::getMesh() const {
    return _meshComponent;
}

void kore::RenderMesh::setMesh(const kore::MeshComponentPtr& mesh) {
    _meshComponent = mesh;
}

const kore::CameraPtr& kore::RenderMesh::getCamera() const {
    return _camera;
}

void kore::RenderMesh::setCamera(const kore::CameraPtr& camera) {
    _camera = camera;
}

const kore::ShaderPtr& kore::RenderMesh::getShader() const {
    return _shader;
}

void kore::RenderMesh::setShader(const kore::ShaderPtr& shader) {
    _shader = shader;
}

bool kore::RenderMesh::dependsOn(const void* thing) {
  return thing == _meshComponent.get() 
       || thing == _shader.get()
       || thing == _camera.get();
}
