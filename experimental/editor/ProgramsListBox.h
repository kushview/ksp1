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

#ifndef KSP1_PROGRAMS_LISTBOX_H
#define KSP1_PROGRAMS_LISTBOX_H

#include "KSP1.h"
#include "Programming.h"

namespace KSP1 {

    class ProgramsListBox: public ListBox,
            public ListBoxModel
    {
    public:

        typedef boost::signals2::signal<void(const Programming::Item&)> Engaged;

        ProgramsListBox (Programming&);
        ~ProgramsListBox();
        int getNumRows() override;
        void paintListBoxItem (int rowNumber, Graphics& g,
                               int width, int height, bool rowIsSelected) override;

        Component* refreshComponentForRow (int rowNumber, bool isRowSelected,
                                           Component* existingComponentToUpdate) override;
        void listBoxItemClicked (int row, const MouseEvent& e) override;
        void listBoxItemDoubleClicked (int row, const MouseEvent& e) override;
        void backgroundClicked (const MouseEvent&) override;
        void selectedRowsChanged (int lastRowSelected) override;
        void deleteKeyPressed (int lastRowSelected) override;
        void returnKeyPressed (int lastRowSelected) override;
        void listWasScrolled() override;
        var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows) override;
        String getTooltipForRow (int row) override;

        Programming::Item selectedItem() const;
        Engaged& signalEngaged() { return notifyEngaged; }

    private:
        Programming& progs;
        int totalRows;

        Programming::Item item;

        Engaged notifyEngaged;

        void buildRows();
        Programming::Item getItemForRow (int row) const;
    };
}

#endif // KSP1_PROGRAMS_LISTBOX_H
