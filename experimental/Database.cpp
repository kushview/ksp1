/*
    Database.cpp - This file is part of KSP1
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


#include "../libs/sqlite3/sqlite3.h"
#include "KSP1.h"
#include "Database.h"

static int db_init = -100;

// sqlite callback format : int (*callback)(void*,int,char**,char**)
#if 0
int ksp1_db_query (void* arg, int count, char** values,  char** fields)
{
    ValueTree* tree = (ValueTree*) arg;
    ValueTree row ("row");

    for (int i = 0; i < count; ++i) {
        row.setProperty (fields[i], values[i], nullptr);
    }
    tree->addChild (row, -1, nullptr);
    return SQLITE_OK;
}
#endif

// sqlite callback format : int (*callback)(void*,int,char**,char**)
int ksp1_query_var (void* arg, int count, char** values,  char** fields)
{
    DynamicObject::Ptr obj = new DynamicObject();
    for (int i = 0; i < count; ++i)
        obj->setProperty (fields[i], values[i]);
    ((var*) arg)->append (var (obj));
    return SQLITE_OK;
}

namespace KSP1 {

    Database::Database()
        : db (nullptr)
    {
        if (db_init == -100)
            db_init = sqlite3_initialize();
    }

    Database::Database (const char* resource)
    {
        db = nullptr;
        if (db_init == -100)
            db_init = sqlite3_initialize();

        jassert (db_init == SQLITE_OK);

        int res = sqlite3_open (resource, &db);
        if (SQLITE_OK != res)
        {
            DBG ("SQL Error: " << sqlite3_errmsg (db));
            if (db != nullptr)
            {
                sqlite3_close (db);
                db = nullptr;
            }
        }
    }

    Database::Database (const File& file)
        : Database (file.getFullPathName().toRawUTF8())
    { }

    Database::~Database()
    {
        if (db) {
            sqlite3_close (db);
            db = nullptr;
        }
    }

#if 0
    ValueTree Database::executeQuery (const String& sql, const QueryArgs& args)
    {
        if (nullptr == db)
            return ValueTree::invalid;

        char* err = nullptr;
        int res = SQLITE_ERROR;
        ValueTree tree ("results");


        if (! sql.containsChar ('?')) {
            res = sqlite3_exec (db, sql.toRawUTF8(), ksp1_db_query, &tree, &err);
        } else {
            String query;
            int start = 0;
            int end = 0;

            for (const var& val : args)
            {
                end = sql.indexOf (start, "?");
                query << sql.substring (start, end);

                if (val.isString())
                    query << "'" << val.toString() << "'";
                else
                    query << val.toString();

                start = end + 1;
            }

            query << sql.substring (start);
            res = sqlite3_exec (db, query.toRawUTF8(), ksp1_db_query, &tree, &err);
        }

        if (res == SQLITE_OK) {

        } else {

        }

        if (err) {
            DBG ("SQL Error: " << err);
            sqlite3_free (err);
            err = nullptr;
        }

        return tree;
    }
#endif
    
    bool Database::executeQuery (var& results, const String& sql, const QueryArgs& args)
    {
        if (nullptr == db)
            return false;
        
        char* err = nullptr;
        int res = SQLITE_ERROR;
        
        if (! sql.containsChar ('?'))
        {
            res = sqlite3_exec (db, sql.toRawUTF8(), ksp1_query_var, &results, &err);
        }
        else
        {
            String query;
            int start = 0;
            int end = 0;
            
            for (const var& val : args)
            {
                end = sql.indexOf (start, "?");
                query << sql.substring (start, end);
                
                if (val.isString())
                    query << "'" << val.toString() << "'";
                else
                    query << val.toString();
                
                start = end + 1;
            }
            
            query << sql.substring (start);
            res = sqlite3_exec (db, query.toRawUTF8(), ksp1_query_var, &results, &err);
        }
        
        const bool result = (res == SQLITE_OK);
        
        if (res == SQLITE_OK) {
            
        } else {
            
        }
        
        if (err)
        {
            DBG ("SQL Error: " << err);
            sqlite3_free (err);
            err = nullptr;
        }
        
        return result;
    }
    
    void Database::executeUpdate (const String& sql, const QueryArgs& args)
    {
        if (nullptr == db)
            return;

        char* err = nullptr;
        int res = SQLITE_ERROR;

        if (! sql.containsChar ('?')) {
            res = sqlite3_exec (db, sql.toRawUTF8(), ksp1_query_var, this, &err);
        } else {
            String query;
            int start = 0;
            int end = 0;

            for (const var& val : args)
            {
                const bool isString = val.isString();

                end = sql.indexOf (start, "?");
                query << sql.substring (start, end);

                if (isString)
                    query << "'";

                query << val.toString();

                if (isString)
                    query << "'";

                start = end + 1;
            }

            query << sql.substring (start);
            res = sqlite3_exec (db, query.toRawUTF8(), ksp1_query_var, this, &err);
        }

        if (err)
        {
            DBG ("SQL Error: " << err);
            sqlite3_free (err);
            err = nullptr;
        }
    }
    
}
