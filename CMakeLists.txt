# KSP1 CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(KSP1 VERSION 0.0.1)

add_subdirectory(deps/JUCE)
add_subdirectory(deps/clap-juce-extensions EXCLUDE_FROM_ALL)

juce_add_plugin(KSP1
    COMPANY_NAME Kushview                       # Specify the name of the plugin's author
    IS_SYNTH TRUE                               # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE                       # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE            # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD FALSE               # Should the plugin be installed to a default location after building?
    LV2URI "https://kushview.net/plugins/ksp1"
    PLUGIN_MANUFACTURER_CODE KshV               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Ksp1                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 LV2 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "KSP1")                        # The name of the final executable, which can differ from the target name
clap_juce_extensions_plugin(TARGET KSP1
    CLAP_ID "net.kushview.KSP1"
    CLAP_FEATURES sampler drum-machine)

add_subdirectory(src)

target_compile_definitions(KSP1
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(KSP1
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
