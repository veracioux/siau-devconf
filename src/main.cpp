/** @file main.cpp */

#include "file.h"
#include "devlib/json.h"
#include "devlib/device.h"

#include <fstream>
#include <iostream>
#include <QString>
#include <QList>

/**
  * Bool value that indicates if the first argument in the list `args` is
  * equal to `o1` or `o2`. `o1` is the long option name (e.g. --help) and
  * `o2` is the short option name (e.g. -h).
  */
#define is_option(args, o1, o2) ( !args.empty() \
        && (args.front() == (o1) || args.front() == (o2)))

#define if_option(args, o1, o2) if (is_option(args, o1, o2))

/**
 * @brief Take the first argument from a list of arguments, then pop it.
 * @param arglist The list of arguments.
 * @param defaultValue Fallback value in case `arglist` is empty.
 * @return The first argument from `arglist`.
 */
QString takeArg(QList<QString> &arglist,
        const QString &defaultValue = "")
{
    if (arglist.empty())
        return defaultValue;

    QString str;
    str = arglist.front();
    arglist.pop_front();

    return str;
}

/**
 * @brief Generate template JSON files in the specified directory.
 *
 * This function is to be called when the `--template` option is
 * specified in the command line.
 *
 * @param targetDir The target directory
 * @return Whether the operation was successful.
 */
bool copyTemplate(const QString &targetDir)
{
    // File factory_device.json
    if (!copyFile(TEMPLATE_DIR "/factory_device.json",
                targetDir + "/factory_device.json"))
        return false;

    // File user_device.json
    if (!copyFile(TEMPLATE_DIR "/user_device.json",
            targetDir + "/user_device.json"))
        return false;

    std::cout << "Template JSON files created in " << targetDir.toStdString() << std::endl;
    return true;
}

int main(int argc, char *argv[])
{
    QString input_dir, output_dir;

    // Convert arguments to list of strings, for easier manipulation
    QList<QString> args;
    for (int i = 1; i < argc; i++)
        args.push_back(argv[i]);

    // No arguments -- print help
    if (args.empty() || is_option(args, "--help", "-h"))
        return std::cout << "TODO print help" << std::endl, 1;

    // Option --template, create template files in specified directory
    if_option (args, "--template", "-t")
    {
        args.pop_front();
        if (!copyTemplate(takeArg(args, ".")))
            return 1;
    }

    // Option --input-dir, specified directory is supposed to
    // contain configured JSON files
    if_option (args, "--input-dir", "-i")
    {
        args.pop_front();
        input_dir = takeArg(args, ".");
    }

    // Option --output-dir, specified directory is supposed to
    // contain generated source files
    if_option (args, "--output-dir", "-o")
    {
        args.pop_front();
        output_dir = takeArg(args, ".");
    }

    if (input_dir == "" && output_dir == "")
        return 0;

    // Output dir not specified, default to ./
    if (input_dir != "" && output_dir == "")
        output_dir = ".";
    // Input dir not specified, default to ./
    if (output_dir != "" && input_dir == "")
        input_dir = ".";

    // Parse JSON files into device.h
    Device data = jsonParseDevice(input_dir + "/factory_device.json");
    write(data, TEMPLATE_DIR "/device.h.in", output_dir + "/device.h");

    // TODO just copy the other files for now
    copyFile(TEMPLATE_DIR "/main.cpp.in", output_dir + "/main.cpp");
    copyFile(input_dir + "/factory_device.json", output_dir + "/factory_device.json");
    copyFile(input_dir + "/user_device.json", output_dir + "/user_device.json");

    return 0;
}
