/*
    MediaLibrary.cpp - This file is part of KSP1
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

#include "ContentScanner.h"
#include "Database.h"
#include "DataPath.h"
#include "MediaLibrary.h"

namespace KSP1 {

MediaLibrary::MediaLibrary()
{
}

MediaLibrary::~MediaLibrary()
{

}

void MediaLibrary::initialize()
{
    const File& dbFile = DataPath::defaultDatabaseFile();

    DBG (dbFile.getFullPathName());
    if (true) //(! dbFile.existsAsFile())
    {
        Database db (dbFile.getFullPathName().toRawUTF8());
        db.executeUpdate ("DROP TABLE IF EXISTS assets;"
                          "CREATE TABLE assets ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                            "name VARCHAR (255) NOT NULL, "
                            "path VARCHAR (255) NOT NULL, "
                            "type VARCHAR (64) NOT NULL);");
        db.executeUpdate ("DROP TABLE IF EXISTS tags;"
                          "CREATE TABLE tags ("
                            "id VARCHAR(36) PRIMARY KEY NOT NULL, "
                            "slug VARCHAR (64) NOT NULL"
                          ");");
        db.executeUpdate ("DROP TABLE IF EXISTS tagged;"
                          "CREATE TABLE tagged ("
                            "id VARCHAR(36) PRIMARY KEY NOT NULL, "
                            "object_id INTEGER NOT NULL, "
                            "tag_id INTEGER NOT NULL"
                          ");");

        const char* _tags[] = { "snare", "tom", "kick", 0 };
        StringArray tags (_tags);
        String sql ("INSERT INTO tags (id, slug) VALUES \n");

        for (int i = 0; i < tags.size(); ++i)
        {
            Uuid id;
            sql << "('" << id.toDashedString() << "', '" << tags[i] << "')";
            if (i < (tags.size() - 1))
                sql << ", \n";
        }
        sql << ";";
        db.executeUpdate (sql);
        ContentScanner content (db);
        content.scanLocation (DataPath::factoryContentPath());
    }
}

}
