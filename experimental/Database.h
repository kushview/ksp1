/*
    Database.h - This file is part of KSP1
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

#ifndef KSP1_DATABASE_H
#define KSP1_DATABASE_H

#include "KSP1.h"
#include <initializer_list>
struct sqlite3;

namespace KSP1 {

class Database {
public:
    typedef std::initializer_list<var> QueryArgs;

    Database();
    Database (const char* resource);
    Database (const File& file);
    Database (const Database& other) {
        this->db = other.db;
    }

    ~Database();

#if 0
    ValueTree executeQuery (const String& sql, const QueryArgs& args = {});
#endif
    bool executeQuery (var& results, const String& sql, const QueryArgs& args = {});
    void executeUpdate (const String& sql, const QueryArgs& args = {});

    bool tableExists (const String& name) const;

    Database& operator= (const Database& other) {
        this->db = other.db;
        return *this;
    }

private:
    sqlite3* db;
};

class DatabaseSchema {
public:
    inline DatabaseSchema (Database& db) {
        String sql =
            "CREATE TABLE IF NOT EXISTS assets ("
            "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "    name VARCHAR(128), "
            "    path VARCHAR(255), "
            "    type VARCHAR(64));";

        db.executeUpdate (sql);
    }
};
} // namespace KSP1
#endif // KSP1_DATABASE_H
