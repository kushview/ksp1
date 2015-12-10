#!/usr/bin/env python

from subprocess import call, Popen, PIPE
import os, platform, sys

import sys
sys.path.insert (0, 'tools/waf')
import juce, products, apple, audiounit

common_tools = 'juce'

def options(opt):
    opt.load ('compiler_c compiler_cxx')
    opt.load (common_tools)
    if juce.is_mac():
        opt.load ('apple audiounit')

    opt.add_option ('--tests', default=False, action='store_true', dest='tests', \
        help='Compile Unit Tests [ Default: False ]')
    opt.add_option ('--enable-egl', default=False, action='store_true', dest='enable_egl', \
        help='Use EGL windowing [ Default: False ]')
    opt.add_option ('--without-apps', default=False, action='store_true', dest='without_apps', \
        help='Do not compile Applications/Firmware')
    opt.add_option ('--without-plugins', default=False, action='store_true', dest='without_plugins', \
        help='Do not compile AU/VST/LV2 plugins')
    opt.add_option ('--installers', default=False, action='store_true', dest='installers', \
        help='Create installer(s) after the build is complete [ Default: False ]')

def compiler_pre_config (conf):
    if not juce.is_win32():
        '''Use clang by default on non-windows'''
        if not 'CC' in os.environ or not 'CXX' in os.environ:
            conf.env.CC  = 'clang'
            conf.env.CXX = 'clang++'

def configure_compiler (conf):
    conf.check_cxx11()
    if juce.is_linux() and 'clang' in conf.env.CXX[0]:
        conf.env.append_unique ('CFLAGS', ['-Wno-deprecated-register'])
        conf.env.append_unique ('CXXFLAGS', ['-Wno-deprecated-register'])

def platform_pre_config (conf):
    conf.define ('JUCE_USE_EGL', conf.options.enable_egl)
    conf.env.USE_EGL = conf.options.enable_egl
    conf.env.USE_X11 = not conf.options.enable_egl
    conf.define ('JUCE_USE_X11', conf.env.USE_X11)
    conf.define ('JUCE_USE_XEVENTS', conf.env.USE_X11)
    conf.write_config_header ('src/platform/config.h')

    if juce.is_mac():
        os.environ['PKG_CONFIG_PATH'] = '/opt/bketech/lib/pkgconfig'
        conf.env.ARCH_COCOA  = ['i386', 'x86_64']
        conf.env.append_unique ('LINKFLAGS', '-dead_strip')
        if not conf.options.apple_version_min:
            conf.options.apple_version_min = '10.7'
        if not conf.options.apple_sdk:
            conf.options.apple_sdk = '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk'
        if not conf.options.core_audio_root:
            conf.options.core_audio_root = '/opt/kushview/Developer/Extras/CoreAudio'

    elif juce.is_linux(): pass
    elif juce.is_win32(): pass

