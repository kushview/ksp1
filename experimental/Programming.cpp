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

#include "Programming.h"

namespace KSP1 {

    Programming::Item::Item (Programming& p)
        : owner (p)
    { }

    Programming::Item::Item (Programming& p, const ValueTree& data)
        :  node (data), owner (p)
    {
        if (node.isValid()) {
            groupItem = node.getProperty ("type") == "group";
        }
    }

    Programming::Item::~Item() { }

    File
    Programming::Item::asFile() const
    {
        String s = node.getProperty ("resource", String::empty);
        return File (s);
    }

    bool Programming::Item::isRoot() const { return node.isValid() && node.hasType ("programs"); }
    String Programming::Item::name() const { return node.getProperty ("name", "Invalid Item"); }

    int Programming::Item::numChildren() const
    {
        int count =0;

        for (int c = 0; c < node.getNumChildren(); ++c)
        {
            ValueTree child (node.getChild (c));
            if (child.hasType("entry"))
                ++count;
        }

        return count;
    }

    Programming::Item
    Programming::Item::parent() const
    {
        if (! isRoot())
            return Item (owner, node.getParent());

        return *this; // maybe return invalid ?
    }

    void
    Programming::Item::printXml()
    {
        std::clog << node.toXmlString() << std::endl;
    }

    bool
    Programming::Item::isGroup() const
    {
        return groupItem || isRoot();
    }

    bool
    Programming::Item::isValid() const
    {
        if (! node.isValid())
            return false;
        return true;
    }

    Programming::Item&
    Programming::Item::operator= (const Item& other)
    {
        node = other.node;
        groupItem = other.groupItem;
        return *this;
    }

    Programming::Item
    Programming::Item::addEntry (const File& file)
    {
        ValueTree e ("entry");
        e.setProperty ("type", "file", nullptr);
        e.setProperty ("name", file.getFileNameWithoutExtension(), nullptr);
        e.setProperty ("resource", file.getFullPathName(), nullptr);
        node.addChild (e, -1, nullptr);
        return Item (owner, e);
    }

    Programming::Item
    Programming::Item::addGroup (const String& name)
    {
        ValueTree e ("entry");
        e.setProperty ("type", "group", nullptr);
        e.setProperty ("name", name, nullptr);
        node.addChild (e, -1, nullptr);
        return Item (owner, e);
    }

    Programming::Item
    Programming::Item::getEntry (int index) const
    {
        int i = 0;

        for (int c = 0; c < node.getNumChildren(); ++c)
        {
            ValueTree child (node.getChild (c));
            if (child.hasType ("entry"))
            {
                if (index == i)
                    return Item (owner, child);
                ++i;
            }
        }

        return Item (owner);
    }

    int
    Programming::Item::countEntries()
    {
        return numChildren();
    }


    Programming::Programming()
        : entries ("programs"), banks ("banks")
    {
        entries.setProperty ("name", "Programming", nullptr);
        entries.setProperty ("type", "group", nullptr);
    }

    Programming::~Programming()
    {

    }

    Programming::Item
    Programming::addGroup (const String& name)
    {
        Item group (findGroup (name, false));

        if (! group.isValid())
        {
            ValueTree g ("entry");
            g.setProperty ("name", name, nullptr);
            g.setProperty ("type", "group", nullptr);
            entries.addChild (g, entries.getNumChildren(), nullptr);
            group.node = g;
        }

        return group;
    }

    void
    Programming::loadXmlEntries (const XmlElement& e) {
        Item root (*this, ValueTree::fromXml (e));
        if (root.isValid()) {
            entries = root.node;
        }
    }

    /** Find a toplevel group */
    Programming::Item
    Programming::findGroup (const String& name, bool createIt)
    {
        ValueTree invalid;

        //if (! recursive)
        {
            for (int i = 0; i < entries.getNumChildren(); ++i) {
                Item item (*this, entries.getChild (i));
                if (item.isValid() && item.node.getProperty("name").toString() == name)
                {
                    std::clog << "group was found\n";
                    return item;
                }
            }

            //return Item (*this, invalid);
        }

        if (createIt)
        {
            std::clog << "creating group from find\n";
            ValueTree g ("entry");
            g.setProperty ("name", name, nullptr);
            g.setProperty ("type", "group", nullptr);
            entries.addChild (g, entries.getNumChildren(), nullptr);
            return Item (*this, g);
        }

        return Item (*this, invalid);
    }

    ValueTree
    Programming::addGroup (int, const String& name)
    {
        Item item = addGroup (name);
        return item.node;
    }

    void
    Programming::loadFile (const File& programsFile)
    {
    }

    void
    Programming::loadBanks (XmlElement& banksNode)
    { }

    ValueTree
    Programming::addBank()
    {
        return ValueTree::invalid;
    }

    ValueTree
    Programming::addBank (const String& name)
    {
    #if 0
        ValueTree b (banks.getChildWithProperty ("name", name));

        if (! b.isValid())
        {
            b = ValueTree ("bank");
            b.setProperty ("name", name, nullptr);
            b.setProperty ("id", banks.getNumChildren(), nullptr);
            banks.addChild (b, banks.getNumChildren(), nullptr);
            return b;
        }

        return ValueTree::invalid;
    #endif
        return ValueTree::invalid;
    }

    ValueTree
    Programming::addBank (int id, const String& name)
    {
        ValueTree bank (getBank (id));

        if (! bank.isValid())
        {
            bank = ValueTree ("bank");
            bank.setProperty ("id", id, nullptr);
            bank.setProperty ("name", name, nullptr);
            banks.addChild (bank, -1, nullptr);
        }

        return bank;
    }


