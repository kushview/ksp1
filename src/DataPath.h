/*
    DataPath.h - This file is part of KSP1
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

namespace KSP1 {

    /** This class describes the common directories where BTV session data is stored */
    class DataPath
    {
    public:

        DataPath();
        DataPath (const File& srcDir);
        DataPath (const DataPath& other);
        ~DataPath();

        /** Resolves a relative path to a file in the best matching
            datapath */
        static File resolvePath (const String& path);

        /** Returns BKE's factory content path (e.g. SimBeatThang) */
        static const File& simBeatThangPath();

        /** Returns the default library db file */
        static const File& defaultDatabaseFile();

        /** Returns the default user data path */
        static const File& defaultUserPath();

        /** Returns the expansion content path */
        static const File& expansionContentPath();

        /** Returns the factory content path */
        static const File& factoryContentPath();

        /** Get the Directory associated with this datapath */
        inline const File& getFile() const { return file; }

        // operators
        inline operator const File& () { return file; }
        inline DataPath& operator= (const DataPath& other) { this->file = other.file; return *this; }
        inline bool operator== (const DataPath& other) const { return file == other.file; }
        inline bool operator!= (const DataPath& other) const { return ! (operator==(other)); }

    private:
        File file;

    };

}
