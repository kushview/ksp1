/*
    SqlQuery.h - This file is part of KSP1
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
#ifndef KSP1_SQL_QUERY_H
#define KSP1_SQL_QUERY_H

#include "KSP1.h"
#include "URIs.h"
#include "../../libs/lvtk/lvtk/ext/atom.hpp"

namespace KSP1 {

class SqlQuery {
public:

    SqlQuery (const String& query)
        : sql (query)
    { }

    SqlQuery (const KSP1::URIs& uris, const lvtk::AtomObject& o)
    {
        assert (o.otype() == uris.ksp1_SqlQuery);
        const lvtk::Atom atom;
        if (1 == lv2_atom_object_get (o, uris.slugs_sql, &atom, 0))
            sql = String::fromUTF8 (atom.as_string());
    }

    const String& getSql() const { return sql; }
private:
    String sql;
};

}
#endif // KSP1_SQL_QUERY_H
