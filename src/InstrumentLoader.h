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

#pragma once

#include "KSP1.h"

namespace juce {
    class File;
}


namespace KSP1 {

    /** A listener for progress reports from something */
    class ProgressSink {
    public:
        virtual ~ProgressSink() { }
        virtual void handleProgress (float p) = 0;
        virtual void handleStatus (const String& msg) = 0;
        virtual void handleProgressStart() { }
        virtual void handleProgressFinished() { }
    };

    class DummyProgress :  public ProgressSink {
    public:

        DummyProgress() { }
        ~DummyProgress() { }
        void handleProgress (float) override { }
        void handleStatus (const String&) override { }
    };


    class Instrument;

    class AssetLoader {
    public:

        AssetLoader()
            : statusMessage(),
              currentStep (0),
              usingSink (false) { }

        virtual ~AssetLoader() { }

        virtual void loadFile (const File& file) =0;
        virtual void loadResource (const String& res) = 0;
        virtual void loadXml (const XmlElement& e) { }

        virtual int getNumSteps() { return -1; }
        inline int getCurrentStep() const { return currentStep; }

        inline void tickProgress()
        {
            if (getNumSteps() < 0) {
                notifyProgress (-1.0f);
                return;
            }

            if (currentStep < 0)
                return;

            if (currentStep == 0) {
                currentStep = 1;
            }

            notifyProgress ((float) currentStep / (float) getNumSteps());

            if (++currentStep > getNumSteps()) {
                currentStep = -1;
            }
        }

        void setStatusMessage (const String& msg)
        {
            if (statusMessage != msg)
            {
                statusMessage = msg;
                notifyStatus (statusMessage);
            }
        }

        void startLoading()
        {
            notifyStart();
        }

        void finishedLoading()
        {
            notifyFinished();
        }

        template<class SinkType>
        inline void setSink (SinkType& sink)
        {
            if (! usingSink) {
                notifyProgress.connect (boost::bind (&SinkType::handleProgress, &sink, ::_1));
                notifyStatus.connect (boost::bind (&SinkType::handleStatus, &sink, ::_1));
                notifyStart.connect (boost::bind (&SinkType::handleProgressStart, &sink));
                notifyFinished.connect (boost::bind (&SinkType::handleProgressFinished, &sink));
                usingSink = true;
            }
        }

    private:

        boost::signals2::signal<void()> notifyStart;
        boost::signals2::signal<void()> notifyFinished;
        boost::signals2::signal<void(float)> notifyProgress;
        boost::signals2::signal<void(const String&)> notifyStatus;

        String statusMessage;
        int currentStep;
        bool usingSink;

    };

    class InstrumentLoader : public AssetLoader
    {
    public:

        InstrumentLoader (Instrument& i)
            : instrument (i)
        { }

    protected:

        Instrument& instrument;

    };

}
