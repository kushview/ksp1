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

#pragma once

#include "KSP1.h"
#include "Instrument.h"

namespace KSP1 {

    class LayerCell;

    class LayersListBox : public ListBox,
                          public ListBoxModel,
                          public DragAndDropTarget
    {
    public:
        typedef boost::signals2::signal<void()> Selected;

        LayersListBox();
        ~LayersListBox();

        void clearKey();
        void setKeyItem (const KeyItem& key);
        LayerItem getSelectedLayer() const;
        void updateLayers();

        void resized() override;

        // list box
        int getNumRows() override;
        void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;
        void listBoxItemClicked (int row, const MouseEvent& e) override;
        void listBoxItemDoubleClicked (int row, const MouseEvent& e) override;
        Component* refreshComponentForRow (int row, bool selected, Component* existing) override;
        void backgroundClicked (const MouseEvent&) override;
        void selectedRowsChanged (int lastRowSelected) override;
        void deleteKeyPressed (int lastRowSelected) override;
        void returnKeyPressed (int lastRowSelected) override;
        void listWasScrolled() override;
        var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows) override;
        String getTooltipForRow (int row) override;

        void mouseWheelMove (const MouseEvent &, const MouseWheelDetails &) override;

        Selected& signalSelected() { return notifyChosen; }

        bool isInterestedInDragSource (const SourceDetails& dragSourceDetails) override;
        void itemDragEnter (const SourceDetails& dragSourceDetails) override;
        void itemDragMove (const SourceDetails& dragSourceDetails) override;
        void itemDragExit (const SourceDetails& dragSourceDetails) override;
        void itemDropped (const SourceDetails& dragSourceDetails) override;
        bool shouldDrawDragImageWhenOver() override;

    private:
        Selected notifyChosen;
        KeyItem keyItem;
        LayerCell* cellForRow (int row);
        friend class LayerCell;
    };
}
