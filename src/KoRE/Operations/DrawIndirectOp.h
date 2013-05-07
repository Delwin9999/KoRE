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

#ifndef KORE_SRC_KORE_OPERATIONS_DRAWINDIRECTOP_H_
#define KORE_SRC_KORE_OPERATIONS_DRAWINDIRECTOP_H_

#include "KoRE/Common.h"
#include "KoRE/Operations/Operation.h"

namespace kore {
  class DrawIndirectOp : public Operation {
  public:
    DrawIndirectOp();
    explicit DrawIndirectOp(const GLenum mode, const GLuint bufOffset);
    virtual ~DrawIndirectOp();

    void connect(const GLenum mode, const GLuint bufOffset);

    virtual void update(void);
    virtual void reset(void);
    virtual bool isValid() const;
    virtual bool dependsOn(const void* thing) const;

  private:
    GLenum _mode;
    GLuint _bufOffset;

    virtual void doExecute() const;
  };
}

#endif  // KORE_SRC_KORE_OPERATIONS_DRAWINDIRECTOP_H_
