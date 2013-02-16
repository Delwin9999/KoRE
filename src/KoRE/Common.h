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
#ifndef CORE_INCLUDE_CORE_COMMON_H_
#define CORE_INCLUDE_CORE_COMMON_H_

//# define GLCOREARB_PROTOTYPES 1
//# define GL_GLEXT_PROTOTYPES 1
//#include <GL/glcorearb.h>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <memory>


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "KoRE/Log.h"

// Typedefs:
typedef unsigned int uint;
//typedef uint64_t uint64;
typedef long uint64;
typedef glm::half float16;
typedef double float64;

// Use this to indicate an invalid GL-handle of type GLuint
#define GLUINT_HANDLE_INVALID 0xFFFFFFFF
#define GLINT_HANDLE_INVALID -1
#define UINT_INVALID 0xFFFFFFFF
#define UINT64_INVALID 0xFFFFFFFFFFFFFFFF
#define TAG_INVALID 0x00000000

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define SAFE_DELETE_ARR(p) if (p[ 0 ]) delete[] p;
#define BUFFER_OFFSET(i) (reinterpret_cast<char*>(NULL + (i)))
//#define STATIC_ASSERT(expression) {int __static_assert_checkArray[1]; __static_assert_checkArray[expression];}

#endif  // CORE_INCLUDE_CORE_COMMON_H_
