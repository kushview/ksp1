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

#ifndef KSP1_BEATTHANG_KIT_LOADER_H
#define KSP1_BEATTHANG_KIT_LOADER_H

#include "Filesystem.h"
#include "Instrument.h"
#include "InstrumentLoader.h"

namespace KSP1 {

class BeatThangKitLoader : public InstrumentLoader {
    OptionalScopedPointer<KnownFilesystems> fses;
    int currentLayer;

public:
    BeatThangKitLoader (Instrument& i)
        : InstrumentLoader (i) {
        fses.setOwned (new KnownFilesystems());
        FilesystemScanner scan (*fses, fses->searchPath());
        totalSteps   = 0;
        currentLayer = 0;
    }

    BeatThangKitLoader (Instrument& i, const KnownFilesystems& fs)
        : InstrumentLoader (i) {
        fses.setNonOwned (const_cast<KnownFilesystems*> (&fs));
        totalSteps   = 0;
        currentLayer = 0;
    }

    BeatThangKitLoader (Instrument& i, ProgressSink& s)
        : InstrumentLoader (i) {
        fses.setOwned (new KnownFilesystems());
        { FilesystemScanner (*fses, fses->searchPath()); }

        totalSteps   = 0;
        currentLayer = 0;

        setSink (s);
    }

    void loadResource (const String& res) {
        File f (res);
        loadFile (f);
    }

    inline int getNumSteps() {
        return totalSteps;
    }

    inline void
        loadFile (const File& kitFile) {
        root  = XmlDocument::parse (kitFile);
        error = root != 0 ? String::empty : "Could not read kit file";
        if (String::empty == error) {
            loadXml (*root);
        }
    }

    inline void
        loadXml (const XmlElement& data) {
        startLoading();

        if (String::empty == error) {
            if (XmlElement* name = data.getChildByName ("name")) {
                String fname (name->getAllSubText());
                instrument.setName (fname.upToLastOccurrenceOf (".", false, true));
            } else {
                instrument.setName ("Unknown");
            }

            if (XmlElement* list = data.getChildByName ("instrumentList")) {
                // count the number of layers total
                totalSteps = 0;
                forEachXmlChildElementWithTagName (*list, inst, "instrument") {
                    XmlElement* eId = inst->getChildByName ("id");
                    if (! eId)
                        continue;
                    forEachXmlChildElement (*inst, child) {
                        if (child->getTagName() == "layer")
                            ++totalSteps;
                    }
                }

                forEachXmlChildElementWithTagName (*list, inst, "instrument") {
                    XmlElement* eId = inst->getChildByName ("id");
                    if (! eId)
                        continue;

                    const int id = eId->getAllSubText().getIntValue();
                    KeyItem key (instrument.addKey (id));

                    forEachXmlChildElement (*inst, child) {
                        if (child->getTagName() == "layer") {
                            handleLayer (key, *child);
                            ++currentLayer;
                        } else {
                            handleKeyProperty (key, *child);
                        }
                    }
                }
            }
        }

        finishedLoading();
    }

    bool ok() const { return error == String::empty; }
    String getError() { return error; }

private:
    String error;
    ScopedXml root;

    void handleLayer (KeyItem& key, XmlElement& e) {
        LayerItem layer;

        forEachXmlChildElement (e, child) {
            if (child->getTagName() == "SampleFile") {
                const String fsid (child->getChildByName ("Filesystem")->getChildByName ("FilesystemId")->getAllSubText());
                const String path = child->getChildByName ("FilePath")->getAllSubText();

                File sampleFile = fses->getFile (fsid, path);
                if (sampleFile.existsAsFile()) {
                    //setStatusMessage (sampleFile.getFileName());
                    layer.setFile (sampleFile);
                }
            } else {
                //setStatusMessage (String (""));
                handleLayerProperty (layer, *child);
            }
        }

        key.addLayer (layer); // commit this layer with the key
        //tickProgress();
    }

