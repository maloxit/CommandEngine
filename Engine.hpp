#pragma once
#include "Wrapper.hpp"
#include <map>

/**
 * \brief Class for menadgment of wrapped functions.
 * \see Wrapper, Argument
 */
class Engine
{
private:
    /**
     * \brief Map for access to regestered commands by name
     */
    std::map<std::string, Wrapper*> registeredCommands;
public:
    /**
     * \brief Register wrapped functions as command with given name.
     * if command with the same name is already registered, or given vrapper is invalid, exception is thrown.
     * \param[in] wrapper - pointer to wrapped function
     * \param[in] name - name of new command
     */
    void registerCommand(Wrapper* wrapper, const std::string& name)
    {
        auto command = registeredCommands.find(name);
        if (command != registeredCommands.end())
            throw std::exception("Command with the same name is already registered.");
        if (wrapper == nullptr || !wrapper->isValid())
            throw std::exception("Can't register command with invalid wrapper.");
        registeredCommands[name] = wrapper;
    }
    /**
     * \brief Executes command by name with given list of arguments.
     * if no such command is regestered, out_of_range exception is thrown.
     * if argument list is not sutable for wrapped function, exception is thrown.
     * \param[in] name - name of command to execute
     * \param[in] argList - list of arguments in any order
     */
    int execute(const std::string& name, const std::vector<Argument>& argList)
    {
        return registeredCommands.at(name)->execute(argList);
    }

};