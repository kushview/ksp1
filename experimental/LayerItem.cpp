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

#include "Instrument.h"

namespace KSP1 {

using namespace kv;

LayerItem::LayerItem (const ValueTree& data)
    : kv::ObjectModel (data) {
    setMissingProperties();
}

LayerItem::LayerItem()
    : kv::ObjectModel (Tags::layer) {
    setMissingProperties();
}

LayerItem::~LayerItem() {
}

int32 LayerItem::index() const {
    const ValueTree keyNode (node().getParent());
    return keyNode.indexOf (this->node());
}

int32 LayerItem::getNote() const {
    if (objectData.getParent().isValid()) {
        KeyItem kitem (objectData.getParent());
        return kitem.getNote();
    }

    return (int32) objectData.getProperty (Slugs::note, -1);
}

void LayerItem::setMissingProperties() {
    Random rand (Time::currentTimeMillis());
    stabilizePropertyPOD (Slugs::id, rand.nextInt (Range<int> (1, std::numeric_limits<int>::max())));
    stabilizePropertyString (Slugs::name, "Invalid");
    stabilizePropertyPOD (Tags::start, (double) 0);
    stabilizePropertyPOD (Tags::length, (double) 0.0f);
    stabilizePropertyPOD (Tags::index, (int32) -1);
    stabilizePropertyPOD (Tags::note, (int32) -1);
    stabilizePropertyPOD (Tags::volume, (double) 0.0f);
    stabilizePropertyPOD (Tags::cutoff, (double) 1.0f);
    stabilizePropertyPOD (Tags::resonance, (double) 0.0f);
    stabilizePropertyPOD (Tags::panning, (double) 0.5f);
    stabilizePropertyPOD (Tags::velocityUpper, (double) 1.0f);
    stabilizePropertyPOD (Tags::velocityLower, (double) 0.0f);
}

void LayerItem::fixPropertyTypes() {
    set (Tags::name, get (Slugs::name).toString());
    set (Tags::start, getStart());
    set (Tags::length, getLength());
    set (Tags::index, (int32) get (Slugs::index));
    set (Tags::note, (int32) objectData.getProperty (Slugs::note, -1));
    set (Tags::volume, (double) get (Slugs::volume));
    set (Tags::cutoff, (double) get (Tags::cutoff));
    set (Tags::resonance, (double) get (Tags::resonance));
    set (Tags::panning, (double) get (Tags::panning));
    set (Tags::velocityUpper, (double) get (Tags::velocityUpper));
    set (Tags::velocityLower, (double) get (Tags::velocityLower));
}

const var& LayerItem::get (const Identifier& prop) const {
    return objectData.getProperty (prop);
}

void LayerItem::set (const Identifier& prop, const var& val) {
    objectData.setProperty (prop, val, nullptr);
}

} // namespace KSP1
