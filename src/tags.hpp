#pragma once

#include "juce.hpp"

namespace ksp1 {
namespace tags {

static const juce::Identifier ID      = "ID";
static const juce::Identifier index   = "index";
static const juce::Identifier name    = "name";
static const juce::Identifier note    = "note";
static const juce::Identifier volume  = "volume";
static const juce::Identifier panning = "panning";
static const juce::Identifier pitch   = "pitch";
static const juce::Identifier file    = "file";

static const juce::Identifier start  = "start";
static const juce::Identifier offset = "offset";
static const juce::Identifier length = "length";

static const juce::Identifier velocityLower    = "velocityLower";
static const juce::Identifier velocityUpper    = "velocityUpper";

static const juce::Identifier sounds = "sounds";
static const juce::Identifier layers = "layers";

static const juce::Identifier voiceGroup = "voiceGroup";
static const juce::Identifier triggerMode = "triggerMode";

} // namespace tags
} // namespace ksp1
