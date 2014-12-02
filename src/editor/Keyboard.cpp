/*
    This file is part of KSP1
    Copyright (C) 2014  Kushview, LLC. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "editor/Keyboard.h"
#include "editor/SamplerDisplay.h"
#include "editor/SamplerView.h"

namespace KSP1 {
namespace Gui {

bool KeyboardWidget::isInterestedInDragSource (const SourceDetails& details)
{
    return details.description.isArray() && details.description[0] == "files";
}

void KeyboardWidget::itemDragEnter (const SourceDetails& details)
{
    DragAndDropTarget::itemDragEnter (details);
}

void KeyboardWidget::itemDragMove (const SourceDetails& details)
{
    DragAndDropTarget::itemDragMove (details);
}

void KeyboardWidget::itemDragExit (const SourceDetails& details)
{
    DragAndDropTarget::itemDragExit (details);
}

void KeyboardWidget::itemDropped (const SourceDetails& details)
{
    SamplerView* view = findParentComponentOfClass<SamplerView>();
    SamplerDisplay* display = view->getDisplay();
    InstrumentPtr inst = display->getInstrument();
    int note (findNoteAt (details.localPosition));

    KeyItem key (inst->getOrCreateKey (note));
    if (key.isNotValid())
        return;

    DBG (key.node().toXmlString());

    key.clearLayers();

    for (int i = 1; i < details.description.size(); ++i)
    {
        const File file (details.description[i].toString());
        LayerItem layer (key.addLayer (file));
    }

    display->selectNote (note, true);
}

bool KeyboardWidget::shouldDrawDragImageWhenOver()
{
    return true;
}

}}
