#pragma once

#include <string>
class Action
{
    virtual void execute();
    virtual void undo();
    virtual void setup();
    virtual std::string statusString();
};

class ActionFactory
{
    ActionFactory() = delete;

    static bool RegisterAction (const string name, .

    )
}