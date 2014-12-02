#!/usr/bin/env python
# -*- coding: utf-8 -*-

import audiounit, juce

def build_btsp1 (bld):
    node = bld.path.find_resource ('plugins/BTSP1/BTSP1.jucer')
    p = juce.IntrojucerProject (bld, node.abspath())
    client_path = p.getModulePath ('juce_audio_plugin_client')

    au_resource = bld.path.find_resource (client_path + '/AU/juce_AU_Resources.r')
    au_code = p.getLibraryCode() + bld.path.ant_glob ('src/**/*.cpp') + \
              bld.path.ant_glob ('plugins/BTSP1/Source/**/*.cpp')

    plug = bld.shlib (
        features = 'audiounit',
        au_resource = au_resource,
        mac_package = True,
        mac_extension = '.component',
        mac_plist = 'plugins/BTSP1/AudioUnit/Info.plist',
        name     = 'BTSP1',
        target   = 'Plug-Ins/Components/BTSP1',
        source   = au_code,
        includes = ['src', 'plugins/BTSP1/JuceLibraryCode', 'plugins/BTSP1/Source'],
        use      = ['LILV', 'SUIL', 'COCOA', 'DISC_RECORDING', 'IO_KIT', \
                    'AUDIO_TOOLBOX', 'ACCELERATE', 'CORE_AUDIO', \
                    'CORE_MIDI', 'OPEN_GL', 'QUARTZ_CORE', 'WEB_KIT', \
                    'AUDIO_UNIT', 'CORE_AUDIO_KIT', 'CARBON'],
        cxxflags = ['-DBTSP1_PLUGIN=1']
    )

    return plug
