#include "DisplayView.h"

namespace KSP1 {

DisplayView::DisplayView() {
    addAndMakeVisible (display);
}

DisplayView::~DisplayView() {
}

void DisplayView::paint (Graphics& g) {
}

void DisplayView::resized() {
    display.setBounds (getLocalBounds());
}

} // namespace KSP1
