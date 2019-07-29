/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "editor/LevelMeter.h"
#include "editor/LayersListBox.h"
#include "editor/Keyboard.h"
//[/Headers]

#include "SamplerDisplay.h"


namespace KSP1 {

//==============================================================================
/**
                                                                    //[Comments]
                                                                    //[/Comments]
*/
class SamplerView  : public Component,
                     public DragAndDropContainer,
                     private Value::Listener,
                     public Slider::Listener,
                     public Button::Listener
{
public:
    //==============================================================================
    SamplerView ();
    ~SamplerView();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setVolume (float db, bool notify = false);

    inline SamplerDisplay* getDisplay() const { return display.get(); }

    LevelMeter* levelMeter() const;

    InstrumentPtr getInstrument (const int track = 0) const;
    void setInstrument (InstrumentPtr i);

    MidiKeyboardState& getMidiKeyboardState() { return keyboardState; }

    // signal/callback handlers
    void loadFile (const File&);
    void onDisplayUpdate();
    void onKeyboardMidi (const MidiMessage& midi);
    void onKeySelected (int k);
    void stabilizeView();
    void setMainRMS (const float rmsL, const float rmsR);

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Value activeSound;
    MidiKeyboardState keyboardState;

    class Updater; friend class Updater;
    ScopedPointer<Updater> updater;

    void layerChosen();
    void updateControls (NotificationType n = dontSendNotification);
    void valueChanged (Value&) override;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<SamplerDisplay> display;
    std::unique_ptr<Label> volLabel;
    std::unique_ptr<LevelMeter> meter;
    std::unique_ptr<Slider> volume;
    std::unique_ptr<KeyboardWidget> keyboard;
    std::unique_ptr<Label> outputLabel;
    std::unique_ptr<Label> ksp1Label;
    std::unique_ptr<TextButton> soundsButton;
    std::unique_ptr<TextButton> layersButton;
    std::unique_ptr<TextButton> editButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerView)
};

} /* KSP1 */

//[EndFile] You can add extra defines here...

//[/EndFile]

