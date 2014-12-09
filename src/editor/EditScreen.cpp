/*
    Copyright 2014 Kushview, LLC.  All rights reserved
    This is an automatically generated GUI class created by the Introjucer!
*/

#include "editor/EditScreen.h"
#include "editor/ProgramsListBox.h"
#include "editor/SamplerDisplay.h"
#include "editor/SamplerView.h"
#include "editor/Screens.h"
#include "editor/QuickBrowser.h"
#include "Instrument.h"

namespace KSP1 {
namespace Gui {


class SoundsTimeline : public TimelineComponent
{
public:

    SoundsTimeline()
        : TimelineComponent()
    {
        setTrackWidth (64);
        const_cast<TimeScale&> (timeScale()).setPixelsPerBeat (32);
        const_cast<TimeScale&> (timeScale()).updateScale();
    }

    ~SoundsTimeline() { }

    void mouseDown (const MouseEvent& ev) {
        TimelineComponent::mouseDown (ev);
    }

    void timelineBodyClicked (const MouseEvent& ev, int track)
    {

    }

    int getNumTracks() const override
    {
        return (instrument) ? instrument->getNumChildren() : 0;
    }

    void setInstrument (InstrumentPtr ptr)
    {
        instrument = ptr;
        if (! instrument) {
            removeClips();
            return;
        }

        removeClips();

        ValueTree node = instrument->node();
        for (int i = 0; i < node.getNumChildren(); ++i)
        {
            if (! node.getChild(i).hasType(Tags::key))
                continue;

            KeyItem item (node.getChild(i));
            addTimelineClip (new KeyItemClip (*this, item));
        }

        setAllTrackHeights (18);
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

        g.drawText (txt, area, Justification::left);

    }

    void clipClicked (TimelineClip *clip, const MouseEvent& /*ev*/) override
    {
        clip->setSelected (! clip->isSelected());
        clip->repaint();
    }

private:
    InstrumentPtr instrument;
    ComponentDragger dragger;

    class KeyItemClip : public Element::TimelineClip
    {
    public:
        KeyItemClip (SoundsTimeline& timeline, const KeyItem& item)
            : Element::TimelineClip (timeline),
              key (item)
        {
        }

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
            key.node().setProperty (Slugs::note, roundDoubleToInt (loc.getStart()), 0);
            int32 len = roundDoubleToInt (loc.getLength() - 1.0);
            if (len < 0) {
                len = 0;
            }

            key.node().setProperty (Slugs::length, len, 0);
        }

        virtual TimeUnit getTimeUnit() const { return TimeUnit::Beats; }
        virtual int32 trackIndex() const
        {
            ValueTree node = key.node(); ValueTree parent = node.getParent();
            return (node.isValid() && parent.isValid()) ? parent.indexOf (node) : 0;
        }

