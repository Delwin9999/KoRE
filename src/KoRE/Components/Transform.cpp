#include "KoRE/Components/Transform.h"

kore::Transform::Transform(void) : _global(glm::mat4(1.0f)),
                                   _local(glm::mat4(1.0f)),
                                   kore::SceneNodeComponent() {
  ShaderData input = ShaderData();
  input.type = GL_FLOAT_MAT4;
  input.name = "model Matrix";
  input.data = glm::value_ptr(_global);
  _shaderData.push_back(input);

  input = ShaderData();
  input.type = GL_FLOAT_MAT4;
  input.name = "object Matrix";
  input.data = glm::value_ptr(_local);
  _shaderData.push_back(input);

  _type = COMPONENT_TRANSFORM;
}

kore::Transform::~Transform() {
}

bool kore::Transform::
isCompatibleWith(const SceneNodeComponent& otherComponent) const {
  return true;
}

void kore::Transform::setGlobal(const glm::mat4& global) {
  _global = global;
}

void kore::Transform::setLocal(const glm::mat4& local) {
  _local = local;
}
