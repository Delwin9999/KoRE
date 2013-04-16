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

#ifndef SHADERINPUTITEM_H_
#define SHADERINPUTITEM_H_

#include <QGraphicsItem>
#include "KoRE/ShaderInput.h"
<<<<<<< HEAD
#include "KoRE/Operations/BindOperations/BindOperation.h"

namespace koregui {
=======

namespace koregui {
  class BindPathItem;
>>>>>>> hax
  class ShaderInputItem : public QGraphicsItem {
  public:
    ShaderInputItem(const kore::ShaderInput* input, QGraphicsItem* parent = 0);
    ~ShaderInputItem(void);
<<<<<<< HEAD
    inline const kore::ShaderInput* getInput(void) {return _input;};
    void refresh(void);

  protected:
=======
    inline const kore::ShaderInput* getInput(void) {return _input;}
    bool checkInput(BindPathItem* binding);
    void setBinding(BindPathItem* binding);
    void reset();

  protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

>>>>>>> hax
    QRectF boundingRect() const;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);

  private:
    const kore::ShaderInput* _input;
<<<<<<< HEAD
    kore::BindOperation* _op;
=======
    BindPathItem* _binding;
    bool _mouseover;
>>>>>>> hax
  };
}
#endif  // SHADERINPUTITEM_H_
