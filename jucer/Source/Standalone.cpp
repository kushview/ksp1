/*
    Standalone.cpp - This file is part of KSP1
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

#if 0
#if KSP1_STANDALONE

#include "KSP1.h"
#include "editor/SamplerView.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace KSP1 {

    class Standalone :  public JUCEApplication {
    public:
        LookAndFeel_V3 style;
        static Standalone* instance()
        {
            return dynamic_cast<Standalone*> (JUCEApplication::getInstance());
        }

        const String getApplicationName() { return ProjectInfo::projectName; }
        const String getApplicationVersion() { return ProjectInfo::versionString; }

        PluginProcessor* getProcessor()
        {
            return (nullptr != player) ? dynamic_cast<PluginProcessor*> (player->getCurrentProcessor())
                                       : nullptr;
        }

        void initialise (const String &commandLine) override
        {
            globals = new Globals();
            globals->initialize();

            PropertiesFile::Options opts;
            opts.applicationName = "KSP1";
            opts.commonToAllUsers = true;
            opts.filenameSuffix = ".conf";
            opts.folderName = ".KSP1";
            opts.storageFormat = PropertiesFile::storeAsCompressedBinary;
            settings.setStorageParameters (opts);

            LookAndFeel::setDefaultLookAndFeel (&style);
            devices = new AudioDeviceManager ();
            KSP1::PluginProcessor* proc = new KSP1::PluginProcessor (*globals);
            player = new AudioProcessorPlayer ();
            devices->addAudioCallback (player);
            devices->addMidiInputCallback (String::empty, player);
            player->setProcessor (proc);

            if (AudioProcessorEditor* editor = proc->createEditorIfNeeded())
            {
                window = new MainWindow (editor);
                window->setVisible(true);
                editor->setVisible(true);

                OwnedArray<AudioIODeviceType> types;
                devices->createAudioDeviceTypes (types);

                PropertiesFile* file = settings.getCommonSettings (true);
                ScopedPointer<XmlElement> xml = file->getXmlValue ("devices");

                devices->initialise (0, 2, xml, true);

                const String path (settings.getCommonSettings(true)->getValue ("lastInstrument"));
                if (path.isNotEmpty())
                {
                    InstrumentPtr i = proc->getInstrument();
                    const File file (path);
                    DummyProgress sink;
                    i->load (file, sink);
                    proc->getSampler()->currentSynth()->setInstrument (i);
                    proc->getSampler()->currentSynth()->reloadState();
                }
            }
            else
            {
                quit();
            }
        }

        void showAudioDevices()
        {
            if (dwin == nullptr)
            {
                dwin = new DevicesWindow (this->devices);
            }

            dwin->setVisible (true);
            dwin->addToDesktop();
        }

        void hideAudioDevices()
        {
            if (! dwin)
                return;

            dwin->setVisible (false);
            dwin->removeFromDesktop();
        }

        void shutdown() override
        {
            hideAudioDevices();
            dwin = nullptr;
            window = nullptr;

            if (devices)
            {
                devices->removeAudioCallback (player);
                devices->removeMidiInputCallback ("default", player);
                if (ScopedPointer<XmlElement> xml = devices->createStateXml())
                    settings.getCommonSettings(true)->setValue ("devices", xml);
                devices = nullptr;
            }

            if (player)
            {
                PluginProcessor* proc = dynamic_cast<PluginProcessor*> (player->getCurrentProcessor());
                player->setProcessor (nullptr);
                if (proc)
                {
                    settings.getCommonSettings(true)->setValue ("lastInstrument", proc->getInstrument()->getFile().getFullPathName());
                    delete proc;
                    proc = nullptr;
                }
                player = nullptr;
            }

            settings.saveIfNeeded();
        }

        InstrumentPtr getInstrument()
        {
            if (PluginProcessor* plug = getProcessor())
                return plug->getInstrument();
            return nullptr;
        }

    private:
        ApplicationProperties settings;
        ScopedPointer<Globals> globals;
        ScopedPointer<DocumentWindow> dwin;
        ScopedPointer<AudioDeviceManager> devices;
        ScopedPointer<AudioProcessorPlayer> player;

        class MainWindow :  public DocumentWindow,
                            public MenuBarModel
        {
        public:

            enum MenuIndex {
                FileNewInstrument = 1,
                FileOpenInstrument,
                FileSaveInstrumentAs,
                FileConsolodate,
                FileSaveInstrument,
                FileInstrumentStats,
                FileTransposeUp,
                FileTransposeDown,
                FileDeviceSettings
            };

            explicit MainWindow (Component* editor)
                : DocumentWindow ("KSP-1 Sampler", Colours::darkgrey, DocumentWindow::closeButton)
            {
                setUsingNativeTitleBar (true);
                setResizable (true, false);
                setContentOwned (editor, true);
                setMenuBar (this);
            }

            ~MainWindow() {
                setMenuBar (nullptr);
            }

            void closeButtonPressed() {
                Standalone::instance()->quit();
            }

            virtual StringArray getMenuBarNames() {
                const char* names[] = { "File", nullptr };
                return StringArray (names);
            }

            virtual PopupMenu getMenuForIndex (int topLevelMenuIndex,
                                               const String& menuName)
            {
                PopupMenu menu;
                if (menuName == "File")
                {
                    menu.addItem (FileNewInstrument, "New Instrument");
                    menu.addItem (FileOpenInstrument, "Open Instrument");
                    menu.addItem (FileSaveInstrument, "Save Instrument");
                    menu.addItem (FileSaveInstrumentAs, "Save Instrument As...");
                    menu.addItem (FileConsolodate, "Consolodate Instrument");
                    menu.addSeparator();
                    menu.addItem (FileInstrumentStats, "Instrument Stats");
                    menu.addItem (FileTransposeUp, "Transpose Up");
                    menu.addItem (FileTransposeDown, "Transpose Down");

                    menu.addSeparator();
                    menu.addItem (FileDeviceSettings, "Device Settings");
                }
                return menu;
            }

            virtual void menuItemSelected (int menuItemID,
                                           int topLevelMenuIndex)
            {
                if (FileNewInstrument == menuItemID && topLevelMenuIndex == 0)
                {
                    InstrumentPtr inst = new Instrument ("New Instrument");
                    Standalone::instance()->getProcessor()->getSampler()->setInstrument (inst);
                    Standalone::instance()->getProcessor()->getSampler()->currentSynth()->reloadState();
                    Gui::PluginEditor* ed = dynamic_cast<Gui::PluginEditor*> (getContentComponent());
                    ed->setInstrument (inst);
                }
                else if (FileConsolodate == menuItemID && topLevelMenuIndex == 0) {
                     InstrumentPtr inst (Standalone::instance()->getInstrument());
                     FileChooser chooser ("Consolodate Instrument", File (String ("./content")), "*", false);
                     if (chooser.browseForDirectory())
                     {
                        DBG (chooser.getResult().getFullPathName());
                        const File target (chooser.getResult());
                        const File samplesDir (target.getChildFile("Samples"));
                        const File instsDir (target.getChildFile("Instruments"));
                        const File instFile (instsDir.getChildFile (inst->getName() + ".xml"));
                        samplesDir.createDirectory();
                        instsDir.createDirectory();

                        StringArray samples;
                        inst->getSamples (samples);
                        for (const String& s : samples)
                        {
                            const File src (s);
                            const File dst (samplesDir.getChildFile (src.getFileName()));

                            DBG (dst.getRelativePathFrom (instFile));
                            if (! dst.exists())
                                src.copyFileTo (dst);
                        }

                        if (XmlElement* xml = inst->createXml())
                        {
                            xml->writeToFile (instFile, String::empty);
                            delete xml;
                        }
                     }
                }
                else if (FileOpenInstrument == menuItemID && topLevelMenuIndex == 0)
                {
                    InstrumentPtr inst (Standalone::instance()->getInstrument());
                    StringArray samples;
                    inst->getSamples(samples);
                    for (const String& s : samples) {
                        const File f (s);
                        const String uri (String ("ksp1://factory/Samples") + "/" + f.getFileName());
                        DBG (uri);
                    }
                }
                else if (FileSaveInstrument == menuItemID && topLevelMenuIndex == 0) {

                }
                else if (FileSaveInstrumentAs == menuItemID && topLevelMenuIndex == 0)
                {
                     FileChooser chooser ("Save Instrument", File (String ("./content")), "*.xml", false);
                     if (chooser.browseForFileToSave (true))
                     {
                         const File file (chooser.getResult());
                         if (XmlElement* xml = Standalone::instance()->getInstrument()->createXml())
                         {
                             xml->writeToFile (file, String::empty);
                             delete xml;
                         }
                     }
                }
                else if (FileInstrumentStats == menuItemID && topLevelMenuIndex == 0)
                {
                    StringArray samples;
                    InstrumentPtr inst (Standalone::instance()->getInstrument());
                    inst->getSamples (samples);
                    for (const String& s : samples) {
                        DBG (s);
                    }

                    DBG ("Instrument Statistics");
                    DBG ("---------------------");
                    DBG ("Name: " << inst->getName());
                    DBG ("Num Samples: " << samples.size() << "\n");
                }
                else if (FileTransposeUp == menuItemID && topLevelMenuIndex == 0)
                {
                    InstrumentPtr inst (Standalone::instance()->getInstrument());
                    const ValueTree& node (inst->node());
                    for (int i = 0; i < node.getNumChildren(); ++i) {
                        ValueTree key (node.getChild (i));
                        key.setProperty (Slugs::note, (int) key.getProperty (Slugs::note, -1) + 12, 0);
                        for (int j = 0; j < key.getNumChildren(); ++j) {
                            ValueTree layer (key.getChild (j));
                            layer.setProperty (Slugs::note, (int) key.getProperty (Slugs::note, -1), 0);
                        }
                    }
                }
                else if (FileTransposeDown == menuItemID && topLevelMenuIndex == 0)
                {

                }
                else if (FileDeviceSettings == menuItemID && topLevelMenuIndex == 0)
                {
                    Standalone::instance()->showAudioDevices();
                }
            }

        };

        class DevicesWindow : public DocumentWindow
        {
        public:
            DevicesWindow (AudioDeviceManager* d)
                : DocumentWindow ("Audio Devices", Colours::darkgrey, DocumentWindow::closeButton, false)
            {
                AudioDeviceSelectorComponent* ds = new AudioDeviceSelectorComponent (*d, 0, 0, 0, 2,
                        true, true, true, false);
                ds->setSize (300, 300);
                setResizable (true, false);
                setContentOwned (ds, true);
            }

            void closeButtonPressed() override
            {
                Standalone::instance()->hideAudioDevices();
            }
        };

        ScopedPointer<MainWindow> window;
    };

}

START_JUCE_APPLICATION (KSP1::Standalone)

#endif
#endif