def configure (conf):
    compiler_pre_config (conf)
    platform_pre_config (conf)
    conf.load ('compiler_c compiler_cxx ' + common_tools)

    if juce.is_mac():
        conf.load ('apple audiounit')

    configure_compiler (conf)

    conf.env.KSP1_BUILD_APPS          = not conf.options.without_apps
    conf.env.KSP1_BUILD_PLUGINS       = not conf.options.without_plugins and not juce.is_linux()
    conf.env.KSP1_BUILD_TESTS         = conf.options.tests
    conf.env.KSP1_BUILD_INSTALLERS    = conf.options.installers

    conf.env.VERSION_STRING         = '0.0.1'
    conf.define ('KSP1_VERSION_STRING', conf.env.VERSION_STRING)
    #conf.define ('KSP1_STANDALONE', 1)

    conf.env.DATADIR = os.path.join (conf.env.PREFIX, 'share')

    # Check for packages
    def add_pkg_defs (pkgdefs):
        for d in pkgdefs:
            conf.env[d] = conf.is_defined (d)

    conf.check_cfg (package='lilv-0', uselib_store='LILV', args='--cflags --libs', mandatory=True)
    conf.check_cfg (package='suil-0', uselib_store='SUIL', args='--cflags --libs', mandatory=False)
    conf.check_cfg (package='jack', uselib_store='JACK', args='--cflags --libs', mandatory=False)
    add_pkg_defs (['HAVE_LILV', 'HAVE_SUIL', 'HAVE_JACK'])

    if juce.is_linux():
        conf.check(features='c cxx cxxprogram', lib=['dl'], cflags=['-Wall'], uselib_store='DL')
        conf.check(features='c cxx cxxprogram', lib=['pthread'], cflags=['-Wall'], uselib_store='PTHREAD')
        conf.check_cfg (package="alsa", uselib_store="ALSA", args='--cflags --libs', mandatory=True)
        conf.check_cfg (package="freetype2", uselib_store="FREETYPE2", args='--cflags --libs', mandatory=True)
        if conf.env.USE_EGL:
            conf.check_cfg (package='x11-xcb', uselib_store='XCB', args='--cflags --libs', mandatory=False)
            conf.check_cfg (package="glesv2", uselib_store="GLESV2", args='--cflags --libs', mandatory=True)
            conf.check_cfg (package="egl", uselib_store="EGL", args='--cflags --libs', mandatory=True)
        else:
            conf.check_cfg (package="gl", uselib_store="GL", args='--cflags --libs', mandatory=True)
            conf.check_cfg (package="x11", uselib_store="X11", args='--cflags --libs', mandatory=True)
            conf.check_cfg (package="xext", uselib_store="XEXT", args='--cflags --libs', mandatory=True)
        add_pkg_defs (['HAVE_ALSA', 'HAVE_X11', 'HAVE_XEXT', 'HAVE_FREETYPE2', 'HAVE_GL', \
                       'HAVE_EGL', 'HAVE_XCB', 'HAVE_GLES2'])

    pat = conf.env['cshlib_PATTERN']
    if not pat:
        pat = conf.env['cxxshlib_PATTERN']
    if pat.startswith('lib'):
        pat = pat[3:]
    conf.env['plugin_PATTERN'] = pat
    conf.env['plugin_EXT'] = pat[pat.rfind('.'):]

    print
    juce.display_header ("BTV Build Summary")
    juce.display_msg (conf, "Installation Prefix", conf.env.PREFIX)
    juce.display_msg (conf, "Shared Data Path", conf.env.DATADIR)
    juce.display_msg (conf, "Compile Standalone App", conf.env.KSP1_BUILD_APPS)
    juce.display_msg (conf, "Compile Plugins", conf.env.KSP1_BUILD_PLUGINS)
    juce.display_msg (conf, "Jack Audio Support", len(conf.env.LIB_JACK) > 0)
    juce.display_msg (conf, "LV2 Plugin Support", conf.env.HAVE_LILV)
    juce.display_msg (conf, "LV2 Plugin GUI Support", conf.env.HAVE_SUIL)
    juce.display_msg (conf, "Firmware Version", conf.env.VERSION_STRING)
    juce.display_msg (conf, "Build Installers", conf.env.KSP1_BUILD_INSTALLERS)

    if juce.is_mac():
        print
        juce.display_header ("Apple Configuration Summary")
        juce.display_msg (conf, "Apple Framework Dir", conf.env.FRAMEWORKDIR)
        juce.display_msg (conf, "Apple Deployment Target", conf.env.APPLE_VERSION_MIN)
        juce.display_msg (conf, "Apple SDK Path", conf.env.APPLE_SDK)
        juce.display_msg (conf, "Apple Architecture(s)", conf.env.ARCH_COCOA)
        juce.display_msg (conf, "Install in User Directories", conf.env.APPLE_USERDIRS)

    print
    juce.display_header ("Global Compiler Flags")
    juce.display_msg (conf, "Compile flags (c)", conf.env.CFLAGS)
    juce.display_msg (conf, "Compile flags (c++)", conf.env.CXXFLAGS)
    juce.display_msg (conf, "Linker flags", conf.env.LINKFLAGS)

def build_prepare_mac (bld):
    return

def build_lv2_plugin (bld):
    return

def build_sqlite3 (bld):
    bld.stlib (
        name = 'libsqlite3',
        target = 'lib/sqlite3_ksp1',
        source = ['libs/sqlite3/sqlite3.c'],
        install_path = None,
        cflags = ['-fPIC']
    )

    bld.program (
        name = 'bin/sqlite3',
        target = 'bin/sqlite3',
        source = bld.path.ant_glob ('libs/sqlite3/*.c'),
        use = ['libsqlite3', 'DL', 'PTHREAD'],
        install_path = None
    )

