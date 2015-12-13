
#include "AppController.h"
#include "Commands.h"
#include "World.h"

namespace KSP1 {

AppController::AppController (World& w) : world(w)
{

}

AppController::~AppController() { }

ApplicationCommandTarget* AppController::getNextCommandTarget()  { return nullptr; }

void AppController::getAllCommands (Array<CommandID>& commands)
{
    commands.add (Commands::quit);
}

void AppController::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
        case Commands::quit:
            result.setInfo ("Quit", "Quit the application", "Application", 0);
            result.addDefaultKeypress ('q', ModifierKeys::commandModifier);
            break;
    }
}

bool AppController::perform (const InvocationInfo&)
{
    return false;
}

}
