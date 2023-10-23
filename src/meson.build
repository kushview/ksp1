fs = import ('fs')

lv2_bundle = 'ksp1.lv2'

ksp1_sources = files ('''
    plugin.cpp
'''.split())

if host_machine.system() == 'darwin'
    ksp1_sources += 'jucelib.mm'
else
    ksp1_sources += 'jucelib.cpp'
endif

ksp1_ui_type = 'X11UI'
if host_machine.system() == 'windows'
    ksp1_ui_type = 'WindowsUI'
elif host_machine.system() == 'darwin'
    ksp1_ui_type = 'CocoaUI'
endif

lv2_install_dir = fs.expanduser (get_option ('lv2dir')).strip()
if lv2_install_dir == ''
    if target_machine.system() == 'darwin'
        lv2_install_dir = '/Library/Audio/Plug-Ins/LV2'
    elif target_machine.system() == 'haiku'
        lv2_install_dir = '/boot/common/add-ons/lv2'
    elif target_machine.system() == 'windows'
        lv2_install_dir = 'C:/Program Files/Common Files/LV2'
    else
        lv2_install_dir = get_option('prefix') / 'lib' / 'lv2'
    endif
endif

lv2_install_dir = lv2_install_dir / lv2_bundle

plugin = shared_module ('ksp1',
    ksp1_sources,
    name_prefix : '',
    dependencies : [ lvtk_dep, juce_dep ],
    install : true,
    install_dir : lv2_install_dir,
    gnu_symbol_visibility : 'hidden'
)

ui = shared_module ('ksp1-ui',
    'ui.cpp',
    name_prefix : '',
    dependencies : [ lvtk_opengl_dep ],
    include_directories : [ '.' ],
    install : true,
    install_dir : lv2_install_dir,
    cpp_args : [ ],
    gnu_symbol_visibility : 'hidden'
)

manifest_ttl = configure_file (
    input : 'manifest.ttl.in',
    output : 'manifest.ttl',
    configuration : {
        'BINARY' : fs.name (plugin.full_path()),
        'UI_BINARY' : fs.name (ui.full_path()),
        'UI_TYPE' : ksp1_ui_type
    },
    install : true,
    install_dir : lv2_install_dir
)

ksp1_ttl = configure_file (
    input : 'ksp1.ttl.in',
    output : 'ksp1.ttl',
    copy : true,
    install : true,
    install_dir : lv2_install_dir
)

lv2_validate = find_program ('lv2_validate', required : false)
if lv2_validate.found()
    test ('lv2_validate', lv2_validate, args : [ manifest_ttl, ksp1_ttl ])
endif

summary ('Bundle', lv2_install_dir, section : 'LV2')
