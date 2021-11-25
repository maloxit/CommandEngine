// CommandEngine.cpp : Defines the entry point for the application.
//

#include "CommandEngine.hpp"

using namespace std;

class Sample
{
public:
    int someFunc1(int param1, int param2)
    {
        return param1 - param2;
    }
};

static int failedCount = 0;

static void logTestFail(const char* text)
{
    failedCount++;
    cout << text << endl;
}

static void logTestFail(const string& text)
{
    failedCount++;
    cout << text << endl;
}

int main()
{
    cout << "[BEGIN]" << endl;
    try
    {
        Engine engine;
        Sample obj;

        //Test 1: Correct wrapper initialization
        Wrapper wrapper1(&obj, &Sample::someFunc1, { {"param 1", 0}, {"param 2", 0} });
        if (!wrapper1.isValid())
            logTestFail("[FAILED]Test 1: Correct wrapper initialization.");

        //Test 2: Incorrect wrapper initialization
        Wrapper wrapper2(&obj, &Sample::someFunc1, { {"param 1", 0} });
        if (wrapper2.isValid())
            logTestFail("[FAILED]Test 2: Incorrect wrapper initialization.");

        //Test 3: Correct wrapper registration
        try
        {
            engine.registerCommand(&wrapper1, "some command1");
        }
        catch (std::exception& ex)
        {
            logTestFail("[FAILED]Test 3: Correct wrapper registration.");
        }

        //Test 4: Incorrect wrapper registration
        try
        {
            engine.registerCommand(&wrapper2, "some command2");
            logTestFail("[FAILED]Test 4: Incorrect wrapper registration.");
        }
        catch (std::exception& ex)
        {
            if (string(ex.what()) != "Can't register command with invalid wrapper.")
                logTestFail("[FAILED]Test 4: Incorrect wrapper registration.");
        }

        //Test 5: Duplicate correct wrapper registration
        try
        {
            engine.registerCommand(&wrapper1, "some command1");
            logTestFail("[FAILED]Test 5: Duplicate correct wrapper registration.");
        }
        catch (std::exception& ex)
        {
            if (string(ex.what()) != "Command with the same name is already registered.")
                logTestFail("[FAILED]Test 5: Duplicate correct wrapper registration.");
        }

        //Test 6: Correct command call(ordered arguments)
        try
        {
            if (2 != engine.execute("some command1", { {"param 1", 3}, {"param 2", 1} }))
                logTestFail("[FAILED]Test 6: Correct command call(ordered arguments).");
        }
        catch (std::exception& ex)
        {
            logTestFail("[FAILED]Test 6: Correct command call(ordered arguments).");
        }

        //Test 7: Correct command call(unordered arguments)
        try
        {
            if (-2 != engine.execute("some command1", { {"param 2", 3}, {"param 1", 1} }))
                logTestFail("[FAILED]Test 7: Correct command call(unordered arguments).");
        }
        catch (std::exception& ex)
        {
            logTestFail("[FAILED]Test 7: Correct command call(unordered arguments).");
        }

        //Test 8: Unregistered command call
        try
        {
            engine.execute("some command3", { {"param 2", 3}, {"param 1", 1} });
            logTestFail("[FAILED]Test 8: Unregistered command call.");
        }
        catch (std::out_of_range& ex)
        {
        }
        catch (std::exception& ex)
        {
            logTestFail("[FAILED]Test 8: Unregistered command call.");
        }

        //Test 9: Incorrect arguments count command call
        try
        {
            engine.execute("some command1", { {"param 1", 3} });
            logTestFail("[FAILED]Test 9: Incorrect arguments count command call.");
        }
        catch (std::exception& ex)
        {
            if (string(ex.what()) != "Command call contains incorrect arguments count.")
                logTestFail("[FAILED]Test 9: Incorrect arguments count command call.");
        }

        //Test 10: Unknown arguments command call
        try
        {
            engine.execute("some command1", { {"param 1", 3}, {"param 3", 1} });
            logTestFail("[FAILED]Test 10: Unknown arguments command call.");
        }
        catch (std::exception& ex)
        {
            if (string(ex.what()) != "Unknown command argument.")
                logTestFail("[FAILED]Test 10: Unknown arguments command call.");
        }

        //Test 11: Duplicate arguments command call
        try
        {
            engine.execute("some command1", { {"param 1", 3}, {"param 1", 1} });
            logTestFail("[FAILED]Test 11: Duplicate arguments command call.");
        }
        catch (std::exception& ex)
        {
            if (string(ex.what()) != "Command call contains multiple argument with same name.")
                logTestFail("[FAILED]Test 11: Duplicate arguments command call.");
        }
    }
    catch (std::exception& ex)
    {
        logTestFail("[FAILED]Unhandled exception: " + string(ex.what()) + ".");
    }
    cout << "[FINISHED] Tast fails count: " << failedCount << endl;
    return 0;
}
