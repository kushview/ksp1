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

#ifndef KSP1_BEATTHANG_PATTERN_LOADER_H
#define KSP1_BEATTHANG_PATTERN_LOADER_H
#if KSP1_SEQUENCER

#include "../PatternLoader.h"

namespace KSP1 {

class BeatThangPatternLoader :  public PatternLoader
{
public:

    BeatThangPatternLoader (Pattern& p, const KnownFilesystems& f)
        : PatternLoader (p, f) { }

    inline int getNumSteps() { return 16; }

    inline void
    loadFile (const File& file)
    {
        ScopedXml xml = XmlDocument::parse (file);
        if (xml)
            loadXml (*xml);
    }

    inline void
    loadXml (const XmlElement &e)
    {
        startLoading();
        {
            if (e.getTagName() == "Pattern")
            {
                // legacy patterns go top-down
/*
                <Notes>Created with the BeatThang</Notes>
                <Writable>true</Writable>
                <PPQ>96</PPQ>
                <Version>1</Version>
                <Tempo>95</Tempo>
                <Rec_Mode>0</Rec_Mode>
                <Swing>0</Swing>
                <Signature>3</Signature>
                <Pre_roll>1</Pre_roll>
                <Click_Vol>60</Click_Vol>
                <Click>1</Click>
                <Step_Div>1</Step_Div>
    */
                patsy.setProperty (Slugs::name, "Beat Thang Pattern");
                patsy.setProperty (Slugs::ppq, e.getChildByName("PPQ")->getAllSubText().getIntValue());
                patsy.setProperty (Slugs::bpm, e.getChildByName("Tempo")->getAllSubText().getFloatValue());
                patsy.setProperty (Slugs::version, e.getChildByName("Version")->getAllSubText().getIntValue());

                const int32 beatsPerBar = 4;
                const int32 ppqn = patsy.ppqn();

                if (XmlElement* list = e.getChildByName ("TrackList"))
                {
                    int32 trackIndex = 0;
                    forEachXmlChildElementWithTagName (*list, t, "Track")
                    {
                        this->midi.set (trackIndex, new MidiMessageSequence());
                        MidiMessageSequence& mseq = *midi.getUnchecked (trackIndex);
                        Pattern::Track track (patsy.getTrack (trackIndex));

                        int32 numBars = 0;
                        int32 totalLength = 0;

                        if (XmlElement* ble = t->getChildByName ("BarLength"))
                            numBars = ble->getAllSubText().getIntValue();

                        assert (numBars > 0);
                        totalLength = numBars * beatsPerBar;

                        forEachXmlChildElement (*t, el)
                        {
                            XmlElement* node = el;
                            if (node->getTagName() == "KitReference")
                            {
                                int32 parts = 0;
                                String data [2];

                                if (XmlElement* fs = node->getChildByName ("Filesystem"))
                                {
                                    if (XmlElement* fsid = fs->getChildByName ("FilesystemId"))
                                    {
                                        data[0] << fsid->getAllSubText();
                                        ++parts;
                                    }
                                }

                                if (XmlElement* path = node->getChildByName ("FilePath"))
                                {
                                    data[1] << path->getAllSubText();
                                    if (data[1].isNotEmpty())
                                        ++parts;
                                }

                                if (parts == 2)
                                {
                                    ValueTree tnode = track.state();
                                    ValueTree block (tnode.getOrCreateChildWithName (Slugs::block, nullptr));
                                    block.setProperty ("id", KSP1_SAMPLER_URI, nullptr);
                                    block.setProperty ("format", "Internal", nullptr);

                                    ValueTree asset (block.getOrCreateChildWithName (Slugs::asset, nullptr));
                                    asset.setProperty ("fsid", data[0], nullptr);
                                    asset.setProperty ("path", data[1], nullptr);
                                    asset.setProperty ("version", 1.0f, nullptr);
                                }
                            } // kit reference end
                            else if (node->getTagName() == "EventList")
                            {

                                // build a MidiMessageSequence, let it do the note sorting...
                                // MidiMessageSequence mseq;
                                forEachXmlChildElementWithTagName (*node, ev, "NoteEvent")
                                {
                                    XmlElement* event = ev;
                                    const int32 note      = event->getChildByName ("Note")->getAllSubText().getIntValue();
                                    const float velocity  = event->getChildByName ("Volume")->getAllSubText().getFloatValue();
                                    const double when     = Shuttle::scaledTick (event->getChildByName("When")->getAllSubText().getDoubleValue(), ppqn);

                                    MidiMessage msg = velocity > 0 ? MidiMessage::noteOn (1, note, velocity)
                                                                   : MidiMessage::noteOff (1, note);
                                    msg.setTimeStamp (when);
                                    mseq.addEvent (msg);
                                    mseq.updateMatchedPairs();
                                }

                                mseq.sort();
                                mseq.updateMatchedPairs();

                                Pattern::Events seq (patsy.trackEvents (trackIndex));
                                seq.addMidiMessageSequence (mseq);
                            }
                        }

                        tickProgress();
                        ++trackIndex;
                    }
                }
            }
        }

        finishedLoading();
    }

    inline void
    loadResource (const String& res)
    {
        if (FileHelpers::isAbsolutePath (res))
        {
            File file (res);
            loadFile (file);
        }
    }
};

}

#endif
#endif
