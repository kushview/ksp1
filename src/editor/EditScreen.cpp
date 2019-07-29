/*
    Copyright 2014 Kushview, LLC.  All rights reserved
    This is an automatically generated GUI class created by the Introjucer!
*/

#include "KSP1.h"
#include "editor/EditScreen.h"
#include "editor/SamplerDisplay.h"
#include "editor/SamplerView.h"
#include "editor/Screens.h"
#include "Instrument.h"

using namespace kv;

namespace KSP1 {

class SoundsTimeline : public TimelineComponent
{
public:
    SoundsTimeline()
        : TimelineComponent()
    {
        setTrackWidth (100);
        setIndicator (nullptr);
        const_cast<kv::TimeScale&> (timeScale()).setPixelsPerBeat (24);
        const_cast<kv::TimeScale&> (timeScale()).updateScale();
    }

    ~SoundsTimeline() { }

    void mouseDown (const MouseEvent& ev) override
    {
        TimelineComponent::mouseDown (ev);
    }

    void timelineBodyClicked (const MouseEvent& ev, int track) override
    {
        if (instrument && isPositiveAndBelow (track, instrument->getNumSounds()))
            instrument->setActiveSoundIndex (track);
    }

    void timelineTrackHeadersClicked (const MouseEvent&, int track) override
    { 
        if (instrument && isPositiveAndBelow (track, instrument->getNumSounds()))
            instrument->setActiveSoundIndex (track);
    }

    void clipClicked (kv::TimelineClip* clip, const MouseEvent&) override
    {
        if (instrument && isPositiveAndBelow (clip->trackIndex(), instrument->getNumSounds()))
            instrument->setActiveSoundIndex (clip->trackIndex());
    }

    int getNumTracks() const override
    {
        return (instrument) ? instrument->getNumSounds() : 0;
    }

    void refresh()
    {
        removeClips();

        if (auto* view = findParentComponentOfClass<SamplerView>())
            instrument = view->getInstrument();

        if (! instrument)
            return;
        
        ValueTree node = instrument->getValueTree();
        for (int i = 0; i < node.getNumChildren(); ++i)
        {
            if (! node.getChild(i).hasType (Tags::key))
                continue;

            KeyItem item (node.getChild (i));
            addTimelineClip (new KeyItemClip (*this, item), i);
        }

        setAllTrackHeights (18);
        resized();
        repaint();
    }

    void paintTrackHeader (Graphics& g, int track, const Rectangle<int>& area) override
    {
        g.setColour (Colours::lightgrey);
        g.fillRect (area);

        g.setColour (Colours::black);
        String txt;
#if 0
        if (KeyItemClip* clip = dynamic_cast<KeyItemClip*> (getFirstClipOnTrack (track)))
        {
            txt << MidiMessage::getMidiNoteName (clip->key.getNote(), true, true, 3)
                << " - " <<  MidiMessage::getMidiNoteName (clip->key.getNote() + (int) clip->key.getProperty(Slugs::length), true, true, 3);
        }
        else
#endif
        {
            txt << "Sound " << track;
        }

        g.setFont (9.f);
        g.drawText (txt, area.withX(10).withWidth(area.getWidth() - 10), Justification::centredLeft);
    }

private:
    InstrumentPtr instrument;
    ComponentDragger dragger;

    class KeyItemClip : public kv::TimelineClip
    {
    public:
        KeyItemClip (SoundsTimeline& timeline, const KeyItem& item)
            : kv::TimelineClip (timeline),
              key (item) { }

        virtual bool moveable() const { return true; }
        virtual bool resizable() const { return true; }
        virtual bool timeIsMusical() const { return false; }

        virtual void getClipRange (ClipRange<double>& loc)
        {
            if (key.isValid())
            {
                loc.setStart ((double) key.getNote());
                loc.setLength ((double) (int) key.getProperty (Slugs::length) + 1);
                loc.setOffset (0);
            }
        }

        virtual void setClipRange (const ClipRange<double>& loc)
        {
            int note = jmax (0, roundToInt (loc.getStart()));
            int len =  jmax (0, roundToInt (loc.getLength() - 1.0));

            if (isPositiveAndBelow (note, 127))
            {
                key.setProperty (Slugs::note, note);
                key.setProperty (Slugs::length, len);
            }
        }

        virtual TimeUnit getTimeUnit() const { return TimeUnit::Beats; }
        virtual int32 trackIndex() const
        {
            ValueTree node = key.node(); ValueTree parent = node.getParent();
            return (node.isValid() && parent.isValid()) ? parent.indexOf (node) : 0;
        }

        void paint (Graphics& g)
        {
            const int letterSpace = 24;
            String n1 (MidiMessage::getMidiNoteName (key.getNote(), true, true, 3));
            String n2 (MidiMessage::getMidiNoteName (key.getNote() + (int) key.getProperty(Slugs::length), true, true, 3));

            if (isSelected())
                g.setColour (Colours::teal);
            else
                g.setColour (Colours::white);

            g.fillAll();

            g.setColour (Colours::black);
            g.drawRect(0, 0, getWidth(), getHeight(), 1);

            g.setColour (Colours::black);
            g.setFont (10.f);
            g.drawText (n1, 0, 0, letterSpace, getHeight(), Justification::centred);
            if ((int) key.getProperty (Slugs::length) > 0)
            {
                g.drawText (n2, getWidth() - (letterSpace + 1), 0,
                            letterSpace, getHeight(), Justification::centred);
            }
        }