def build (bld):
    build_sqlite3 (bld)

    plugin_source = '''
        src/engine/Jobs.cpp
        src/engine/ADSR.cpp
        src/engine/LayerData.cpp
        src/engine/LowPassFilter.cpp
        src/engine/SampleCache.cpp
        src/engine/SamplerSounds.cpp
        src/engine/SamplerSynth.cpp
        src/engine/SamplerVoice.cpp
        src/engine/LV2Plugin.cpp
        src/URIs.cpp
    '''.split()

    juce_module_path = 'libs/libjuce/src/modules'
    plugin_modules = ['juce_core', 'juce_audio_basics', 'juce_audio_formats']
    for module in plugin_modules:
        plugin_source.append (juce_module_path + '/' + module + '/' + module + '.cpp')

    plugin_dir = bld.env.PREFIX + '/lib/lv2/ksp1.lv2'

    plugin_environ = bld.env.derive()
    plugin_environ.cshlib_PATTERN = plugin_environ.cxxshlib_PATTERN = \
        bld.env.plugin_PATTERN

    plugin = bld.shlib (
        source = plugin_source,
        includes = ['jucer/JuceLibraryCode', 'jucer/Source', 'src', 'libs/lvtk'],
        target = 'plugins/ksp1.lv2/plugin',
        use = ['libsqlite3', 'PTHREAD', 'DL'],
        cxxflags = ['-DKSP1_BUILD_LV2=1'],
        install_path = plugin_dir,
        env = plugin_environ
    )

    bld.add_group()

    p = juce.IntrojucerProject (bld, 'standalone/KSP1 Standalone.jucer');
    ui = bld.shlib (
        source = bld.path.ant_glob ('src/*.cpp') +
                 bld.path.ant_glob ('src/editor/*.cpp') +
                 p.getLibraryCode(),
        includes = ['standalone/JuceLibraryCode', 'src', 'libs/lvtk'],
        target = 'plugins/ksp1.lv2/ui',
        use = ['libsqlite3', 'LILV', 'SUIL', 'X11', 'XEXT',
               'ALSA', 'FREETYPE2', 'GL', 'EGL', 'GLESV2', 'XCB'],
        cxxflags = ['-DKSP1_STANDALONE=1'],
        linkflags = ['-lpthread'],
        install_path = plugin_dir,
        env = plugin_environ
    )

    ttl = bld (
        features = 'subst',
        source = 'jucer/LV2/manifest.ttl',
        target = 'plugins/ksp1.lv2/manifest.ttl',
        install_path = plugin_dir
    )

    ttl = bld (
        features = 'subst',
        source = 'jucer/LV2/KSP1.ttl',
        target = 'plugins/ksp1.lv2/KSP1.ttl',
        install_path = plugin_dir
    )
    '''
    libksp1 = bld.objects (
        source = p.getLibraryCode() + bld.path.ant_glob ('src/**/*.cpp'),
        includes = ['jucer/JuceLibraryCode', 'jucer/Source', 'src'],
        name = 'libksp1',
        target = 'libs/ksp1',
        use = ['LILV', 'SUIL', 'X11', 'XEXT', 'ALSA', 'FREETYPE2', 'GL', \
               'EGL', 'GLESV2', 'XCB'],
        cxxflags = ['-fPIC'],
        linkflags = ['-lpthread']
    )
    '''

    bld.add_group()

    bt = bld.program (
        source = bld.path.ant_glob ('standalone/Source/**/*.cpp') +
                 p.getLibraryCode(),
        includes = ['standalone/JuceLibraryCode', 'standalone/Source', 'src'],
        target = 'bin/ksp1',
        use = ['libsqlite3', 'PTHREAD', 'LILV', 'SUIL', 'X11', 'XEXT',
               'ALSA', 'FREETYPE2', 'GL', 'EGL', 'GLESV2', 'XCB'],
        cxxflags = ['-DKSP1_STANDALONE=1'],
        env = bld.env.derive()
    )

    # if bld.env.KSP1_BUILD_TESTS:
    #     tests = bld.program (
    #         source = bld.path.ant_glob ('tests/**/*.cpp'),
    #         includes = ['src', 'project/JuceLibraryCode'],
    #         name = 'runTests',
    #         target = 'runTests',
    #         use = ['BTV', 'LILV', 'SUIL'],
    #         linkflags = obj.linkflags,
    #     )

    #bld.add_post_fun (build_installer)

def build_macdeploy (bld):
    if bld.env.KSP1_BUILD_APPS:
        call (['bash', 'tools/macdeploy'])
    if bld.env.KSP1_BUILD_PLUGINS:
        call (['bash', 'tools/macdeploy-plugins'])

def build_installer (bld):
    '''A post-build function used to create installers'''

    if not bld.env.KSP1_BUILD_INSTALLERS:
        return

    cwd = os.getcwd()
    if juce.is_mac():
        os.chdir ('tools/installer/packagemaker')
        call (['bash', 'build-installer'])
        os.chdir (cwd + '/tools/installer/qt-ifw')
        call (['bash', 'build-osx'])
    else:
        pass

    os.chdir (cwd)

from waflib import TaskGen
@TaskGen.extension ('.mm')
def mm_hook (self, node):
    return self.create_compiled_task ('cxx', node)

from waflib import TaskGen
@TaskGen.extension ('.r')
def res_hook (self, node):
    pass
