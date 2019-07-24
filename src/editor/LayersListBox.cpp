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

#include "editor/LayerCell.h"
#include "editor/LayersListBox.h"
#include "editor/SamplerDisplay.h"
#include "editor/SamplerView.h"


namespace KSP1 {

    LayersListBox::LayersListBox()
    {
        setModel (this);
        updateContent();
    }

    LayersListBox::~LayersListBox()
    {
        setModel (nullptr);
    }

    void LayersListBox::resized()
    {
        ListBox::resized();
    }

    LayerItem LayersListBox::getSelectedLayer() const
    {
        return keyItem.getLayer (getSelectedRow());
    }

    int LayersListBox::getNumRows()
    {
        const int count = keyItem.countLayers();
        if (count > 0)
            return keyItem.countLayers();
        return 1;
    }

    void LayersListBox::paintListBoxItem (int rowNumber, Graphics& g,
                      int width, int height, bool rowIsSelected)
    {

        if (rowIsSelected)
        {
            g.setColour (Colours::darkorange);
            g.fillAll();
        }

        String txt;

        if (keyItem.countLayers() > 0) {
            LayerItem item = keyItem.getLayer (rowNumber);
            File f (item.fileString());
            txt = f.getFileNameWithoutExtension();
        }
        else
        {
            txt = "No layers";
        }

        g.setColour (Colours::whitesmoke);
        g.drawFittedText (txt, 6, 0, width - 6, height, Justification::left, 2);

    }

    void LayersListBox::listBoxItemClicked (int row, const MouseEvent& e)
    {
        updateLayers();
    }

    void LayersListBox::listBoxItemDoubleClicked (int row, const MouseEvent& e)
    {
        if (keyItem.isValid())
        {
            LayerItem layer (keyItem.getLayer(row));
            if (layer.isValid())
                notifyChosen();
        }
    }

    Component* LayersListBox::refreshComponentForRow (int row, bool selected, Component* existing)
    {
        return ListBoxModel::refreshComponentForRow (row, selected, existing);
    }


    void LayersListBox::backgroundClicked (const MouseEvent& ev)
    {
        if (ev.mods.isPopupMenu())
        {
            FileChooser chooser ("Open Sample");
            if (chooser.browseForFileToOpen())
                DBG(chooser.getResult().getFullPathName());
        }
    }

    void LayersListBox::mouseWheelMove (const MouseEvent &e, const MouseWheelDetails &d)
    {
        ListBox::mouseWheelMove (e, d);
    }

    void LayersListBox::selectedRowsChanged (int lastRowSelected) {

    }

    void LayersListBox::deleteKeyPressed (int lastRowSelected)
    {
        keyItem.removeLayer (lastRowSelected);
        updateLayers();
    }

    void LayersListBox::returnKeyPressed (int lastRowSelected) {

    }

    void LayersListBox::listWasScrolled() {

    }

    var LayersListBox::getDragSourceDescription (const SparseSet<int>& currentlySelectedRows) {
        return String("");
    }

    String LayersListBox::getTooltipForRow (int row) {
        return String("");
    }

    void LayersListBox::setKeyItem (const KeyItem& key)
    {
        keyItem = key;
        updateLayers();
    }

    LayerCell* LayersListBox::cellForRow (int row)
    {
        return dynamic_cast<LayerCell*> (this->getComponentForRowNumber (row));
    }

    void LayersListBox::updateLayers()
    {
        for (int i = 0; i < getNumRows(); ++i)
            if (LayerCell* cell = cellForRow (i))
                cell->setSelected (cell->layerIndex() == getSelectedRow());

        updateContent();

        //std::clog << "UPDATE LAYERS: nrows: " << getNumRows() << " selected: " << getSelectedRow() << std::endl;
        if (getNumRows() > 0 && getSelectedRow() < 0) {
            //std::clog << " selecting first row\n";
            selectRow (0);
        }

        repaint();
    }


    bool LayersListBox::isInterestedInDragSource (const SourceDetails& details)
    {
        return details.description[0] == "files";
    }

    void LayersListBox::itemDragEnter (const SourceDetails& details) { }
    void LayersListBox::itemDragMove (const SourceDetails& details) { }
    void LayersListBox::itemDragExit (const SourceDetails& details) { }
    void LayersListBox::itemDropped (const SourceDetails& details)
    {
        SamplerView* view = findParentComponentOfClass<SamplerView>();
        SamplerDisplay* display = view->getDisplay();
        InstrumentPtr inst = display->getInstrument();

        if (! keyItem.isValid()) {
            keyItem = inst->getOrCreateKey (display->selectedNote());
        }

        for (int i = 0; i < details.description.size(); ++i)
        {
            if (i == 0) continue;
            const File file (details.description[i].toString());
            keyItem.addLayer (file);
        }

        updateContent();
    }

    bool LayersListBox::shouldDrawDragImageWhenOver() { return true; }

}