        KeyItem key;

    protected:
        virtual void reset()
        {

        }

        virtual void selectedStateChanged()
        {

        }

        virtual int32 trackRequested (const int32 track)
        {
            return trackIndex();
        }
    };
};

//=============================================================================
class LayersTimeline : public kv::TimelineComponent
{
public:

    LayersTimeline()
        : kv::TimelineComponent(),
          key (nullptr)
    {
        setTrackWidth (100);
        const_cast<kv::TimeScale&> (timeScale()).setPixelsPerBeat (640);
        const_cast<kv::TimeScale&> (timeScale()).updateScale();
    }

    ~LayersTimeline() { }

    int getNumTracks() const override
    {
        return (key && key->isValid()) ? key->countLayers() : 0;
    }

    void mouseWheelMove (const MouseEvent& ev, const MouseWheelDetails& md) override
    {
        auto& ts = const_cast<kv::TimeScale&> (timeScale());

        if (md.deltaY > 0.0)
            ts.setHorizontalZoom (ts.horizontalZoom() + 10);
        else if (md.deltaY < 0.0 && ts.horizontalZoom() >= 10)
            ts.setHorizontalZoom (ts.horizontalZoom() - 10);

        ts.updateScale();
        triggerAsyncUpdate();
    }

    void setKey (const KeyItem& item)
    {
        key = new KeyItem (item);
        Instrument i (key->node().getParent());
        i.sortKeys();

        removeClips();

        if (key && key->isValid()) {
            for (int i = key->countLayers(); --i >= 0;)
                addTimelineClip (new LayerClip (*this, key->getLayer(i)));
        }

        setAllTrackHeights (TrackHeights::Large);
        repaint();
    }

    void paintTrackHeader (Graphics& g, int track, const Rectangle<int>& area) override
    {
        g.setColour (Colours::lightgrey);
        g.fillRect (area);

        g.setColour (Colours::black);
        String txt ("  Layer "); txt << track + 1;
        g.drawText (txt, area, Justification::centred);
    }

    AudioPeakFactory& getPeaks() { return peaks; }

private:
    AudioPeakFactory peaks;
    ScopedPointer<KeyItem> key;
    class LayerClip : public kv::TimelineClip
    {
    public:
        LayerClip (LayersTimeline& tl, const LayerItem& item)
            : kv::TimelineClip (tl),
              layer (item)
        {
            const File file (item.fileString());
            if (file.existsAsFile())
            {
                peak = new AudioPeak (tl.getPeaks());
                peak->setSource (new FileInputSource (file));
            }
        }

        virtual bool moveable() const { return true; }
        virtual bool resizable() const { return true; }
        virtual bool timeIsMusical() const { return false; }

        virtual void getClipRange (ClipRange<double>& loc)
        {
            if (layer.isValid())
            {
                loc.setStart (layer.getStart());
                loc.setLength (layer.getLength());
                loc.setOffset ((double) layer.getProperty (Slugs::offset));
            }
        }

        virtual void setClipRange (const ClipRange<double>& loc)
        {
            layer.setProperty (Slugs::offset, loc.getOffset());
            layer.setProperty (Slugs::start,  loc.getStart());
            layer.setProperty (Slugs::length, loc.getLength());
        }

        virtual TimeUnit getTimeUnit() const { return TimeUnit::Seconds; }
        virtual int32 trackIndex() const { return (int32) layer.getProperty (Slugs::index); }

        void paint (Graphics& g)
        {
            g.setColour (Colours::white);
            g.fillAll();

            //g.setColour (Colours::lightgrey);
            //g.drawLine (0, getHeight() / 2, getWidth(), getHeight() / 2, 1.0);

            if (peak)
            {
                ClipRange<double> range; getClipRange (range);
                double start = range.getOffset();
                double end = range.getOffset() + range.getLength();
                Rectangle<int> r (getLocalBounds());

                if (start < 0.0)
                {
                    r.setX (timeline().timeToWidth (std::fabs (start)));
                    r.setWidth (r.getWidth() - r.getX());
                    start = 0.0;
                }

                g.setColour (Colours::red);
                peak->drawChannels (g, r, start, end, 0.90f);
            }

            g.setColour (Colours::black);
            String txt ("Pos: ");
            #if 0
            // FIXME
            txt << roundDoubleToInt (layer.offset() * timeline().timeScale().getSampleRate())
                << " : " << roundDoubleToInt (layer.start() * timeline().timeScale().getSampleRate())
                << " : " << roundDoubleToInt (layer.end() * timeline().timeScale().getSampleRate());
            #endif
            g.drawText (txt, 1, 1, getWidth(), getHeight(), Justification::topLeft);
        }

        LayerItem layer;

    protected:

        virtual void reset()
        {

        }

