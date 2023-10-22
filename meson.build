project ('ksp1', ['c', 'cpp'], 
    version : '1.0.0',
    default_options : [ 
        'cpp_std=c++17', 
        'default_library=static',
        'b_ndebug=if-release',
        'buildtype=release',
        'c_std=c99',
    ])

# Add objective-c if on mac
if host_machine.system() == 'darwin'
  add_languages(['objc', 'objcpp'], native : false, required : true)
  objcc = meson.get_compiler ('objc')
  objcpp = meson.get_compiler ('objcpp')
endif

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

lvtk_opengl_dep = dependency ('lvtk-opengl-3', 
    fallback : [ 'lvtk', 'lvtk_opengl_dep' ],
    version : '>= 3.0.0',
    default_options : lvtk_opts,
    required : true
)

juce_proj = subproject ('juce')
juce_dep = juce_proj.get_variable ('juce_dep')
