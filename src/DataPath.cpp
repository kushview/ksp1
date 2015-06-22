/*
    DataPath.cpp - This file is part of KSP1
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

#include "DataPath.h"

namespace KSP1 {

    DataPath::DataPath() { file = DataPath::defaultUserPath(); }
    DataPath::DataPath (const DataPath& other) : file (other.file) { }
    DataPath::DataPath (const File &srcDir) { file = srcDir.isDirectory() ? srcDir : srcDir.getParentDirectory(); }
    DataPath::~DataPath() { }

    const File&
    DataPath::defaultUserPath()
    {
        static File __userpath;
        if (__userpath == File::nonexistent)
        {
#if JUCE_IOS
            File d = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getSiblingFile("Documents");
            __userpath = d.getChildFile ("User Data");
            d = __userpath;
#elif JUCE_ANDROID
            
#else
            File d = DataPath::factoryContentPath();
            __userpath = d.getChildFile (String ("UserData"));

            d = __userpath;
#endif
            
            if (! d.exists())
            {
                d.createDirectory();
                const char* s[] = { "Instruments", "Samples", nullptr };
                StringArray subs (s);
                
                for (const auto& sub : subs)
                    d.getChildFile(sub).createDirectory();
            }
        }

        return __userpath;
    }

    const File&
    DataPath::defaultDatabaseFile()
    {
        static File __dbPath;
        if (File::nonexistent == __dbPath)
        {
            __dbPath = factoryContentPath().getChildFile ("library.db");
            __dbPath.getParentDirectory().createDirectory();
        }
        return __dbPath;
    }

    const File&
    DataPath::expansionContentPath()
    {
        static File exPath;
        if (! exPath.exists())
        {
            exPath = File::getSpecialLocation (File::userMusicDirectory)
                            .getChildFile("KSP1")
                            .getChildFile("Expansions");
        }

        return exPath;
    }

    const File& DataPath::factoryContentPath()
    {
        static File factoryPath;
        if (factoryPath == File::nonexistent)
        {
           #if JUCE_IOS
            File d = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getSiblingFile("Documents");
            __userpath = d.getChildFile ("User Data");
            d = __userpath;
           #elif JUCE_ANDROID

           #elif JUCE_MAC
            File d = File::getSpecialLocation (File::currentApplicationFile);
            factoryPath = d.getChildFile ("Contents/Resources");
           #endif
        }

        return factoryPath;
    }


    File DataPath::resolvePath (const String& path)
    {
        jassert (path.isNotEmpty());
        jassert (! File::isAbsolutePath (path));
        if (path.substring (0, 1) != "/")
            return factoryContentPath().getChildFile (path);

        File file (path);
        return file;
    }

    const File& DataPath::simBeatThangPath()
    {
        static File simBeatThangPath;
        if (simBeatThangPath == File::nonexistent)
        {
           #if JUCE_MAC
            File d (String ("/Users/Shared/SimBeatThang"));
           #elif JUCE_IOS
            File docs = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getSiblingFile("Documents");
            File d = docs.getChildFile ("SimBeatThang");
            d.createDirectory();
           #elif defined (_WIN32)
            File d;
            if (SystemStats::isOperatingSystem64Bit()) {
                d = "C:\\Program Files (x86)\\Beat Kangz\\Virtual Beat Thang";
            } else {
                d = "C:\\Program Files\\Beat Kangz\\Virtual Beat Thang";
            }
           #elif defined (_WIN64)
            File d = "C:\\Program Files\\Beat Kangz\\Virtual Beat Thang";
           #elif JUCE_LINUX
            File d = File::getSpecialLocation (File::userHomeDirectory);
            d = d.getChildFile (".btvpro/Legacy/SimBeatThang");
            d.createDirectory();
           #endif
            simBeatThangPath = d;
        }

        return simBeatThangPath;
    }
}
