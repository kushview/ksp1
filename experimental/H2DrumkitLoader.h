/*
    H2Drumkit.h - This file is part of KSP1
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

#ifndef KSP1_H2DRUMKIT_LOADER_H
#define KSP1_H2DRUMKIT_LOADER_H

namespace KSP1 {

class H2DrumkitLoader {
    SampleCache cache;
    SoundSet<SamplerSound> soundSet;

    File kitPath;
    int currentLayer;

    struct Meta {
        String name;
    } meta;

public:
    H2DrumkitLoader() : root (0), currentLayer (0) {}
    H2DrumkitLoader (const File& kit) : root (0), currentLayer (0) {
        loadKit (kit);
    }

    const String& name() const { return meta.name; }

    void loadKit (const File& kitFile) {
        kitPath   = kitFile;
        meta.name = String::empty;
        error     = String::empty;

        if (! kitPath.isDirectory()) {
            kitPath = File::nonexistent;
            error   = "Not a valid Hydrogen drumkit";
            return;
        }

        File drumkit (kitPath.getChildFile ("drumkit.xml"));
        if (! drumkit.existsAsFile()) {
            error = "Not a valid Hydrogen drumkit";
            return;
        }

        root.reset (XmlDocument::parse (drumkit));
        soundSet.clearSounds();

        error = root != 0 ? String::empty : "Could not parse Hydrogen drumkit";

        if (String::empty == error) {
            if (XmlElement* n = root->getChildByName ("name")) {
                meta.name = n->getAllSubText();
            }

            if (XmlElement* list = root->getChildByName ("instrumentList")) {
                forEachXmlChildElementWithTagName (*list, inst, "instrument") {
                    currentLayer = 0;

                    XmlElement* eId = inst->getChildByName ("id");
                    const int keyid = eId->getAllSubText().getIntValue();

                    const int slot = soundSet.findByRootNote (keyid);

                    if (slot < 0) {
                        //XX   Shared<SamplerSound> sound = SamplerSound::create (keyid);
                        //XX   soundSet.addSound (sound);
                    }

                    forEachXmlChildElement (*inst, child) {
                        if (child->getTagName() == "layer") {
                            handleLayer (keyid, *child);
                        } else
                            handleKeyProperty (keyid, *child);
                    }
                }
            }
#if 0
                BOOST_FOREACH (const Shared<SamplerSound>& snd, soundSet.getSounds())
                {
                    if (snd != 0)
                        snd->prepare();
                }
#endif
        }
    }

    bool ok() const { return error == String::empty; }
    String getError() { return error; }

    //fish::SoundSet<SamplerSound>& getSounds() { return soundSet; }

    void loadMedia() {
#if 0
            for (int i = soundSet.getSounds().size(); --i >= 0;)
            {
                Shared<SamplerSound> snd = soundSet.getSound(i);
                if (snd != 0)
                {
                    for (int j = snd->getNumLayers(); --j >= 0;)
                    {
                       Shared<SamplerSound::Layer> layer (snd->getLayer (j));
                       if (layer != 0)
                       {
                           //Shared<AudioSampleBuffer> audio (cache.load (layer->))
                       }
                    }
                }
            }
#endif
    }

private:
    String error;
    ScopedXml root;

    // typedef Shared<SamplerSound::Layer> SharedLayer;

    void handleLayer (const int keyid, XmlElement& e) {
        forEachXmlChildElement (e, child)
            handleLayerProperty (keyid, *child);
        ++currentLayer;
    }

    void handleLayerProperty (const int keyid, XmlElement& e) {
#if 0
            const int slot = soundSet.findByRootNote (keyid);
            assert (slot >= 0);

            const Shared<SamplerSound>& sound (soundSet.getSound (slot));
            Key& k (sound->getKey());

            Shared<SamplerSound::Layer> layer (sound->getLayer (currentLayer));
            if (layer == 0)
            {
                layer = SamplerSound::createLayer ((uint32_t) currentLayer);
                sound->setLayer (layer);
                layer = sound->getLayer ((uint32_t) currentLayer);
            }

            const String& tag (e.getTagName());

            if (tag == "filename")
            {
                File samp (kitPath.getChildFile (e.getAllSubText()));
                if (samp.existsAsFile())
                {
                    Shared<AudioSampleBuffer> audio = cache.load (samp);

                    if (audio)
                        layer->buffer = audio;
                    else
                        layer->buffer.reset (new AudioSampleBuffer (1, 1));
                }
            }
            else if (tag == "min")
            {
                layer->min = e.getAllSubText().getFloatValue();
            }
            else if (tag == "max")
            {
                layer->max = e.getAllSubText().getFloatValue();
            }
            else if (tag == "gain")
            {
                layer->volume = fish::gain_to_db (e.getAllSubText().getFloatValue(), -70.f);
            }
            else if (tag == "pitch")
            {
                layer->pitch = e.getAllSubText().getFloatValue();
            }
#endif
    }

    void handleKeyProperty (int keyid, XmlElement& e) {
#if 0
            const int slot = soundSet.findByRootNote (keyid);
            const Shared<SamplerSound>& key (soundSet.getSound (slot));
            Key& k (key->getKey());
            const String& tag (e.getTagName());

            if (tag == "id")
            {
                k.id = e.getAllSubText().getIntValue();

            }
            else if (tag == "name")
            {

            }
            else if (tag == "volume")
            {
                k.volume = e.getAllSubText().getFloatValue();
            }
            else if (tag == "isMuted")
            {
                k.muted = (e.getAllSubText() == "true");
            }
            else if (tag == "isMuted")
            {
                k.locked = (e.getAllSubText() == "true");
            }
            else if (tag == "gain")
            {
                k.gain =  e.getAllSubText().getFloatValue();
            }
            else if (tag == "randomPitchFactor")
            {
                k.randomPitch = e.getAllSubText().getFloatValue();
            }
            else if (tag == "Attack")
            {
                k.adsr.attack = e.getAllSubText().getFloatValue();
            }
            else if (tag == "Decay")
            {
                k.adsr.decay = e.getAllSubText().getFloatValue();
            }
            else if (tag == "Sustain")
            {
                k.adsr.sustain = e.getAllSubText().getFloatValue();
            }
            else if (tag == "Release")
            {
                k.adsr.release = e.getAllSubText().getFloatValue();
            }
            else if (tag == "muteGroup")
            {
                k.muteGroup = e.getAllSubText().getIntValue();
            }
            else if (tag == "filename")
            {
                handleLayerProperty (keyid, e);
                currentLayer++;
            }
#endif
    }
};

} // namespace KSP1
#endif // KSP1_H2DRUMKIT_LOADER_H
