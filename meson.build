project ('ksp1', ['c', 'cpp'], 
    version : '1.0.0',
    default_options : [ 
        'cpp_std=c++17', 
        'default_library=static',
        'b_ndebug=if-release',
        'buildtype=release',
        'c_std=c99',
    ])

fs = import('fs')

cpp = meson.get_compiler('cpp')

# Add objective-c if on mac
if host_machine.system() == 'darwin'
  add_languages(['objc', 'objcpp'], native : false, required : true)
  objcc = meson.get_compiler ('objc')
  objcpp = meson.get_compiler ('objcpp')
endif

nodelete_cpp = cpp.get_supported_link_arguments(['-Wl,-z,nodelete'])

if not meson.is_subproject()
    add_global_arguments (['-DLVTK_STATIC=1', '-DPUGL_STATIC=1'], 
        language : [ 'c', 'cpp', 'objc', 'objcpp' ])
endif

lvtk_opts = [
    'doc=disabled',
    'test=disabled',
    'demo=disabled',
    'default_library=static'
]

lvtk_dep = dependency ('lvtk-3', 
    fallback : [ 'lvtk', 'lvtk_dep' ],
    version : '>= 3.0.0',
    default_options : lvtk_opts,
    required : true
)

lvtk_cairo_dep = dependency ('lvtk-opengl-3', 
    fallback : [ 'lvtk', 'lvtk_cairo_dep' ],
    version : '>= 3.0.0',
    default_options : lvtk_opts,
    required : true
)

juce_proj = subproject ('juce')
juce_dep = juce_proj.get_variable ('juce_dep')

lv2_install_dir = get_option ('lv2dir').strip()
if lv2_install_dir != ''
    lv2_install_dir = fs.expanduser (lv2_install_dir)
endif

if lv2_install_dir == ''
    if target_machine.system() == 'darwin'
        lv2_install_dir = fs.expanduser ('~/Library/Audio/Plug-Ins/LV2')
    elif target_machine.system() == 'haiku'
        lv2_install_dir = '/boot/common/add-ons/lv2'
    elif target_machine.system() == 'windows'
        lv2_install_dir = fs.expanduser ('~/AppData/Roaming/LV2')
    else
        lv2_install_dir = fs.expanduser('~/.lv2')
    endif
endif

subdir ('src')
# subdir ('kits/sn-r8-04')
# subdir ('kits/sn-r8-11')
