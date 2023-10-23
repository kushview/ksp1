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

#ifndef KSP1_BANKS_H
#define KSP1_BANKS_H

#include "../Programming.h"

namespace KSP1 {

class BankList : public Component,
                 public ListBoxModel {
    Programming& progs;

public:
    BankList (Programming& programs)
        : progs (programs) {
        addAndMakeVisible (&listbox);
        listbox.setModel (this);
        listbox.updateContent();
    }

    ~BankList() {
    }

    inline int
        getNumRows() {
        return progs.getNumBanks();
    }

    inline void
        paintListBoxItem (int rowNumber, Graphics& g,
                          int width, int height,
                          bool rowIsSelected) {
        if (rowIsSelected) {
            g.setColour (Colours::black.withAlpha (0.8f));

        } else
            g.setColour (Colours::black.withAlpha (1.0f));

        g.fillRect (0, 0, width, height);

        g.setColour (Colours::whitesmoke);
        g.drawText (progs.getBankName (rowNumber),
                    2,
                    0,
                    width - 4,
                    height,
                    Justification::centredLeft,
                    true);
    }

    inline void
        listBoxItemClicked (int row, const MouseEvent& e) {
    }

    inline void
        listBoxItemDoubleClicked (int row, const MouseEvent& e) {
    }

#if 0
Component* refreshComponentForRow (int rowNumber, bool isRowSelected,
        Component* existingComponentToUpdate);

void backgroundClicked() { }

void selectedRowsChanged (int lastRowSelected);

void deleteKeyPressed (int lastRowSelected);

void returnKeyPressed (int lastRowSelected);

void listWasScrolled();

var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows);

String getTooltipForRow (int row);
#endif

    inline void
        resized() {
        listbox.setBoundsInset (BorderSize<int> (8));
    }

private:
    ListBox listbox;
};

class BankCombo : public ComboBox {
public:
    BankCombo (KSP1::Programming& programs) {
        reset (programs);
    }

    inline void
        reset (KSP1::Programming& programs) {
        clear (sendNotification);

        int selected = getSelectedId();

        for (int i = 0; i < programs.getNumBanks(); ++i)
            addItem (programs.getBankName (i), i + 1);

        if (this->indexOfItemId (selected) >= 0)
            setSelectedId (selected, dontSendNotification);
        else
            setSelectedId (0, dontSendNotification);
    }

private:
};
} // namespace KSP1

#endif /* KSP1_BANKS_H */
