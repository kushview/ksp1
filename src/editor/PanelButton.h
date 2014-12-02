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

#ifndef KSP1_PANEL_BUTTON_H
#define KSP1_PANEL_BUTTON_H

#include "Panels.h"

namespace KSP1 {
namespace Gui {

    class PanelButton : public Label
    {
    public:

        typedef boost::signals2::signal<void(Panel::ID)> SelectedSignal;

        PanelButton()
            : panel (Panel::samplerPanel),
              menuIsShowing (false)
        { }

        ~PanelButton() { }

        inline void
        selectPanel (Panel::ID p, bool notify = false)
        {
            setText (Panel::name (p), notify ? dontSendNotification : sendNotification);
            panel = p;

            if (notify)
                selectedSignal (panel);
        }

        inline void
        mouseDown (const MouseEvent& ev)
        {
            if (! menuIsShowing && ev.mods.isLeftButtonDown())
            {
                PopupMenu menu;
                for (int i = 1; i < Panel::numPanels; ++i)
                    menu.addItem (i, Panel::name ((Panel::ID) i));

                menuIsShowing = true;
                int res = menu.showMenu (PopupMenu::Options());
                if (res && res != panel)
                    selectPanel ((Panel::ID) res, true);
                menuIsShowing = false;
            }
        }

        inline SelectedSignal& signalSelected() { return selectedSignal; }

    private:

        Panel::ID panel;
        bool menuIsShowing;
        SelectedSignal selectedSignal;

    };
}}


#endif /* KSP1_PANEL_BUTTON_H */
