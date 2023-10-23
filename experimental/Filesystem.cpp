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

#include "./Filesystem.h"
#include "./DataPath.h"
#include "./Types.h"

namespace KSP1 {

Filesystem::Filesystem (const String& uuid_, const File& path_, const String& name_)
    : urid (0), uuid (uuid_), path (path_), name (name_) {
    if (uuid.isEmpty())
        uuid = Utility::createGUID (path.getFullPathName() + name);

    jassert (path.isDirectory());
}

KnownFilesystems::KnownFilesystems() {
    addSearchPath (DataPath::simBeatThangPath());
    addSearchPath (DataPath::defaultUserPath().getParentDirectory());
}

KnownFilesystems::~KnownFilesystems() {}

void KnownFilesystems::add (const String& uuid_,
                            const File& path_,
                            const String& name_) {
    for (const Filesystem* fs : llist)
        if (fs->path == path_)
            return;

    llist.add (new Filesystem (uuid_, path_, name_));
}

void KnownFilesystems::buildAssetTreeIndexes() {
#if 0
        for (Filesystem* fs : llist)
        {
            fs->clear();

            if (! fs->getFile().existsAsFile())
            {
                for (DirectoryIterator iter (fs->getFile().getParentDirectory(),
                                             false, "*", File::findDirectories); iter.next();)
                {
                    fs->root().addFile (iter.getFile(), -1, true);
                }

                if (ScopedXml xml = fs->createXml())
                {
                    if (! xml->writeToFile (fs->getFile(), String::empty))
                        Logger::writeToLog ("Failed to write filesystem manifest: " + fs->getFile().getFullPathName());
                    else
                        Logger::writeToLog ("Filesystem manifest written to disk");
                }
                else
                {
                    Logger::writeToLog("Failed to create filesystem XML");
                }
            }
            else
            {
                if (ScopedXml xml = XmlDocument::parse (fs->getFile()))
                {
                    fs->loadFromXml (*xml);
                    Logger::writeToLog ("Filesystem restored: " + fs->nameValue().getValue().toString());
                }
                else {
                    Logger::writeToLog ("Failed to read filesystem manifest");
                }
            }
        }
#endif
}

const File&
    KnownFilesystems::getPathForID (uint32 urid) const {
    for (const Filesystem* fs : llist)
        if (fs != nullptr && fs->hashCode() == urid)
            return fs->path;

    return File::nonexistent;
}

const File&
    KnownFilesystems::getPathForUUID (const String& uuid) const {
    for (const Filesystem* fs : llist) {
        if (fs && fs->uuid == uuid)
            return fs->path;
    }

    return File::nonexistent;
}

File KnownFilesystems::getFile (const String& uuid, const String& path) const {
    File f = getPathForUUID (uuid).getChildFile (path);
    return f.existsAsFile() ? f : File::nonexistent;
}

File KnownFilesystems::getFile (const ValueTree& node) const {
    return getFile (node.getProperty (Slugs::fsid),
                    node.getProperty (Slugs::path));
}

void KnownFilesystems::getNames (StringArray& names) {
    for (const Filesystem* fs : llist)
        names.add (fs->name);
}

Filesystem*
    KnownFilesystems::findByName (const String& name) const {
    for (Filesystem* fs : llist)
        if (fs != nullptr && fs->name == name)
            return fs;

    return nullptr;
}

Filesystem*
    KnownFilesystems::get (uint32 index) const {
    if (index < llist.size())
        return llist.getUnchecked (index);
    return nullptr;
}

FilesystemScanner::FilesystemScanner (KnownFilesystems& fses, const FileSearchPath& searchPaths) {
    for (int i = 0; i < searchPaths.getNumPaths(); ++i) {
        File sbt (searchPaths[i]);
        Array<File> subdirs;
        sbt.findChildFiles (subdirs, File::findDirectories, false, "*");

        for (int i = 0; i < subdirs.size(); ++i) {
            File fsid (subdirs[i].getChildFile ("fsid.xml"));

            if (! fsid.existsAsFile())
                fsid = subdirs[i].getChildFile (".fsid.xml");

            if (fsid.existsAsFile()) {
                ScopedXml e (XmlDocument::parse (fsid));
                fses.add (e->getChildByName ("FilesystemId")->getAllSubText(),
                          fsid.getParentDirectory(),
                          e->getChildByName ("FilesystemName")->getAllSubText());
            } else {
                const File manifest = subdirs[i].getChildFile ("manifest.xml");
                if (manifest.existsAsFile()) {
                    fses.add (String::empty,
                              manifest.getParentDirectory(),
                              "New Filesystem");
                }
            }
        }
    }
}
} // namespace KSP1
