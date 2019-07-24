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

#ifndef KSP1_PANELS_H
#define KSP1_PANELS_H

#include "KSP1.h"

namespace KSP1 {

    class Panel :  public Component
    {
    public:
        /** Panel types */
        enum ID {
            assetsPanel   = 0,
            programsPanel = 1,
            samplerPanel  = 2,
            settingsPanel = 3,
            numPanels     = 4
        };

        Panel () { }
        virtual ~Panel() { }

        inline static const String&
        name (Panel::ID type)
        {
            static const String __names [numPanels + 1] = {
                String ("Assets"),
                String ("Programs"),
                String ("Sampler"),
                String ("Settings"),
                String ("Empty Panel")
            };

            return __names [type];
        }

        virtual int32 type() const = 0;
    };

    class EmptyPanel : public Panel
    {
    public:
        EmptyPanel() { }
        ~EmptyPanel() { }
        int32 type() const { return Panel::numPanels; }
    };

}

#endif /* KSP1_PANELS_H */
