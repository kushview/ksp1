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

#include "editor/ProgramsListBox.h"
#include "Filesystem.h"

namespace KSP1 {

ProgramsListBox::ProgramsListBox (Programming& p)
    : progs (p), item (p, p.getEntries()) {
    setModel (this);

    setSize (120, 400);
    buildRows();
}

ProgramsListBox::~ProgramsListBox() {
    setModel (nullptr);
}

void ProgramsListBox::buildRows() {
    updateContent();
    repaint();
}

int ProgramsListBox::getNumRows() {
    return item.isRoot() ? item.numChildren() : item.numChildren() + 1;
}

Programming::Item
    ProgramsListBox::getItemForRow (int row) const {
    Programming::Item entry (progs);

    if (! item.isRoot() && row == 0) {
        entry = item.parent();
    } else if (! item.isRoot()) {
        entry = item.getEntry (row - 1);
    } else {
        entry = item.getEntry (row);
    }

    return entry;
}

void ProgramsListBox::paintListBoxItem (int row, Graphics& g,
                                        int w, int h, bool selected) {
    Programming::Item entry (getItemForRow (row));

    const int fontSize = 14;
    int indentPix      = 6;

    if (selected) {
        g.setColour (Colours::darkorange);
        g.fillAll();
    }

    g.setColour (Colours::whitesmoke);

    if (entry.isGroup()) {
        g.setFont (Font (fontSize, Font::plain));
    } else {
        g.setFont (Font (fontSize, Font::plain));
        indentPix = 12;
    }

    g.drawText (entry.name(), indentPix, 0, w - indentPix, h, Justification::left, true);
}

Programming::Item
    ProgramsListBox::selectedItem() const {
    return getItemForRow (getSelectedRow());
}

Component*
    ProgramsListBox::refreshComponentForRow (int r, bool s, Component* c) {
    return ListBoxModel::refreshComponentForRow (r, s, c);
}

void ProgramsListBox::listBoxItemClicked (int row, const MouseEvent& e) {
}

void ProgramsListBox::listBoxItemDoubleClicked (int row, const MouseEvent& e) {
    Programming::Item entry = getItemForRow (row);
    if (entry.isGroup()) {
        item = entry;
        buildRows();
    } else if (entry.isValid()) {
        notifyEngaged (entry);
    }
}

void ProgramsListBox::backgroundClicked (const MouseEvent&) {}
void ProgramsListBox::selectedRowsChanged (int lastRowSelected) {}
void ProgramsListBox::deleteKeyPressed (int lastRowSelected) {}
void ProgramsListBox::returnKeyPressed (int lastRowSelected) {}
void ProgramsListBox::listWasScrolled() {}

var ProgramsListBox::getDragSourceDescription (const SparseSet<int>& selectedRows) {
    Programming::Item entry (getItemForRow (selectedRows[0]));
    return entry.asFile().getFullPathName();
}

String ProgramsListBox::getTooltipForRow (int row) { return String::empty; }

} // namespace KSP1