    void
    Programming::setBankName (int bank, const String& name)
    {
        ValueTree b (getBank (bank));
        if (b.isValid())
            b.setProperty ("name", name, nullptr);
    }

    ValueTree
    Programming::getBank (int id)
    {
        return banks.getChildWithProperty ("id", id);
    }

    ValueTree
    Programming::getBank (int msb, int lsb)
    {
        return banks.getChildWithProperty ("id", Midi::combine (msb, lsb));
    }


    void
    Programming::setBankName (uint8 coarse, uint8 fine, const String& name)
    {
        if (coarse > 0x7f || fine > 0x7f)
            return;

        setBankName ((int) Midi::combine (coarse, fine), name);
    }

    void
    Programming::setBankName (const Midi::Bank& bank, const String& name)
    {
        setBankName (bank.bank(), name);
    }


    String
    Programming::getBankName (int bank)
    {
        String name ("Invalid Bank ");
        name << bank;

        if (getBank(bank).isValid())
        {
            name = banks.getChild(bank).getProperty("name");
            if (name == String::empty)
                name << "Bank " << bank;
            return name;
        }

        return name;
    }

    void
    Programming::addProgram (int bank, const String& resource)
    {
        ValueTree b (banks.getChildWithProperty ("id", bank));

        if (! b.isValid())
        {
            b = ValueTree ("bank");
            b.setProperty ("id", bank, nullptr);
            banks.addChild (b, banks.getNumChildren(), nullptr);
        }

        if (! b.isValid())
            return;

        if (b.getNumChildren() <= 128)
        {
            const int index = b.getNumChildren();

            ValueTree program ("program");
            program.setProperty ("href", resource, nullptr);
            program.setProperty ("id", index, nullptr);
            b.addChild (program, index, nullptr);

            buildProgramTable();
        }
    }

    int
    Programming::getNumBanks() const
    {
        return banks.getNumChildren();
    }

    XmlElement*
    Programming::createGroupsNode()
    {
        return entries.createXml();
    }

    XmlElement*
    Programming::createBanksNode()
    {
        return banks.createXml();
    }

    ValueTree
    Programming::getProgram (int slot)
    {
        if (banks.isValid())
        {
            for (int i = 0; i < banks.getNumChildren(); ++i)
            {
                ValueTree bank (banks.getChild(i));
                if (bank.getType().toString() == "bank")
                {
                    for (int p = 0; p < bank.getNumChildren(); ++p)
                    {
                        ValueTree prog (bank.getChild (p));
                        if (slot == (int) prog.getProperty("slot"))
                            return prog;
                    }
                }
            }
        }

        return ValueTree();
    }

    ValueTree
    Programming::getProgram (int* prog)
    {
        ValueTree bank = getBank (Midi::combine (prog[0], prog[1]));
        int index = prog[2];
        return bank.getChildWithProperty ("id", index);
    }

    ValueTree Programming::getBanks()  { return banks; }
    ValueTree Programming::getEntries() { return entries; }

    void
    Programming::buildProgramTable()
    {
        if (banks.isValid())
        {
            // start at one so this can be used in a juce menu
            int index = 1;

            for (int i = 0; i < banks.getNumChildren(); ++i)
            {
                ValueTree bank (banks.getChild(i));
                if (bank.getType().toString() == "bank")
                {
                    for (int p = 0; p < bank.getNumChildren(); ++p)
                    {
                        ValueTree prog (bank.getChild (p));
                        prog.setProperty ("slot", index++, nullptr);
                    }
                }
            }
        }
    }

    void
    Programming::scanAllMedia()
    {
        Item root (*this, entries);


        {
            Item factory (root.addGroup ("Factory"));
            File path (File::getCurrentWorkingDirectory().getChildFile ("content/TSSD"));
            DBG (path.getFullPathName());
            Array<File> files;
            path.findChildFiles (files, File::findFiles, true, "*.xml");
            for (const File& f : files) {
                factory.addEntry (f);
            }
        }


        {
            Item bke (root.addGroup ("BKE, LLC"));

            // find legacy BKE stuff + Add-on Sound Packs
            KnownFilesystems fses;
            FilesystemScanner scanner (fses, FileSearchPath (DataPath::simBeatThangPath().getFullPathName()));

            // BeatThang factory content is encrypted. Ignore factory directories
            const char* ignoreNames[] = { "Virtual", "Solo", "Demo", nullptr };
            StringArray ignore (ignoreNames);

            for (uint32 i = 0 ; i < fses.size(); ++i)
            {
                Filesystem* fs (fses.get (i));
                if (ignore.contains (fs->name))
                    continue;

                Programming::Item filesystem (bke.addGroup (fs->name));

                Item drumkits (filesystem.addGroup ("Drumkits"));
                Item insruments (filesystem.addGroup ("Instruments"));
               #if KSP1_SEQUENCER
                Item patterns (filesystem.addGroup ("Patterns"));
               #endif

                File subpath (fs->path.getChildFile ("drumkits"));
                Array<File> media;

                subpath.findChildFiles (media, File::findFiles, true, "*.btdk");
                for (const File& f : media) {
                    drumkits.addEntry (f);
                }

                subpath = fs->path.getChildFile ("instruments");
                media.clearQuick();
                subpath.findChildFiles (media, File::findFiles, true, "*.btdk");
                for (const File& f : media) {
                    insruments.addEntry (f);
                }

               #if KSP1_SEQUENCER
                subpath = fs->path.getChildFile ("patterns");
                media.clearQuick();
                subpath.findChildFiles (media, File::findFiles, true, "*.btpt");
                for (const File& f : media) {
                    patterns.addEntry (f);
                }
               #endif
            }
        }
    }
}
