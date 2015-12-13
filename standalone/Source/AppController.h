#ifndef KSP1_APP_CONTROLLER_H
#define KSP1_APP_CONTROLLER_H

#include "ControllerBase.h"

namespace KSP1 {

class World;

class AppController :  public ControllerBase,
                       public ApplicationCommandTarget
{
public:
    AppController (World&);
    ~AppController();

    ApplicationCommandManager& getCommandManager() { return commands; }
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands (Array<CommandID>& commands) override;
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override;
    bool perform (const InvocationInfo& info) override;

private:
    World& world;
    ApplicationCommandManager commands;
};

}

#endif // KSP1_APP_CONTROLLER_H

