#include "system/Audio.h"
#include "system/ComException.h"
#include "system/ComInstance.h"
#include "system/Console.h"
#include "system/WideException.h"
#include <algorithm>
#include <cassert>
#include <codecvt>
#include <cstdio>
#include <fcntl.h>
#include <functional>
#include <io.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <vector>
#include <windows.h>
#include <regex>
#include "commands.h"

using namespace std::string_literals;
using System::Console;

void ShowCommands(std::wostream &output);
void ShowExamples(std::wostream &output);
void ShowNotes(std::wostream &output);

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
    const auto appVersion = L"0.1"s;
    const auto appName = L"Audio Device Switcher " + appVersion;
    auto consoleArgs = CmdLineToArgs(cmdLine);
    auto mainResult = -1;
    auto &out = std::wcout;
    std::optional<Console> console;
    auto createConsole = [&]()
    {
        if (console.has_value())
            return;
        console.emplace();
        console->title(appName);
        out << "Welcome to " << appName << "\n";
        out << "\n";
    };

    try
    {
        const auto comInstance = ComInstance();
        using CommandFn = std::function<bool(const CmdArgs &args, std::wostream &output)>;

        if (consoleArgs.size() > 1)
        {
            const auto &commandName = consoleArgs[1];
            const auto commands = std::map<std::wstring, CommandFn>{
                {L"set", [](auto &&args, auto &&output)
                 { return Commands::SetDefaultDevice(GetArg(args, 0), output); }},
                {L"toggle", [](auto &&args, auto &&output)
                 { return Commands::Toggle(args, output); }},
                {L"list", [](auto &&args, auto &&output)
                 { return Commands::ListDevices(output); }}};

            const auto command = commands.find(commandName);
            if (command == commands.end())
            {
                createConsole();
                throw BasicWideException(L"command " + Quote(commandName) + L" is not supported\n");
            }
            std::wstringstream res;
            if (!command->second({consoleArgs.begin() + 2, consoleArgs.end()}, res))
            {
                createConsole();
                throw BasicWideException(res.str());
            }
            out << res.str();
            return 0;
        }

        createConsole();
        console->size(40, 135);

        Commands::ListDevices(out);
        ShowCommands(out);
        ShowExamples(out);
        ShowNotes(out);
        mainResult = 0;
    }
    catch (const ComException &e)
    {
        out << "Error: " << e.message() << "[0x" << std::hex << e.errorCode() << "]\n";
    }
    catch (const WideException &e)
    {
        out << "Error: " << e.w_what() << '\n';
    }
    catch (const std::exception &e)
    {
        out << "Error: " << e.what() << '\n';
    }
    out << "\nPress enter to exit...\n";
    std::wcin.get();
    return mainResult;
}

void ShowCommands(std::wostream &output)
{
    using System::Console;
    Console::SetTextColor(Console::Color::Green);
    output << "Commands:\n";
    Console::SetTextColor(Console::Color::Normal);
    output << "  default " << Quote(L"device name") << "\n";
    output << "  toggle " << Quote(L"device name1") << ' ' << Quote(L"device name2") << "\n";
    output << "  list\n";
    output << "\n";
}

void ShowExamples(std::wostream &out)
{
    using System::Console;
    Console::SetTextColor(Console::Color::Green);
    out << "Examples:\n";
    Console::SetTextColor(Console::Color::Normal);
    const auto devices = System::Audio::GetDevices();
    const std::wstring deviceName1 = devices.size() >= 1 ? devices[0].name() : L"Device name 1";
    const std::wstring deviceName2 = devices.size() >= 2 ? devices[1].name() : L"Device name 2";
    const auto moduleName = ThisModuleFileName();
    out << "  " << moduleName << " set " << Quote(deviceName1) << "\n";
    out << "  " << moduleName << " toggle " << Quote(deviceName1) << ' ' << Quote(deviceName2) << '\n';
    out << "\n";
}

void ShowNotes(std::wostream &out)
{
    Console::SetTextColor(Console::Color::Green);
    out << "Notes:\n";
    Console::SetTextColor(Console::Color::Normal);
    out << "  * Device names containing spaces must be quoted as shown in the examples. Same goes for the executable path.\n"
        << "  * If two devices have the same name then go to Window Sound Settings and rename one of them.\n";
}
