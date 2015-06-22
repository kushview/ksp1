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

#include <initializer_list>
#include "KSP1.h"
struct sqlite3;

namespace KSP1 {

class Database
{
public:
    typedef std::initializer_list<var> QueryArgs;

    Database();
    Database (const char* resource);
    Database (const Database& other) {
        this->db = other.db;
    }

    ~Database();

    ValueTree executeQuery (const String& sql, const QueryArgs& args = {});
    void executeUpdate (const String& sql, const QueryArgs& args = {});

    Database& operator= (const Database& other) {
        this->db = other.db;
        return *this;
    }

private:
    sqlite3* db;
};

}
#endif // KSP1_DATABASE_H
