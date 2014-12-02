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

#ifndef __JUCE_HEADER_8A8B6F6667583E52__
#define __JUCE_HEADER_8A8B6F6667583E52__

//[Headers]
#include "PortWriter.h"
#include "editor/Keyboard.h"
#include "editor/LayersListBox.h"
#include "editor/LevelMeter.h"
#include "editor/Banks.h"
#include "editor/Panels.h"

namespace KSP1 {
namespace Gui {
    class SamplerEditor;
}}
//[/Headers]

#include "ArticulationControls.h"
#include "SamplerDisplay.h"


namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
    This is the main view used when the sampler is run as a tone module sampler.
    E.g.  No composer and pattern editing.   Just a sampler that plays Instruments
                                                                    //[/Comments]
*/
class SamplerView  : public Panel,
                     public DragAndDropContainer,
                     public ButtonListener,
                     public SliderListener,
                     public ComboBoxListener
{
public:
    SamplerView ();
    ~SamplerView();

    //[UserMethods]
    void setVolume (float db, bool notify = false);

    inline SamplerDisplay* getDisplay() const { return display; }

    inline int32 type() const { return Panel::samplerPanel; }

    LevelMeter* levelMeter() const;

    InstrumentPtr getInstrument (const int track = 0) const;
    void setInstrment (InstrumentPtr i);
    void setInterface (SamplerInterface* iface);

    // signal/callback handlers
    void loadFile (const File&);
    void onDisplayUpdate();
    void onKeyboardMidi (const MidiMessage& midi);
    void onKeySelected (int k);
    void buttonStateChanged (Button*);
    void stabilizeView();


    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]
    SamplerInterface* interface;
    MidiKeyboardState keyboardState;

    //Globals& world;
    //SamplerEditor& editor;

    class Updater; friend class Updater;
    ScopedPointer<Updater> updater;

    ScopedPointer<AssetTree> tree;

    /** @internal load a program into the current instrument */
    void loadProgram (const Programming::Item&);
    /** @internal Called when a layer is chosen in the listbox */
    void layerChosen();
    /** @internal Update all of the controls in this view */
    void updateControls (NotificationType n = dontSendNotification);
    //[/UserVariables]

    ScopedPointer<TextButton> layerKeyButton;
    ScopedPointer<GroupComponent> articulationGroup;
    ScopedPointer<Label> volLabel;
    ScopedPointer<Slider> layerVolume;
    ScopedPointer<Slider> voiceGroup;
    ScopedPointer<Label> layersLabel;
    ScopedPointer<LayersListBox> layersListBox;
    ScopedPointer<ComboBox> triggerMode;
    ScopedPointer<LevelMeter> meter;
    ScopedPointer<DecibelScaleComponent> dbScale;
    ScopedPointer<Slider> volume;
    ScopedPointer<Slider> layerPan;
    ScopedPointer<Slider> layerPitch;
    ScopedPointer<Slider> fxSend1;
    ScopedPointer<Slider> fxSend2;
    ScopedPointer<TabbedComponent> mediaTabs;
    ScopedPointer<ArticulationControls> articulationControls;
    ScopedPointer<Slider> fxSend3;
    ScopedPointer<Slider> fxSend4;
    ScopedPointer<Label> layerVolLabel;
    ScopedPointer<Label> layerPanLabel;
    ScopedPointer<Label> layerPitchLabel;
    ScopedPointer<Label> fx1Label;
    ScopedPointer<Label> fx1Label2;
    ScopedPointer<Label> fx1Label3;
    ScopedPointer<Label> fx1Label4;
    ScopedPointer<KeyboardWidget> keyboard;
    ScopedPointer<SamplerDisplay> display;
    ScopedPointer<Slider> keyLength;
    ScopedPointer<TextButton> propsButton;
    ScopedPointer<TextButton> editButton;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerView)
};

//[EndFile] You can add extra defines here...

//[/EndFile]

}} /* namespace KSP1::Gui */

#endif   // __JUCE_HEADER_8A8B6F6667583E52__
