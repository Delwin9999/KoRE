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

#ifndef SRC_KOREGUI_RESOURCEVIEWER_H_
#define SRC_KOREGUI_RESOURCEVIEWER_H_

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>

namespace koregui {
  class ResourceViewer : public QWidget {
      Q_OBJECT

  enum Mode {
    MODE_SELECT,
    MODE_LOAD
    };
  public:
    ResourceViewer(QWidget *parent = 0);
    ~ResourceViewer();

    void update(void);
    void refresh(void);

  public slots:
    void useBrowser(void);

  protected:
    void keyPressEvent(QKeyEvent* event);

  private:
    Mode _currentmode;
    QVBoxLayout _layout;
    QTabWidget  _tabs;
    QPushButton _addButton;
    QListWidget _meshes;
    QListWidget _textures;
    QListWidget _programs;
    QListWidget _framebuffers;
  };
}
#endif // SRC_KOREGUI_RESOURCEVIEWER_H_
