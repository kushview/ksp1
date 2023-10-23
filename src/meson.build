fs = import ('fs')

lv2_bundle = 'ksp1.lv2'

## Plugin Sources
plugin_sources = files ('''
    adsr.cpp
    articulator.cpp
    cache.cpp
    jobs.cpp
    layerdata.cpp
    lowpass.cpp
    plugin.cpp
    sounds.cpp
    synth.cpp
    voice.cpp
'''.split())

if host_machine.system() == 'darwin'
    plugin_sources += 'jucelib.mm'
else
    plugin_sources += 'jucelib.cpp'
endif

ui_type = 'X11UI'
if host_machine.system() == 'windows'
    ui_type = 'WindowsUI'
elif host_machine.system() == 'darwin'
    ui_type = 'CocoaUI'
endif

## UI Sources
ui_sources = files ('''
    ui.cpp
'''.split())

bundledir = lv2_install_dir / lv2_bundle

plugin = shared_module ('ksp1',
    plugin_sources,
    name_prefix : '',
    dependencies : [ lvtk_dep, juce_dep ],
    install : true,
    install_dir : bundledir,
    link_args : [ nodelete_cpp ],
    gnu_symbol_visibility : 'hidden'
)

ui = shared_module ('ksp1-ui',
    ui_sources,
    name_prefix : '',
    dependencies : [ lvtk_opengl_dep ],
    include_directories : [ '.' ],
    install : true,
    install_dir : bundledir,
    cpp_args : [ ],
    link_args : [ nodelete_cpp ],
    gnu_symbol_visibility : 'hidden'
)

manifest_ttl = configure_file (
    input : 'manifest.ttl.in',
    output : 'manifest.ttl',
    configuration : {
        'BINARY' : fs.name (plugin.full_path()),
        'UI_BINARY' : fs.name (ui.full_path()),
        'UI_TYPE' : ui_type
    },
    install : true,
    install_dir : bundledir
)

plugin_ttl = configure_file (
    input : 'ksp1.ttl.in',
    output : 'ksp1.ttl',
    copy : true,
    install : true,
    install_dir : bundledir
)

lv2_validate = find_program ('lv2_validate', required : false)
if lv2_validate.found()
    test ('lv2_validate', lv2_validate, args : [ manifest_ttl, plugin_ttl ])
endif

summary ('Bundle', bundledir, section : 'LV2')
