#pragma once
#include <windows.h>
#include <string>
#include <regex>

using CmdArgs = std::vector<std::wstring>;

inline std::wstring Quote(std::wstring str)
{
    return L'"' + str + L'"';
}

inline const std::wstring &GetArg(const CmdArgs &args, size_t i)
{
    if (i >= args.size())
        throw std::runtime_error("command expects at least " + std::to_string(i) + " arguments");
    return args[i];
}

inline std::wstring Pad(const std::wstring &str, size_t padding)
{
    return str.size() >= padding ? str : str + std::wstring(padding - str.size(), ' ');
}

inline std::wstring ThisModuleFilePath()
{
    wchar_t result[MAX_PATH + 1];
    result[0] = '\0';
    const auto length = GetModuleFileName(nullptr, result, MAX_PATH);
    return {result, length};
}

inline std::wstring ThisModuleFileName()
{
    const auto path = ThisModuleFilePath();
    const auto pos = path.find_last_of('\\');
    return pos == std::string::npos ? path : path.substr(pos + 1, path.size() - pos - 1);
}

inline CmdArgs CmdLineToArgs(LPWSTR cmdLine)
{
    CmdArgs args;
    auto argc = 0;
    if (wchar_t **wArgs = CommandLineToArgvW(cmdLine, &argc))
    {
        static const auto r = std::wregex(L".*.exe");
        if (!std::regex_match(wArgs[0], r))
            args.push_back(L"");
        args.insert(args.end(), &wArgs[0], &wArgs[argc]);
        LocalFree(wArgs);
    }
    return args;
}
