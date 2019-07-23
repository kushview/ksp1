/*
    This file is part of KSP1
    Copyright (C) 2015  Kushview, LLC. All rights reserved.

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

#include "editor/AssetsListBox.h"
#include "editor/SamplerView.h"
#include "Database.h"
#include "DataPath.h"


namespace KSP1 {

    class AssetsListBoxHeader : public Component,
                                public TextEditor::Listener
    {
    public:
        AssetsListBoxHeader (AssetsListBox* box)
            : owner (box)
        {
            addAndMakeVisible (&searchText);
            searchText.addListener (this);
            searchText.setColour (TextEditor::backgroundColourId, Colours::darkgrey);
            searchText.setColour (TextEditor::textColourId, Colours::whitesmoke);
            searchText.setTooltip ("Search for samples and instruments");
            setSize (100, 24);
        }
        
        ~AssetsListBoxHeader()
        {
            searchText.removeListener (this);
        }

        void resized () override
        {
            searchText.setSize (getWidth(), getHeight());
        }

        virtual void textEditorTextChanged (TextEditor& text) override
        {

        }

        virtual void textEditorReturnKeyPressed (TextEditor& text) override
        {
            Database db (DataPath::defaultDatabaseFile().getFullPathName().toRawUTF8());
            String sql ("SELECT id, name, path FROM assets WHERE name LIKE '%");
            sql << text.getText() << "%' ORDER BY name ASC;";
            
            var results;
            if (db.executeQuery (results, sql))
            {
                owner->results = results;
                
                File f ("~/Desktop/results.json");
                f.create();
                f.replaceWithText(JSON::toString (results));
                owner->updateContent();
            }
        }

        /** Called when the user presses the escape key. */
        virtual void textEditorEscapeKeyPressed (TextEditor&) override
        {

        }

        /** Called when the text editor loses focus. */
        virtual void textEditorFocusLost (TextEditor&) override
        {

        }

    private:
        AssetsListBox* owner;
        TextEditor searchText;
    };

    AssetsListBox::AssetsListBox()
    {
        root = nullptr;
        setModel (this);
        setRowHeight (18);
        setHeaderComponent (new AssetsListBoxHeader (this));
    }

    AssetsListBox::~AssetsListBox()
    {
        setModel (nullptr);
        setHeaderComponent (nullptr);
    }

    /* Element::Icon AssetsListBox::getIcon() const
    {
        return  Element::Icon (Element::getIcons().document, Colours::yellow);
    } */

    void AssetsListBox::setRootItem (const AssetItem &asset)
    {
        root = new AssetItem (asset);
        updateContent();
    }

    int AssetsListBox::getNumRows()
    {
        return results.size();
    }

    AssetItem AssetsListBox::getAsset (int32 row) const
    {
        assert (root != nullptr);
        return root->getChild (row);
    }

    AssetItem AssetsListBox::getSelectedAsset() const
    {
        return getAsset (this->getSelectedRow());
    }

    void AssetsListBox::paintListBoxItem (int row, Graphics& g, int w, int h, bool selected)
    {
        if (! isRootItemValid())
            return;

        const var& r (results[row]);

        const int pady = 4;
        const int padx = 22;

        if (selected)
        {
            g.setColour (Colours::darkorange);
            g.fillRect (0, 0, w, h);
        }

#if 0
        getIcon().draw (g, Rectangle<float> (4.0f, (float)pady,
                                             getRowHeight(), getRowHeight() - (2.0f * (float)pady)),
                                             false);
#endif
        g.setColour (Colours::whitesmoke);
        g.setFont (Font (getRowHeight() - 2));

        const String name = r.getDynamicObject()->getProperties().getWithDefault("name", "Error");
        g.drawFittedText (name, padx, pady, w - padx, h - (2 * pady),
                          Justification::centredLeft, 1);
    }

    Component* AssetsListBox::refreshComponentForRow (int row, bool selected, Component* existing)
    {
        return nullptr;
    }

    void AssetsListBox::listBoxItemClicked (int row, const MouseEvent& e)
    {
        selectRow (row);
        return ListBoxModel::listBoxItemClicked (row, e);
    }

    void AssetsListBox::listBoxItemDoubleClicked (int row, const MouseEvent& e)
    {
        SparseSet<int> s;
        s.addRange (Range<int> (row, row));
        setSelectedRows (s, dontSendNotification);
#if 0
        const ValueTree r (results.getChild(row));
        DBG (r.getProperty(Slugs::path, String::empty).toString());
        return ListBoxModel::listBoxItemDoubleClicked (row, e);
#endif
    }

    void AssetsListBox::backgroundClicked (const MouseEvent& e)
    {
        return ListBoxModel::backgroundClicked (e);
    }

    void AssetsListBox::selectedRowsChanged (int row)
    {
        return ListBoxModel::selectedRowsChanged (row);
    }

    void AssetsListBox::deleteKeyPressed (int row)
    {
        return ListBoxModel::deleteKeyPressed (row);
    }

    void AssetsListBox::returnKeyPressed (int row)
    {
#if 0
        const ValueTree r (results.getChild (row));
        if (! r.isValid()) return;

        SamplerView* view = findParentComponentOfClass<SamplerView>();
        if (! view) return;

        String filePath (r.getProperty ("path", String::empty).toString());
        if (filePath.substring(0, 1) != "/") {
            filePath = DataPath::factoryContentPath().getChildFile(filePath).getFullPathName();
        }

        const int note (view->getDisplay()->selectedNote());
        KeyItem key (view->getDisplay()->getInstrument()->getOrCreateKey (note));
        File file (DataPath::resolve();
        if (! file.existsAsFile())
            DBG ("File doesn't exist");

        if (key.isValid())
            key.addLayer (file);
        else
            DBG ("Key is invalid, can't add layer");
#endif
    }

    void AssetsListBox::listWasScrolled() { }

    var AssetsListBox::getDragSourceDescription (const SparseSet<int>& selected)
    {
        var desc;
        desc.append ("files");
        for (int i = 0; i < selected.size(); ++i)
        {
#if 0
            const ValueTree r (results.getChild (selected [i]));
            String filePath (r.getProperty (Slugs::path, String::empty).toString());
            const File f (DataPath::resolvePath (filePath));
            desc.append (f.getFullPathName());
#endif
        }

        return desc;
    }

    String AssetsListBox::getTooltipForRow (int row) { return String::empty; }
    MouseCursor AssetsListBox::getMouseCursorForRow (int row) { return ListBoxModel::getMouseCursorForRow (row); }

    void AssetsListBox::handleAssetSelected (const AssetItem &item)
    {
        signalSelected (item);
    }

    bool AssetsListBox::keyPressed (const KeyPress& key)
    {
        const AssetItem item (getSelectedAsset());

        if (key.getKeyCode() == KeyPress::leftKey)
        {
            if (root && ! root->isRoot())
            {
                root = new AssetItem (root->getParent());
                updateContent();
                return true;
            }
        }
        else if (key.getKeyCode() == KeyPress::rightKey)
        {
            if (item.isFile())
            {
                handleAssetSelected (item);
                return true;
            }
            else if (root)
            {
                setRootItem (item);
                return true;
            }
        }
        else if (key == KeyPress::spaceKey) {
            DBG ("Play sample !!!");
        }

        return ListBox::keyPressed (key);
    }
}
