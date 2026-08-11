#include "keyboard_prompts.h"
#include "test_registry.h"

TEST_CASE(GenericPcActionResourcesAreDetectedWithoutMatchingValidDeviceIcons)
{
    CHECK_TRUE(shh::IsGenericPcActionResource("shv_buttonactionpc"));
    CHECK_TRUE(shh::IsGenericPcActionResource("PC_DODGEFLASH"));
    CHECK_TRUE(!shh::IsGenericPcActionResource("shv_buttonpcmouseleft_32"));
    CHECK_TRUE(!shh::IsGenericPcActionResource("shv_buttonpccontrol2"));
    CHECK_EQ(shh::ResolveGenericPcActionCommand(34, "shv_buttonactionpc"), 5u);
    CHECK_EQ(shh::ResolveGenericPcActionCommand(46, "pc_dodgeflash"), 5u);
    CHECK_EQ(shh::ResolveGenericPcActionCommand(6, "shv_buttonactionpc"), 6u);
    CHECK_EQ(shh::ResolveGenericPcActionCommand(8, "shv_buttonactionpc"), 8u);
    CHECK_EQ(shh::ResolveGenericPcActionCommand(34, "shv_buttonpccontrol1"), 34u);
}

TEST_CASE(ParseKeyboardPromptBindingsReadsProfileZeroKeyboardAndMouse)
{
    const auto bindings = shh::ParseKeyboardPromptBindings(
        "setbind 0 COMMAND_KICK MOUSE 0 BUTTON_0 -1 1 1\n"
        "setbind 0 COMMAND_JUMP KEYBOARD 0 KEY_SPACE -1 1 1\n"
        "setbind 1 COMMAND_KICK CONTROLLER 0 BUTTON_0 -1 1 1\n");

    CHECK_EQ(bindings.at("COMMAND_KICK"), std::string("LMB"));
    CHECK_EQ(bindings.at("COMMAND_JUMP"), std::string("Space"));
}

TEST_CASE(ParseKeyboardPromptBindingsHonorsSetbindAndUsesAddbindAsFallback)
{
    const auto bindings = shh::ParseKeyboardPromptBindings(
        "addbind 0 COMMAND_UI_START KEYBOARD 0 KEY_ESCAPE -1 1 1\n"
        "setbind 0 COMMAND_UI_START KEYBOARD 0 KEY_RETURN -1 1 1\n"
        "addbind 0 COMMAND_UI_START KEYBOARD 0 KEY_NUMPADENTER -1 1 1\n");

    CHECK_EQ(bindings.at("COMMAND_UI_START"), std::string("Enter"));
}

TEST_CASE(ReplaceKeyboardPromptTokensUsesAliasesAndKeepsMarkup)
{
    const auto bindings = shh::ParseKeyboardPromptBindings(
        "setbind 0 COMMAND_KICK MOUSE 0 BUTTON_0 -1 1 1\n"
        "setbind 0 COMMAND_PUNCH MOUSE 0 BUTTON_1 -1 1 1\n");

    const std::string result = shh::ReplaceKeyboardPromptTokens(
        "<COLOR,1,2,3>Press <ACTION> or <HEAVY_ATTACK>.<CLEARCOLOR>", bindings);

    CHECK_EQ(result,
             std::string("<COLOR,1,2,3>Press [LMB] or [RMB].<CLEARCOLOR>"));
}

TEST_CASE(ReplaceKeyboardPromptTokensMapsDefenseToCurrentDodgeBinding)
{
    const auto bindings = shh::ParseKeyboardPromptBindings(
        "setbind 0 COMMAND_JUMP KEYBOARD 0 KEY_SPACE -1 1 1\n");

    CHECK_EQ(shh::ReplaceKeyboardPromptTokens("Press <DEFENSE> to dodge.", bindings),
             std::string("Press [Space] to dodge."));
}

TEST_CASE(ReplaceKeyboardPromptTokensBuildsMovementLabels)
{
    const auto bindings = shh::ParseKeyboardPromptBindings(
        "setbind 0 COMMAND_FORWARD KEYBOARD 0 KEY_W -1 1 1\n"
        "setbind 0 COMMAND_LEFT KEYBOARD 0 KEY_A -1 1 1\n"
        "setbind 0 COMMAND_BACKWARD KEYBOARD 0 KEY_S -1 1 1\n"
        "setbind 0 COMMAND_RIGHT KEYBOARD 0 KEY_D -1 1 1\n");

    CHECK_EQ(shh::ReplaceKeyboardPromptTokens("<COMMAND_MOVEMENT>", bindings),
             std::string("[WASD]"));
    CHECK_EQ(shh::ReplaceKeyboardPromptTokens("<COMMAND_LEFT_STICK_LEFTRIGHT>", bindings),
             std::string("[A/D]"));
}

TEST_CASE(ReplaceKeyboardPromptTokensLeavesUnknownTokensUntouched)
{
    const shh::KeyboardPromptBindings bindings;
    CHECK_EQ(shh::ReplaceKeyboardPromptTokens("Press <COMMAND_UNKNOWN> <CLEARCOLOR>", bindings),
             std::string("Press <COMMAND_UNKNOWN> <CLEARCOLOR>"));
}

TEST_CASE(IsLocalizedStringsFileNameAcceptsLanguageTablesOnly)
{
    CHECK_TRUE(shh::IsLocalizedStringsFileName("strings.str"));
    CHECK_TRUE(shh::IsLocalizedStringsFileName("STRINGS_FRA.STR"));
    CHECK_TRUE(!shh::IsLocalizedStringsFileName("m01_dialogue.str"));
}

TEST_CASE(KeyboardPromptScanCodeOverridesUseLiveRemappedKeys)
{
    auto bindings = shh::ParseKeyboardPromptBindings(
        "setbind 0 COMMAND_BLOCK KEYBOARD 0 KEY_Q -1 1 1\n"
        "setbind 0 COMMAND_USE KEYBOARD 0 KEY_TAB -1 1 1\n");
    shh::ApplyKeyboardPromptScanCodeOverrides(bindings, {{8, 0x0F}, {9, 0x32}});

    CHECK_EQ(shh::ReplaceKeyboardPromptTokens("<INVENTORY_WHEEL> <UI_TOGGLE_MAP>", bindings),
             std::string("[Tab] [M]"));
}
