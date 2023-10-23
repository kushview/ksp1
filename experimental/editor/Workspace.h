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

#ifndef KSP1_WORKSPACE_H
#define KSP1_WORKSPACE_H

#include "editor/Panels.h"
#include "editor/Views.h"

namespace KSP1 {

/** A light weight container for showing Panels */
class Workspace : public Component {
public:
    Workspace (SamplerEditor& e)
        : editor (e) {
        panel = nullptr;
        while (panels.size() < Panel::numPanels)
            panels.add (nullptr);

        // create all panel objects
        for (int i = 0; i < Panel::numPanels; ++i)
            switchPanel ((Panel::ID) i);

        switchPanel (Panel::samplerPanel);
    }

    ~Workspace() {
        panels.clear();
    }

    inline void
        resized() {
        if (panel != nullptr)
            panel->setBounds (getLocalBounds().reduced (1));
    }

    inline void
        switchPanel (Panel::ID p, bool reset = false) {
        if (panels[p] == nullptr || reset) {
            switch (p) {
                case Panel::samplerPanel:
                    panels.set (p, new SamplerView(), true);
                    break;
                case Panel::programsPanel:
                case Panel::settingsPanel:
                case Panel::assetsPanel:
                    panels.set (p, new EmptyPanel(), true);
                    break;
                case Panel::numPanels:
                default:
                    jassertfalse; // This isn't a real panel
                    return;
                    break;
            }
        }

        if (panel != nullptr)
            removeChildComponent (panel);

        panel = panels[p];

        if (panel != nullptr) {
            addAndMakeVisible (panel);
            resized();
        }
    }

    SamplerDisplay* display() {
        for (Panel* p : panels)
            if (SamplerView* view = dynamic_cast<SamplerView*> (p))
                return view->getDisplay();
        return nullptr;
    }

private:
    SamplerEditor& editor;
    OwnedArray<Panel> panels;
    Panel* panel;
};

} // namespace KSP1

#endif /* KSP1_WORKSPACE_H */
