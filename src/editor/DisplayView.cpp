#include "DisplayView.h"

namespace KSP1 {
namespace Gui {

DisplayView::DisplayView()
{
    addAndMakeVisible (display);
}

DisplayView::~DisplayView()
{

}

void DisplayView::paint (Graphics& g)
{

}

void DisplayView::resized()
{
    display.setBounds (getLocalBounds());
}

}}
