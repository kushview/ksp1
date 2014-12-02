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

#ifndef KSP1_LAYERS_LISTBOX_H
#define KSP1_LAYERS_LISTBOX_H

#include "KSP1.h"
#include "Instrument.h"

namespace KSP1 {
namespace Gui {

    class LayerCell;

    class LayersListBox : public ListBox,
                          public ListBoxModel,
                          public DragAndDropTarget
    {
    public:
        typedef Signal Selected;

        LayersListBox();
        ~LayersListBox();

        void clearKey();
        void setKeyItem (const KeyItem& key);
        LayerItem getSelectedLayer() const;
        void updateLayers();

        void resized();

        // list box
        int getNumRows();
        void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
        void listBoxItemClicked (int row, const MouseEvent& e);
        void listBoxItemDoubleClicked (int row, const MouseEvent& e);
        Component* refreshComponentForRow (int row, bool selected, Component* existing);
        void backgroundClicked (const MouseEvent&) override;
        void selectedRowsChanged (int lastRowSelected);
        void deleteKeyPressed (int lastRowSelected);
        void returnKeyPressed (int lastRowSelected);
        void listWasScrolled();
        var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows);
        String getTooltipForRow (int row);

        void mouseWheelMove (const MouseEvent &, const MouseWheelDetails &);

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
}}

#endif  /* KSP1_LAYERS_LISTBOX_H */
