/*
    ContentScanner.cpp - This file is part of KSP1
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

#include "ContentScanner.h"
#include "Database.h"

namespace KSP1 {

ContentScanner::ContentScanner (Database& _db)
    : db (_db) {}

ContentScanner::~ContentScanner() {}

void ContentScanner::scanLocation (const File& dir) {
    if (! dir.isDirectory())
        return;

    bool isDirectory = false;
    bool isHidden    = false;
    int64 fileSize   = 0;
    Time modTime;
    Time creationTime;
    bool isReadOnly = false;

    DirectoryIterator iter (dir, true, "*.wav;", File::findFilesAndDirectories);

    const int numRecords = 1;
    int recordNum        = 0;
    String query;
    String assetType = "sample";

    while (iter.next (&isDirectory, &isHidden, &fileSize, &modTime, &creationTime, &isReadOnly)) {
        if (isDirectory)
            continue;

        if (recordNum == 0) {
            query.clear();
            query << "INSERT INTO assets (name, path, type) VALUES ";
        }

        const File& file (iter.getFile());
        const String relPath (file.getRelativePathFrom (dir));
        query << "('" << file.getFileNameWithoutExtension() << "', '" << relPath << "', '" << assetType << "')";

        ++recordNum;

        if (recordNum == numRecords) {
            query << ";";
            db.executeUpdate (query);
            recordNum = 0;
        } else {
            query << ", ";
        }
    }

    if (0 != recordNum) {
        query = query.upToLastOccurrenceOf (", ", false, false);
        query << ";";

        db.executeUpdate (query);
    }
}

void ContentScanner::scanContentPack (const File& dir) {
    if (! dir.isDirectory())
        return;

    bool isDirectory = false;
    bool isHidden    = false;
    int64 fileSize   = 0;
    Time modTime;
    Time creationTime;
    bool isReadOnly = false;

    DirectoryIterator iter (dir, true, "*", File::findFilesAndDirectories);

    const int numRecords = 256;
    int recordNum        = 0;
    String query;
    String assetType = "sample";

    while (iter.next (&isDirectory, &isHidden, &fileSize, &modTime, &creationTime, &isReadOnly)) {
        if (isDirectory)
            continue;

        if (recordNum == 0) {
            query.clear();
            query << "INSERT INTO assets (name, path, type) VALUES ";
        }

        const File& file (iter.getFile());
        const String relPath (file.getRelativePathFrom (dir));
        query << "('" << file.getFileNameWithoutExtension() << "', '" << relPath << "', '" << assetType << "')";

        ++recordNum;
        if (recordNum == numRecords) {
            query << ";";
            db.executeUpdate (query);
            recordNum = 0;
        } else {
            query << ", ";
        }
    }

    if (0 != recordNum) {
        query = query.fromLastOccurrenceOf (", ", false, false);
        query << ";";
        db.executeUpdate (query);
    }
}
} /* namespace KSP1 */
