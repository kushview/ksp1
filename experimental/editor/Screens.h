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

#pragma once

#include "KSP1.h"

namespace KSP1 {

class KeyItem;
class SamplerDisplay;
class SamplerView;

class Screen : public Component {
public:
    enum ID {
        editScreen,
        numScreens
    };

    virtual ~Screen();

    Screen::ID getScreenID() const { return id; }
    static Screen* create (SamplerDisplay& disp, Screen::ID type);

    inline void setTabOrientation (TabbedButtonBar::Orientation o) { pages.setOrientation (o); }
    inline int getNumPages() const { return pages.getNumTabs(); }
    inline int getCurrentPage() const { return pages.getCurrentTabIndex(); }
    inline void setCurrentPage (int page) { pages.setCurrentTabIndex (page); }
    inline void addPage (const String& name, Component* page) {
        pages.addTab (name, Colours::transparentBlack, page, true);
    }

    /** This is called by the display when a note has been selected */
    inline virtual void keySelectedEvent (const KeyItem& /*note*/) {}

    /** Find a page by class type */
    template <class T>
    inline T* findPage() {
        for (int i = pages.getNumTabs(); --i >= 0;)
            if (T* p = dynamic_cast<T*> (pages.getTabContentComponent (i)))
                return p;
        return nullptr;
    }

    virtual void onDisplayUpdate() {}

    virtual void resized() {
        pages.setBounds (getLocalBounds());
    }

    SamplerDisplay& display();
    SamplerView* getSamplerView() const;

protected:
    Screen (SamplerDisplay& disp, const String& name, Screen::ID t);
    TabbedComponent& getTabs() { return pages; }
    std::function<void()> onPageChanged;

private:
    SamplerDisplay& owner;
    class Pages : public TabbedComponent {
    public:
        Pages (Screen& s) : TabbedComponent (TabbedButtonBar::TabsAtLeft),
                            screen (s) {}

        void currentTabChanged (int newIndex, const String& newName) override {
            ignoreUnused (newIndex, newName);
            if (screen.onPageChanged)
                screen.onPageChanged();
        }

        std::function<void()> onTabChanged;

    private:
        Screen& screen;
    };

    Pages pages;
    ValueTree props;
    Screen::ID id;

    friend class Display;
    boost::signals2::connection displayConnection;
};
} // namespace KSP1
