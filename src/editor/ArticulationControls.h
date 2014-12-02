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

    GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_3D552BD7566568E__
#define __JUCE_HEADER_3D552BD7566568E__

//[Headers]
#include "KSP1.h"


namespace KSP1 {
    class Articulation;
}
//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ArticulationControls  : public Component,
                              public ComboBoxListener
{
public:
    ArticulationControls ();
    ~ArticulationControls();

    //[UserMethods]
    void setModel (const KSP1::Articulation& model);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]
    //[/UserVariables]

    ScopedPointer<ComboBox> articulationType;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArticulationControls)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace KSP1::Gui */

#endif   // __JUCE_HEADER_3D552BD7566568E__
