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

#ifndef FRAMEBUFFERSTAGEITEM_H_
#define FRAMEBUFFERSTAGEITEM_H_

#include <vector>
#include <QGraphicsItem>
#include <QContextMenuEvent>
#include "KoRE/Passes/FrameBufferStage.h"
#include "KoRE/FrameBuffer.h"
#include "KoRE_GUI/ShaderPassItem.h"

namespace koregui {
  class FrameBufferStageItem : public QGraphicsItem {
  public:
    FrameBufferStageItem(QGraphicsItem* parent = 0);
    ~FrameBufferStageItem(void);

    void refresh(void);
    QRectF boundingRect() const;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    inline int getHeight(void) {return _bufferheight;};
    inline int getWidth(void) {return _bufferwidth;};
    inline kore::FrameBuffer* getFrameBuffer(void) {return _frameBuffer;};
    inline kore::FrameBufferStage* getStage(void) {return _bufferstage;};

    void setFrameBuffer(kore::FrameBuffer* framebuffer);

    void addShaderPass(ShaderPassItem* pass);
    void removeShaderPass(ShaderPassItem* pass);
    void shaderMoved(ShaderPassItem* pass);

  private:
    std::string _name;
    kore::FrameBuffer* _frameBuffer;
    kore::FrameBufferStage* _bufferstage;
    uint _bufferheight;
    uint _bufferwidth;

    std::vector<ShaderPassItem*> _programs;
  };
}
#endif  // FRAMEBUFFERSTAGEITEM_H_
