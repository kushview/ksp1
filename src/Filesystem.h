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

#ifndef KSP1_FILESYSTEM_H
#define KSP1_FILESYSTEM_H

#include "KSP1.h"

namespace KSP1 {

    /** A data structure which contains information about BeatThang data paths.
        This class should ONLY be used when working Beat Thang mkI content */
    struct Filesystem
    {
        Filesystem (const String& uuid_,  const File& path_,  const String& name_);
        bool valid() const { return path.exists(); }
        inline uint32 hashCode() const { return (uint32) path.hashCode(); }
        operator uint32() const { return hashCode(); }
        uint32 urid;
        String uuid;
        File   path;
        String name;
    };

    /** Maintains a list of filesystems on the system */
    class KnownFilesystems
    {
    public:
        typedef OwnedArray <Filesystem> ArrayType;

        KnownFilesystems();
        ~KnownFilesystems();

        void add (const String& uuid,
                  const File& path,
                  const String& name);

        void buildAssetTreeIndexes();

        void getNames (StringArray& names);
        const File& getPathForID (uint32 urid) const;
        const File& getPathForUUID (const String& uuid) const;

        /** Get a file from a uuid and path

            @param uuid The FSID to look for
            @param path The file path within the filysystem
         */
        File getFile (const String& uuid, const String& path) const;

        /** Get a file from an asset node

            The value tree passed in should have the properties 'fsid' and
            'path'.  fsid should be the filesystem id and path should be the
            relative path within the filesystem location.  The ValueTree name
            is unchecked and can be anything.

            Xml of the tree would look like this...
            <asset fsid="some-uuid-number" path="pattern/Nifty-Pattern.btpt" />
         */
        File getFile (const ValueTree& assetNode) const;

        Filesystem* findByName (const String& name) const;
        Filesystem* get (uint32 index) const;

        inline const FileSearchPath& searchPath() const { return locations; }
        inline void addSearchPath (const File& path) { jassert(path.isDirectory()); locations.addIfNotAlreadyThere (path); }

        inline ArrayType& array() { return llist; }
        inline const ArrayType& array() const { return llist; }
        inline uint32 size() const { return llist.size(); }
        inline Filesystem* operator[] (const size_t i) const { return llist.getUnchecked(i); }
        inline const Filesystem* const* begin() const { return llist.begin(); }
        inline const Filesystem* const* end() const { return llist.end(); }

    private:

        FileSearchPath locations;
        ArrayType llist;

    };

    /** A scanner for sampler (beatthang) file systems */
    class FilesystemScanner
    {
    public:

        FilesystemScanner (KnownFilesystems& fses, const FileSearchPath& searchPaths);
        ~FilesystemScanner() { }

    };

}

#endif /* KSP1_FILESYSTEM_H */