    inline void
        handleLayerProperty (LayerItem& layer, XmlElement& e) {
        const String& tag (e.getTagName());
        const double sampleRate = 44100.0f; // I assure you this is correct
                                            // for all BKE instruments - MRF
        if (tag == "sampleStartFrame") {
            const double start = e.getAllSubText().getDoubleValue();
            layer.setStartTime (start / sampleRate);
        } else if (tag == "sampleEndFrame") {
            const double end = e.getAllSubText().getDoubleValue();
            layer.setEndTime (end / sampleRate);
        } else if (tag == "min") {
            const double doubleValue = e.getAllSubText().getDoubleValue();
            layer.setProperty (Tags::velocityLower, doubleValue);
        } else if (tag == "max") {
            const double doubleValue = e.getAllSubText().getDoubleValue();
            layer.setProperty (Tags::velocityUpper, doubleValue);
        } else if (tag == "pan_L") {
            const double doubleValue = e.getAllSubText().getDoubleValue();
            layer.setProperty (Tags::panning, doubleValue);
        } else if (tag == "pan_R") {
            //const double doubleValue = e.getAllSubText().getDoubleValue();
            //layer.setProperty ("panning-right", doubleValue);
        } else if (tag == "filterActive") {
            const bool boolValue = (e.getAllSubText() == "true");
            layer.setProperty ("filter-active", boolValue);
        } else if (tag == "filterCutoff") {
            const double doubleValue = e.getAllSubText().getDoubleValue();
            layer.setProperty ("cutoff", doubleValue);
        } else if (tag == "filterResonance") {
            const double doubleValue = e.getAllSubText().getDoubleValue();
            layer.setProperty ("resonance", doubleValue);
        } else if (tag == "pitch") {
            const double doubleValue = e.getAllSubText().getDoubleValue();
            layer.setProperty ("pitch", doubleValue);
        }
    }

    inline void
        handleKeyProperty (KeyItem& key, XmlElement& e) {
        const String& tag (e.getTagName());

        if (tag == "id") {
            key.node().setProperty (Slugs::note, e.getAllSubText().getIntValue(), nullptr);
        } else if (tag == "name") {
            key.node().setProperty (Slugs::name, e.getAllSubText(), nullptr);
        } else if (tag == "volume") {
            const double value = Decibels::gainToDecibels (e.getAllSubText().getDoubleValue());
            key.node().setProperty (Slugs::volume, value, nullptr);
        } else if (tag == "isMuted") {
            key.node().setProperty ("muted", (e.getAllSubText() == "true"), nullptr);
        } else if (tag == "randomPitchFactor") {
            //
            // key.node().setProperty ("randomPitch", e.getAllSubText().getDoubleValue(), nullptr);
        } else if (tag == "gain") {
            // the tag is 'gain' but it's really a volume value in decibels
            // key.node().setProperty (KSP1::volumeSlug, e.getAllSubText().getDoubleValue(), nullptr);
        } else if (tag == "VerbSend") {
            key.node().setProperty (KSP1::fxSendIdentifier (1), e.getAllSubText().getDoubleValue(), nullptr);
        } else if (tag == "EchoSend") {
            key.node().setProperty (KSP1::fxSendIdentifier (2), e.getAllSubText().getDoubleValue(), nullptr);
        } else if (tag == "FreakSend") {
            key.node().setProperty (KSP1::fxSendIdentifier (3), e.getAllSubText().getDoubleValue(), nullptr);
        } else if (tag == "EqSend") {
            key.node().setProperty (KSP1::fxSendIdentifier (4), e.getAllSubText().getDoubleValue(), nullptr);
        } else if (tag == "CompressorSend") {
            key.node().setProperty (KSP1::fxSendIdentifier (5), e.getAllSubText().getDoubleValue(), nullptr);
        } else if (tag == "Attack") {
            const double value = e.getAllSubText().getDoubleValue();
            key.node().setProperty (Slugs::attack, value / 44100.0f, nullptr);
        } else if (tag == "Decay") {
            const double value = e.getAllSubText().getDoubleValue();
            key.node().setProperty (Slugs::decay, value / 44100.0f, nullptr);
        } else if (tag == "Sustain") {
            const double value = e.getAllSubText().getDoubleValue();
            key.node().setProperty (Slugs::sustain, value, nullptr);
        } else if (tag == "Release") {
            const double value = e.getAllSubText().getDoubleValue();
            key.node().setProperty (Slugs::release, value / 44100.0f, nullptr);
        } else if (tag == "trigger_mode") {
            int32 triggerMode = e.getAllSubText().getIntValue();

            if (triggerMode > TriggerMode::OneShotGate) {
                triggerMode = TriggerMode::OneShotGate;
            } else if (triggerMode < 0) {
                triggerMode = TriggerMode::Retrigger;
            }

            key.node().setProperty (Tags::triggerMode, triggerMode, nullptr);
        } else if (tag == "muteGroup") {
            const int32 g = e.getAllSubText().getIntValue();
            key.node().setProperty (Tags::voiceGroup, g, nullptr);
        }
    }

private:
    int totalSteps;
};

} // namespace KSP1

#endif // KSP1_BEATTHANG_KIT_LOADER_H
