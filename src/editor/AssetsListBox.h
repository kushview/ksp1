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

#ifndef KSP1_ASSETS_LIST_BOX_H
#define KSP1_ASSETS_LIST_BOX_H

#include "KSP1.h"

namespace KSP1 {
namespace Gui {

    class AssetsListBoxHeader;

    class AssetsListBox : public ListBoxModel,
                          public ListBox
                          //public DragAndDropContainer
    {
    public:

        typedef boost::signals2::signal<void(const AssetItem&)> AssetSignal;
        AssetSignal signalSelected;

        AssetsListBox();
        ~AssetsListBox();

        void setRootItem (const AssetItem& asset);
        AssetItem getAsset (int32 row) const;
        AssetItem getSelectedAsset() const;

        bool keyPressed (const KeyPress& key);

        // ListBox / ListBoxModel
        int getNumRows();
        void paintListBoxItem (int row, Graphics& g, int w, int h, bool selected);
        Component* refreshComponentForRow (int row, bool selected, Component* existing);
        void listBoxItemClicked (int row, const MouseEvent& e);
        void listBoxItemDoubleClicked (int row, const MouseEvent& e);
        void backgroundClicked (const MouseEvent&);
        void selectedRowsChanged (int lastRowSelected);
        void deleteKeyPressed (int lastRowSelected);
        void returnKeyPressed (int lastRowSelected);
        void listWasScrolled();
        var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows);
        String getTooltipForRow (int row);
        MouseCursor getMouseCursorForRow (int row);

    private:
        Scoped<AssetItem> root;
        Scoped<AssetsListBoxHeader> header;
        void setResults (const ValueTree& newData);
        ValueTree results;

        Element::Icon getIcon() const;
        bool isRootItemValid() const { return root != nullptr && root->isValid(); }

        void handleAssetSelected (const AssetItem& item);

        friend class AssetsListBoxHeader;
    };

}}

#endif /* KSP1_ASSETS_LIST_BOX_H */