        virtual void selectedStateChanged()
        {

        }

        virtual int32 trackRequested (const int32 track)
        {
            return trackIndex();
        }

    private:
        AudioPeakPtr peak;
    };

};

//=============================================================================
EditScreen::EditScreen (SamplerDisplay& owner)
    : Screen (owner, "Edit Screen", Screen::editScreen)
{
    addAndMakeVisible (addButton);
    addButton.setTooltip (TRANS("Add a Sample"));
    addButton.setButtonText (TRANS("+"));
    addButton.setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    addButton.addListener (this);
    addButton.setColour (TextButton::buttonColourId, Colour (0xc7282828));
    addButton.setColour (TextButton::buttonOnColourId, Colour (0xe5cbcbcb));
    addButton.setColour (TextButton::textColourOnId, Colour (0xff858585));
    addButton.setColour (TextButton::textColourOffId, Colour (0xffe2e2e2));

    addAndMakeVisible (removeButton);
    removeButton.setTooltip (TRANS("Remove Sample"));
    removeButton.setButtonText (TRANS("-"));
    removeButton.setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    removeButton.addListener (this);
    removeButton.setColour (TextButton::buttonColourId, Colour (0xc72d2d2d));
    removeButton.setColour (TextButton::buttonOnColourId, Colour (0xe5ededed));
    removeButton.setColour (TextButton::textColourOnId, Colour (0xff858585));
    removeButton.setColour (TextButton::textColourOffId, Colour (0xffe2e2e2));

    addChildComponent (properties);

    setSize (600, 400);

    lastNote = display().selectedKey().getNote();

    setTabOrientation (TabbedButtonBar::TabsAtBottom);
    sounds.reset (new SoundsTimeline());
    addPage ("Sounds", sounds.get());
    timeline.reset (new LayersTimeline());
    addPage ("Layers", timeline.get());

    addButton.setAlwaysOnTop (true);
    removeButton.setAlwaysOnTop (true);

    onPageChanged = [this]
    {
        switch (getCurrentPage())
        {
            case 0:
                sounds->refresh();
                break;
            case 1:
            {
                if (auto i = display().getInstrument())
                    timeline->setKey (i->getActiveSound());
            } break;
        }
    };
}

EditScreen::~EditScreen()
{
    timeline.reset();
    sounds.reset();
}

void EditScreen::setPropertiesVisible (bool visible)
{
    if (properties.isVisible() == visible)
        return;
    
    properties.setVisible (visible);
    resized();
}

void EditScreen::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    Screen::paint (g);
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EditScreen::resized()
{
    Screen::resized();
    auto tabBounds (getTabs().getBoundsInParent());

    if (properties.isVisible())
    {
        properties.setBounds (tabBounds.removeFromRight (140));
    }

    getTabs().setBounds (tabBounds);

    addButton.setBounds (getWidth() - 56, getHeight() - 18, 24, 16);
    removeButton.setBounds (getWidth() - 28, getHeight() - 18, 24, 16);
    
}

void EditScreen::parentHierarchyChanged()
{
    sounds->refresh();
    if (auto i = display().getInstrument())
        timeline->setKey (i->getActiveSound());
}

void EditScreen::buttonClicked (Button* buttonThatWasClicked)
{
    KeyItem key (display().getInstrument()->getActiveSound());

    if (buttonThatWasClicked == &addButton)
    {
        if (getCurrentPage() == 1)
        {
            auto dir = File::getSpecialLocation(File::userHomeDirectory).getChildFile("Downloads");
            FileChooser chooser ("Open media", dir, "*.btdk;*.xml;*.wav;*.aiff;*.flac", false);

            if (chooser.browseForMultipleFilesToOpen())
            {
                for (const File& f : chooser.getResults())
                {
                    if (f.getFileExtension() == ".xml" ||
                        f.getFileExtension() == ".btdk")
                    {
                        if (SamplerView* view = getSamplerView()) {
                            view->loadFile (chooser.getResult());
                        }
                    }
                    else
                    {
                        if (key.isValid())
                        {
                            key.addLayer (f);
                            timeline->setKey (key);
                        }
                        else
                        {
                            jassertfalse;
                        }
                    }
                }
            }
        }
        else
        {
            auto instrument = display().getInstrument();
            instrument->addKey (display().selectedNote());
            sounds->refresh();
        }
    }
    else if (buttonThatWasClicked == &removeButton)
    {
        switch (getCurrentPage())
        {
            case 0:
            {
                auto instrument = display().getInstrument();
                auto sound = instrument->getActiveSound();
                instrument->removeSound (sound);
                sounds->refresh();
            } break;
            
            case 1:
            {

            } break;
        }
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode]
void EditScreen::keySelectedEvent (const KeyItem& item)
{
}

void EditScreen::timerCallback()
{
}

void EditScreen::updateComponents()
{
}

void EditScreen::onDisplayUpdate()
{
    // detect a note change from the display
    if (lastNote != display().selectedKey().getNote())
    {
        lastNote = display().selectedKey().getNote();
        timeline->setKey (display().selectedKey());
    }
}

//[/MiscUserCode]

}
