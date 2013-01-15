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

#ifndef INCLUDE_CORE_MESH_H_
#define INCLUDE_CORE_MESH_H_

#include <string>
#include <vector>
#include "core/datatypes.h"

namespace kore {
  struct Attribute {
    std::string name;
    InputType type;
    int size;
    void* data;
  };
  class Mesh {
    friend class MeshLoader;

  public:
    Mesh(void);
    virtual ~Mesh(void);
    bool loadFromFile(const std::string& file);
    int getNumAttributes(void);
    std::string getAttributeName(int attribute_index);
    InputType getAttributeType(int attribute_index);
    int getAttributeSize(int attribute_index);
    void* getAttributeData(int attribute_index);
  private:
    std::vector<Attribute> _attributes;
  };
};
#endif  // INCLUDE_CORE_MESH_H_
