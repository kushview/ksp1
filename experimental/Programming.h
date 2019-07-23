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

#ifndef KSP1_PROGRAMMING_H
#define KSP1_PROGRAMMING_H

#include "KSP1.h"
#include "DataPath.h"
#include "Filesystem.h"


namespace KSP1 {

    class Programming
    {
    public:

        class Item
        {
        public:

            Item (Programming& p);
            Item (Programming& p, const ValueTree& data);
            ~Item();

            File asFile() const;
            bool isRoot() const ;
            String name() const ;

            int numChildren() const;
            Item parent() const;
            void printXml();
            bool isGroup() const;
            bool isValid() const;
            Item& operator= (const Item& other);
            Item addEntry (const File& file);
            Item addGroup (const String& name);
            Item getEntry (int index) const;
            int countEntries();

            inline const ValueTree& getNode() const { return node; }

        private:

            ValueTree node;
            Programming& owner;
            bool groupItem;
            friend class Programming;
        };

        Programming();
        ~Programming();
        Item addGroup (const String& name);
        void loadXmlEntries (const XmlElement& e);
        /** Find a toplevel group */
        Item findGroup (const String& name, bool createIt = false);
        ValueTree addGroup (int, const String& name);
        void loadFile (const File& programsFile);
        void loadBanks (XmlElement& banksNode);
        ValueTree addBank();
        ValueTree addBank (const String& name);
        ValueTree addBank (int id, const String& name);
        void setBankName (int bank, const String& name);
        ValueTree getBank (int id);
        ValueTree getBank (int msb, int lsb);
        void setBankName (uint8 coarse, uint8 fine, const String& name);
        void setBankName (const Midi::Bank& bank, const String& name);
        String getBankName (int bank);
        void addProgram (int bank, const String& resource);
        int getNumBanks() const;
        XmlElement* createGroupsNode();
        XmlElement* createBanksNode();
        ValueTree getProgram (int slot);
        ValueTree getProgram (int* prog);
        ValueTree getBanks();
        ValueTree getEntries();
        void buildProgramTable();
        void  scanAllMedia();

    private:

        ValueTree entries;
        ValueTree banks;

    };
}

#endif /* KSP1_PROGRAMMING_H */
