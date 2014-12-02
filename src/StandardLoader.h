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

#ifndef KSP1_STANDARD_LOADER_H
#define KSP1_STANDARD_LOADER_H

#include "Instrument.h"
#include "InstrumentLoader.h"

namespace KSP1 {

    class XmlLoader  : public InstrumentLoader
    {
    public:

        XmlLoader (Instrument& i, ProgressSink& sink)
            : InstrumentLoader (i)
        {
            setSink (sink);
        }

        ~XmlLoader() { }

        virtual void loadFile (const File &file)
        {
            error = String::empty;
            ScopedXml xml (XmlDocument::parse (file));
            if (! xml)
            {
                error = "Could not parse instrument";
                return;
            }

            loadXml (*xml);
        }

        virtual void loadResource (const String &file)
        {
            loadFile (File (file));
        }

        void loadXml (const XmlElement& xml)
        {
            ValueTree data = ValueTree::fromXml (xml);
            if (! data.hasType ("instrument"))
                return;

            instrument.setData (data);
            finishedLoading();
            return;


            totalSteps = 0;

            for (int i = 0; i < data.getNumChildren(); ++i)
            {
                KeyItem key (data.getChild (i));
                if (! key.isValid())
                    continue;
                totalSteps += key.countLayers();
            }

            startLoading();

            for (int i = 0; i < data.getNumChildren(); ++i)
            {
                KeyItem sourceKey (data.getChild (i));
                if (! sourceKey.isValid())
                    continue;

                KeyItem targetKey (instrument.getKey (i));
                if (! targetKey.isValid())
                    continue;

                // XXX: Mutate the key ?? !!! ??

                for (int i = 0; i < sourceKey.node().getNumChildren(); ++i)
                {
                    LayerItem layer (sourceKey.node().getChild(i));
                    if (layer.isValid())
                    {
                        File f (layer.fileString());
                        setStatusMessage (f.getFileName());
                        targetKey.addLayer (f);
                        tickProgress();
                    }
                    else
                    {
                        setStatusMessage (String::empty);
                    }
                }
            }

            String name = data.getProperty (Slugs::name, String ("Empty Circuit"));
            instrument.setName (name);
            finishedLoading();
        }

    private:

        String error;
        int totalSteps;

    };

    class StandardLoader  : public XmlLoader
    {
    public:

        StandardLoader (Instrument& i, ProgressSink& sink)
            : XmlLoader (i, sink)
        { }

        ~StandardLoader() { }

        void loadFile (const File &file)
        {
            error = String::empty;
            ScopedXml xml (XmlDocument::parse (file));
            if (! xml)
            {
                error = "Could not parse instrument";
                return;
            }

            loadXml (*xml);
        }

    private:
        String error;
    };
}

#endif // KSP1_STANDARD_LOADER_H
