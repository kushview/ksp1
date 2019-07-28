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

#if 0
 #include "BrowseScreen.h"
 #include "PropertiesScreen.h"
#endif

#include "EditScreen.h"
#include "SamplerDisplay.h"
#include "SamplerView.h"
#include "Screens.h"

namespace KSP1 {

    SamplerDisplay& Screen::display() { return owner; }

    Screen::Screen (SamplerDisplay& disp, const String& name, Screen::ID t)
        : owner (disp),
          pages (*this),
          props ("screen"), id (t)
    {
        props.setProperty (kv::Slugs::name, name, nullptr);
        props.setProperty (kv::Slugs::type, (int32) type(), nullptr);

        addAndMakeVisible (&pages);
        pages.setOutline (0);
        pages.setIndent (6);
        pages.setTabBarDepth (18);
        pages.setAlwaysOnTop (true);

        pages.setOrientation (TabbedButtonBar::TabsAtBottom);

        displayConnection = disp.connectUpdateClient (*this);
    }

    Screen::~Screen()
    {
        removeChildComponent (&pages);
        displayConnection.disconnect();
    }

    Screen* Screen::create (SamplerDisplay& disp, Screen::ID type)
    {
        if (type == Screen::editScreen)
            return new EditScreen (disp);
       #if 0
        else if (type == Screen::patternScreen)
            return new KitScreen (disp);
        else if (type == Screen::composeScreen)
            return new EditScreen (disp);
        else if (type == Screen::kitScreen)
            return new KitScreen (disp);
        else if (type == Screen::browseScreen)
            return new BrowseScreen (disp);
       #endif
        jassertfalse;
        return new EditScreen (disp);
    }

    SamplerView* Screen::getSamplerView() const
    {
        return findParentComponentOfClass<SamplerView>();
    }
}
