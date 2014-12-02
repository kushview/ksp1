/*

    IMPORTANT! This file is auto-generated each time you save your
    project - if you alter its contents, your changes may be overwritten!

    There's a section below where you can add your own custom code safely, and the
    Introjucer will preserve the contents of that block, but the best way to change
    any of these definitions is by using the Introjucer's project settings.

    Any commented-out settings will assume their default values.

*/

#ifndef __JUCE_APPCONFIG_DK35TX__
#define __JUCE_APPCONFIG_DK35TX__

//==============================================================================
// [BEGIN_USER_CODE_SECTION]

#define JUCE_USE_X11 1
#define JUCE_USE_XEVENTS 1
#define JUCE_USE_EGL 0
#define LVTK_JUCE_NAMESPACE juce

#if KSP1_STANDALONE
 #define JUCE_ALSA 1
#endif

// [END_USER_CODE_SECTION]

//==============================================================================
#define JUCE_MODULE_AVAILABLE_element_base        1
#define JUCE_MODULE_AVAILABLE_element_gui         1
#define JUCE_MODULE_AVAILABLE_element_models      1
#define JUCE_MODULE_AVAILABLE_lvtk_core           1

//==============================================================================
// element_base flags:

#ifndef    ELEMENT_OSC
 //#define ELEMENT_OSC
#endif

//==============================================================================
// lvtk_core flags:

#ifndef    LVTK_USE_CXX11
 #define   LVTK_USE_CXX11 1
#endif


#endif  // __JUCE_APPCONFIG_DK35TX__
