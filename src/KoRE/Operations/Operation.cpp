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

#include "KoRE/Operations/Operation.h"
#include "KoRE/RenderManager.h"

kore::Operation::Operation(void) : _type(OP_UNDEFINED) {
  _renderManager = RenderManager::getInstance();
}

kore::Operation::~Operation(void) {
//  _renderManager->removeOperation(this);
}

bool kore::Operation::isValid( void ) const {
  return _type != OP_UNDEFINED;
}

void kore::Operation::execute() const {
  if (!isValid()) {
    return;
  }

  // Call the dynamic implementation only if the Op is valid!
  doExecute();
}
