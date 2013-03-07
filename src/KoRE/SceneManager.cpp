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

#include <vector>
#include <string>
#include <utility>
#include "KoRE/SceneManager.h"

kore::SceneManager* kore::SceneManager::getInstance(void) {
  static kore::SceneManager theInstance;
  return &theInstance;
}

kore::SceneManager::SceneManager(void)
                           :_idcount(0),
                            _tagcount(0) {
  addTag("DEFAULT");
  _root = new SceneNode();
  _root->setName("ROOT");
}

kore::SceneManager::~SceneManager(void) {
}

uint64 kore::SceneManager::createID(void) {
  return _idcount++;
}

void kore::SceneManager::update(void) {
  _root->update();
}

void kore::SceneManager::addTag(const std::string& name) {
  if (_tagmap.find(name) != _tagmap.end()) {
    _tagmap.insert(std::pair<std::string, uint>(name, _tagcount++));
  }
}

const uint kore::SceneManager::getTag(const std::string& name) {
  return (_tagmap.find(name) !=
    _tagmap.end())?_tagmap.find(name)->second:KORE_TAG_INVALID;
}

void kore::SceneManager::
  getSceneNodesByTag(const uint tag,
                     std::vector<SceneNode*>& vSceneNodes) {
  _root->getSceneNodesByTag(tag, vSceneNodes);
}

void kore::SceneManager::getSceneNodesByTag(const std::string& name,
                                     std::vector<SceneNode*>& vSceneNodes) {
  const uint uTag = getTag(name);
  if (uTag != KORE_TAG_INVALID) {
      getSceneNodesByTag(uTag, vSceneNodes);
  }
}

void kore::SceneManager::
  getSceneNodesByName(const std::string& name,
                      std::vector<SceneNode*>& vSceneNodes) {
  _root->getSceneNodesByName(name, vSceneNodes);
}

void kore::SceneManager::
getSceneNodesByComponent(const EComponentType componentType,
                           std::vector<SceneNode*>& vSceneNodes) {
  _root->getSceneNodesByComponent(componentType, vSceneNodes);
}

kore::SceneNode* kore::SceneManager::
  getSceneNodeByComponent(const EComponentType componentType) {
    std::vector<SceneNode*> vNodes;
    getSceneNodesByComponent(componentType, vNodes);
    if (vNodes.size() > 0) {
      return vNodes[0];
    }
    return NULL;
}

kore::SceneNode* kore::SceneManager::getRootNode() {
  return _root;
}