        void paint (Graphics& g)
        {
            const int letterSpace = 30;
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


class LayersTimeline : public TimelineComponent {
public:

    LayersTimeline()
        : Element::TimelineComponent(),
          key (nullptr)
    {
        //setTrackHeightsOffset (120, false);
        setTrackWidth (128);
        const_cast<TimeScale&> (timeScale()).setPixelsPerBeat (640);
        const_cast<TimeScale&> (timeScale()).updateScale();
    }

    ~LayersTimeline() { }


    int getNumTracks() const override
    {
        return (key && key->isValid()) ? key->countLayers() : 0;
    }

    void setKey (const KeyItem& item)
    {
        key = new KeyItem (item);
        Instrument i (key->node().getParent());
        i.sortKeys();

        removeClips();
        DBG (this->getPixelOffset ());

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
    class LayerClip : public Element::TimelineClip
    {
    public:
        LayerClip (LayersTimeline& tl, const LayerItem& item)
            : Element::TimelineClip (tl),
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
                loc.setStart (layer.start());
                loc.setLength (layer.length());
                loc.setOffset ((double) layer.getProperty (Slugs::offset));
            }
        }

        virtual void setClipRange (const ClipRange<double>& loc) {
            layer.setStartTime (loc.getStart());
            layer.setEndTime (loc.getEnd());            
            layer.node().setProperty (Slugs::offset, (double) loc.getOffset(), 0);
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
                peak->drawChannels (g, r, start, end, 0.95f);
            }

            g.setColour (Colours::black);
            String txt ("Pos: ");
            txt << roundDoubleToInt (layer.offset() * timeline().timeScale().getSampleRate())
                << " -" << roundDoubleToInt (layer.start() * timeline().timeScale().getSampleRate())
                << " - " << roundDoubleToInt (layer.end() * timeline().timeScale().getSampleRate());
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


class EditScreen::Updater : public Timer
{
public:
    Updater (EditScreen& es)
        : screen (es)
    {
        setFrequecy (500);
        //startTimer (frequency);
    }

    ~Updater()
    {
        stopTimer();
    }

    void setFrequecy (int f)
    {
        const bool wasRunning = isTimerRunning();

        if (wasRunning)
            stopTimer();

        frequency = f;

        if (wasRunning)
            startTimer (frequency);
    }

    void timerCallback () {
        screen.updateComponents();
    }

private:

    EditScreen& screen;
    int frequency;

};


EditScreen::EditScreen (SamplerDisplay& owner)
    : Screen (owner, "Test Screen", Screen::editScreen)
{
    addAndMakeVisible (buttonAddSample = new TextButton ("add-sample-button"));
    buttonAddSample->setTooltip (TRANS("Add a Sample"));
    buttonAddSample->setButtonText (TRANS("+"));
    buttonAddSample->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    buttonAddSample->addListener (this);
    buttonAddSample->setColour (TextButton::buttonColourId, Colour (0xc7282828));
    buttonAddSample->setColour (TextButton::buttonOnColourId, Colour (0xe5cbcbcb));
    buttonAddSample->setColour (TextButton::textColourOnId, Colour (0xff858585));
    buttonAddSample->setColour (TextButton::textColourOffId, Colour (0xffe2e2e2));

    addAndMakeVisible (buttonRemoveLayer = new TextButton ("button-remove-layer"));
    buttonRemoveLayer->setTooltip (TRANS("Remove Sample"));
    buttonRemoveLayer->setButtonText (TRANS("-"));
    buttonRemoveLayer->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    buttonRemoveLayer->addListener (this);
    buttonRemoveLayer->setColour (TextButton::buttonColourId, Colour (0xc72d2d2d));
    buttonRemoveLayer->setColour (TextButton::buttonOnColourId, Colour (0xe5ededed));
    buttonRemoveLayer->setColour (TextButton::textColourOnId, Colour (0xff858585));
    buttonRemoveLayer->setColour (TextButton::textColourOffId, Colour (0xffe2e2e2));

    setSize (600, 400);

    lastNote = display().selectedKey().getNote();
    addPage ("Instrument", sounds = new SoundsTimeline());
    sounds->setInstrument (owner.getInstrument());
    addPage ("Key", timeline = new LayersTimeline());

    buttonAddSample->setAlwaysOnTop (true);
    buttonRemoveLayer->setAlwaysOnTop (true);
}

EditScreen::~EditScreen()
{
    timeline.release();
    sounds.release();
    updater = nullptr;
    buttonAddSample = nullptr;
    buttonRemoveLayer = nullptr;
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
    buttonAddSample->setBounds (getWidth() - 56, getHeight() - 18, 24, 16);
    buttonRemoveLayer->setBounds (getWidth() - 28, getHeight() - 18, 24, 16);
    Screen::resized();
}

void EditScreen::buttonClicked (Button* buttonThatWasClicked)
{
    KeyItem key (display().selectedKey());
    if (buttonThatWasClicked == buttonAddSample)
    {
        FileChooser chooser ("Open media", File (String ("./content")), "*.btdk;*.xml;*.wav;*.aiff;*.flac", false);
        if (chooser.browseForMultipleFilesToOpen())
        {
            for (const File& f : chooser.getResults())
            {
                if (f.getFileExtension() == ".xml" ||
                    f.getFileExtension() == ".btdk")
                {
                    ProgressSink& sink = display().progressBarSink();
                    if (SamplerView* view = getSamplerView()) {
                        view->loadFile (chooser.getResult());
                    }
                }
                else
                {
                    if (! key.isValid()) {
                        key = display().getInstrument()->getOrCreateKey (display().selectedNote());
                    }

                    if (key.isValid())
                    {
                        key.addLayer (f);
                    }
                    else
                    {
                        jassertfalse;
                    }
                }
            }
        }
    }
    else if (buttonThatWasClicked == buttonRemoveLayer)
    {

    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode]
void EditScreen::keySelectedEvent (const KeyItem& item)
{
    if (item.isValid())
    {
        if (timeline) timeline->setKey (item);
        if (sounds) sounds->setInstrument (display().getInstrument());
    }
}

void EditScreen::timerCallback()
{
}

void EditScreen::updateComponents()
{
}

void
EditScreen::onDisplayUpdate()
{
    // detect a note change from the display
    if (lastNote != display().selectedKey().getNote())
    {
        lastNote = display().selectedKey().getNote();
        timeline->setKey (display().selectedKey());
        sounds->setInstrument (display().getInstrument());
    }
}

//[/MiscUserCode]



}} /* namespace KSP1::Gui */
