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

#include "Locations.h"

namespace KSP1 {

    Locations::Locations()
        : fses()
    {
        FilesystemScanner (fses, fses.searchPath());
    }

    Locations::~Locations()
    {
    }

    const KnownFilesystems&
    Locations::getFilesystems() const
    {
        return fses;
    }

    void
    Locations::printAll()
    {
        Logger::writeToLog ("[BTV] Locations: " + String (fses.size()) + String (" total"));
        for (const Filesystem* fs : fses)
            Logger::writeToLog (fs->name + String ("\t") + String (fs->uuid));
    }

    void
    Locations::rebuildIndexes()
    {
        // fses.buildAssetTreeIndexes();
    }

    File
    Locations::resolvePath (const String& path) const
    {
        File ret (File::nonexistent);

        for (const Filesystem* fs : fses)
        {
            ret = fs->path.getChildFile (path);
            if (ret.exists())
                break;
        }

        return ret;
    }
}
