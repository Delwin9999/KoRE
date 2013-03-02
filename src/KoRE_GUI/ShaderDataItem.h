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

/************************************************************************/
/* \author Dominik Ospelt                                               */
/************************************************************************/

#ifndef SHADERDATAITEM_H_
#define SHADERDATAITEM_H_

#include <QGraphicsItem>
#include "KoRE/ShaderData.h"

namespace koregui {
  class ShaderDataItem : public QGraphicsItem {
  public:
    ShaderDataItem(const kore::ShaderData* data, QGraphicsItem* parent = 0);
    ~ShaderDataItem(void);
    inline const kore::ShaderData* getData(void) {return _data;};
    void refresh(void);

  protected:
    QRectF boundingRect() const;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);

  private:
    const kore::ShaderData* _data;
  };
}
#endif  // SHADERDATAITEM_H_
