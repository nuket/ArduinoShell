#include "as_commandandparams.h"

CommandAndParams::CommandAndParams(String rawCommand) :
    commandUsable(false),
    paramCount(0)
{
    // Trim whitespace in place.
    rawCommand.trim();

    // Check that the command was more than just whitespace.
    if (rawCommand.length() == 0) return;

    // At least a command is available, maybe parameters are not.
    commandUsable = true;

    int spaceA = rawCommand.indexOf(' ');
    int spaceB = -1;

    // .indexOf returns -1 if character is not found.
    if (spaceA == -1)
    {
        command = rawCommand;
        return; // Return early, no parameters to be parsed.
    }
    else
    {
        command = rawCommand.substring(0, spaceA);
    }

    for (unsigned int i = 0; i < MAX_PARAMS; i++)
    {
        spaceA++;
        spaceB = rawCommand.indexOf(' ', spaceA);

//        Serial.println(spaceA);
//        Serial.println(spaceB);

        if (spaceB == -1)
        {
            params[i] = rawCommand.substring(spaceA);

            // EOL reached.
            paramCount = i + 1;
            break;
        }
        else
        {
            params[i] = rawCommand.substring(spaceA, spaceB);
            spaceA = spaceB;
        }
    }
}

void CommandAndParams::print()
{
    Serial.print("command:  >>");
    Serial.print(command);
    Serial.println("<<");
    
    for (int i = 0; i < paramCount; i++)
    {
        Serial.print("param[");
        Serial.print(i);
        Serial.print("]: >>");
        Serial.print(params[i]);
        Serial.println("<<");
    }
}

#if ENABLE_UNIT_TESTS
static void test_command_and_params_class()
{
    Serial.println("CommandAndParams unit test starting.");
    
    CommandAndParams testA("test 1 2 3");
    testA.print();
    assert(String("test").equals(testA.command));
    assert(testA.commandUsable);
    assert(String("1").equals(testA.params[0]));
    assert(String("2").equals(testA.params[1]));
    assert(String("3").equals(testA.params[2]));
    assert(testA.paramCount == 3);

    CommandAndParams testB("wb 0x10 0x41"); // write the byte 'a' to position 0x10.
    testB.print();
    assert(String("wb").equals(testB.command));
    assert(testB.commandUsable);
    assert(String("0x10").equals(testB.params[0]));
    assert(String("0x41").equals(testB.params[1]));
    assert(testB.paramCount == 2);
    
    Serial.println("CommandAndParams unit test ending OK.");
}
#endif

