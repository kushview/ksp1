#!/usr/bin/env python

from subprocess import call, Popen, PIPE
import os, sys

sys.path.append (os.getcwd() + "/tools/waf")
import juce, ksp1

VERSION='0.1.0'

def options (opt):
    opt.load ("compiler_c compiler_cxx juce ksp1")

def silence_warnings (conf):
    '''TODO: resolve these'''
    conf.env.append_unique ('CFLAGS', ['-Wno-deprecated-register'])
    conf.env.append_unique ('CXXFLAGS', ['-Wno-deprecated-register'])
    conf.env.append_unique ('CFLAGS', ['-Wno-dynamic-class-memaccess'])
    conf.env.append_unique ('CXXFLAGS', ['-Wno-dynamic-class-memaccess'])
    conf.env.append_unique ('CFLAGS', ['-Wno-deprecated-declarations'])
    conf.env.append_unique ('CXXFLAGS', ['-Wno-deprecated-declarations'])

def configure_product (conf):
    # noop
    return

def configure_product_name (conf):
    return "KSP1"

def configure (conf):
    conf.check_ccache()
    conf.load ("compiler_c compiler_cxx ar juce ksp1")
    conf.check_cxx_version()
    silence_warnings (conf)
    conf.check_common()

    if juce.is_mac(): conf.check_mac()
    else: conf.check_linux()

    conf.env.DEBUG = conf.options.debug
    if conf.env.DEBUG:
        conf.define ('JUCE_DISABLE_AUDIOPROCESSOR_BEGIN_END_GESTURE_CHECKING', 1)

    conf.env.VERSION_STRING = VERSION

    configure_product (conf)

    conf.define ('VERSION_STRING', conf.env.VERSION_STRING)

    conf.check(lib='curl', mandatory=False)
    if juce.is_linux():
        conf.check(lib='pthread', mandatory=True)
        conf.check(lib='dl', mandatory=True)
        conf.check_cfg(package='freetype2', args='--cflags --libs', \
            mandatory=True)
        conf.check_cfg(package='x11', args='--cflags --libs', \
            mandatory=True)
        conf.check_cfg(package='xext', args='--cflags --libs', \
            mandatory=True)
        conf.check_cfg(package='alsa', args='--cflags --libs', \
            mandatory=True)

    print
    juce.display_header ("KSP1 Configuration")
    juce.display_msg (conf, "Installation PREFIX", conf.env.PREFIX)
    juce.display_msg (conf, "Installation DATADIR", conf.env.DATADIR)
    juce.display_msg (conf, "Debugging Symbols", conf.options.debug)
    print
    juce.display_header ("Compiler")
    juce.display_msg (conf, "CFLAGS", conf.env.CFLAGS)
    juce.display_msg (conf, "CXXFLAGS", conf.env.CXXFLAGS)
    juce.display_msg (conf, "LINKFLAGS", conf.env.LINKFLAGS)

def common_use_flags():
    return 'OPEN_GL ACCELERATE AUDIO_TOOLBOX AUDIO_UNIT CORE_AUDIO CORE_AUDIO_KIT COCOA CORE_MIDI IO_KIT QUARTZ_CORE'.split()

def build_mac (bld):
    libEnv = bld.env.derive()

    bld.shlib (
        source      = bld.path.ant_glob ("src/**/*.cpp") + \
                      bld.path.ant_glob ("jucer/Source/**/*.cpp") + \
                      ksp1.get_juce_library_code ('jucer/JuceLibraryCode', '.mm'),
        includes    = [ 'jucer/JuceLibraryCode', 'libs/kv/modules', \
                        'libs/juce/modules', 'src' ],
        target      = 'lib/ksp1',
        name        = 'KSP1',
        env         = libEnv,
        use         = common_use_flags()
    )

    bld.add_group()
    appEnv = bld.env.derive()
    app = bld.program (
        source      = [ 'jucer/JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp',
                        'jucer/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp' ],
        includes    = [ 'jucer/JuceLibraryCode', 'libs/kv/modules', \
                        'libs/juce/modules', 'src' ],
        target      = 'Applications/KSP1',
        name        = 'KSP1_APP',
        env         = appEnv,
        use         = common_use_flags(),
        mac_app     = True,
        mac_plist   = 'tools/macdeploy/Info-Standalone.plist'
        # mac_files   = [ 'tools/macdeploy/Icon.icns' ]
    )
    
    app.use.append ('KSP1')

def build (bld):
    build_mac (bld)

def macdeploy (ctx):
    call (["tools/macdeploy/appbundle.py",
           "-verbose", "3",
           "-dmg", "ksp1-osx-%s" % VERSION,
           "-volname", "KSP1",
           "-fancy", "tools/macdeploy/fancy.plist",
           "build/Applications/KSP1.app"])

def macrelease (ctx):
    call (["bash", "tools/macdeploy/sync-jucer.sh"])
    call (["bash", "tools/macdeploy/clean.sh"])
    call (["python", "waf", "distclean", "configure", "build", "macdeploy"])
