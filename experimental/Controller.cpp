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

#include "Controller.h"
#include "Locations.h"
#include "MediaLibrary.h"
#include "Programming.h"

namespace KSP1 {

class Controller::Impl {
public:
    Impl() {}
    ~Impl() {}

    void createAll() {
        locations   = new Locations();
        media       = new MediaLibrary();
        programming = new Programming();
        //sampleCache = new SampleCache();
        programming->scanAllMedia();
    }

    void freeAll() {
        //sampleCache = nullptr;
        programming = nullptr;
        media       = nullptr;
        locations   = nullptr;
    }

    ScopedPointer<Locations> locations;
    ScopedPointer<MediaLibrary> media;
    ScopedPointer<Programming> programming;
    // ScopedPointer<SampleCache>  sampleCache;
};

Controller::Controller() {
    impl = new Impl();
    impl->createAll();
}

Controller::~Controller() {
    impl->freeAll();
    deleteAndZero (impl);
}

Locations& Controller::locations() { return *impl->locations; }
MediaLibrary& Controller::media() { return *impl->media; }
Programming& Controller::programs() { return *impl->programming; }

void Controller::initialize() {
    assert (impl != nullptr);
    media().initialize();
}

} // namespace KSP1
