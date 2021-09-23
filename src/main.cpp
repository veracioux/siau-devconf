/** @file main.cpp */

#include "devlib/device.h"
#include "devlib/json.h"
#include "file.h"
#include "userdata.h"

#include <iostream>
#include <QList>
#include <QString>
#include <QDir>
#include <QProcess>

/**
 * Bool value that indicates if the first argument in the list `args` is
 * equal to `o1` or `o2`. `o1` is the long option name (e.g. --help) and
 * `o2` is the short option name (e.g. -h).
 */
#define is_option(args, o1, o2)                                                \
    (!args.empty() && (args.front() == (o1) || args.front() == (o2)))

#define if_option(args, o1, o2) if (is_option(args, o1, o2))

/**
 * @brief Take the first argument from a list of arguments, then pop it.
 * @param arglist The list of arguments.
 * @param defaultValue Fallback value in case `arglist` is empty.
 * @return The first argument from `arglist`.
 */
QString takeArg(QList<QString>& arglist, const QString& defaultValue = "")
{
    if (arglist.empty())
        return defaultValue;

    QString str;
    str = arglist.front();
    arglist.pop_front();

    return str;
}

/**
 * @brief Obtain template directory.
 * Uses the environment variable `DEVCONF_TEMPLATE_DIR` if set, otherwise use a
 * default system path.
 */
QString templateDir()
{
    const char* templateDir = std::getenv("DEVCONF_TEMPLATE_DIR");
    if (templateDir == nullptr)
        return QStringLiteral(TEMPLATE_DIR);
    return QString(templateDir);
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
bool copyTemplate(const QString& targetDir)
{
    QString templateDir = ::templateDir();
    // File factory_device.json
    if (!copyFile(QString(templateDir) + "/factory_device.json",
                  targetDir + "/factory_device.json"))
        return false;

    // File user_device.json
    if (!copyFile(QString(templateDir) + "/user_device.json",
                  targetDir + "/user_device.json"))
        return false;

    std::cout << "Template JSON files created in " << targetDir.toStdString()
              << std::endl;
    return true;
}

void generateOutputFiles(const QString& inputDir, const QString& outputDir)
{
    QDir(outputDir).mkpath(".");
    // Parse JSON files into device.h
    Device device = jsonParseDevice(inputDir + "/factory_device.json");
    writeDeviceHeader(device, templateDir() + "/iot_device.h.in", outputDir + "/iot_device.h");
    writeDeviceImpl(device, outputDir + "/iot_device.cpp");

    if (!QDir(outputDir).mkpath("autogen"))
        throw std::runtime_error("Could not create autogen directory");

    UserData userData = UserData::fromJson(inputDir + "/user_device.json");
    auto substitutions = userData.getAttributes();
    substitutions["model"] = device.getModel();
    substituteInFile(templateDir() + "/autogen/user_device.h.in",
             outputDir + "/autogen/user_device.h",
             substitutions);
    writeMessageHandlers(templateDir() + "/autogen/message_handler.h.in",
                 outputDir + "/autogen/message_handler.h", device);
    writeMessageHandlers(templateDir() + "/autogen/message_handler.cpp.in",
                 outputDir + "/autogen/message_handler.cpp", device);
    writeMqttWrapperImpl(templateDir() + "/autogen/mqtt_wrapper.cpp.in",
                   outputDir + "/autogen/mqtt_wrapper.cpp", device);

    // Just copy the other files
    copyFile(templateDir() + "/main.cpp", outputDir + "/main.cpp");
    copyFile(inputDir + "/factory_device.json",
         outputDir + "/factory_device.json");
    copyFile(inputDir + "/user_device.json", outputDir + "/user_device.json");
    copyFile(templateDir() + "/autogen/mqtt_wrapper.h.in", outputDir + "/autogen/mqtt_wrapper.h");
}

int initMbedWorkspace(const QString& outputDir)
{
    QProcess *mbedCli = new QProcess;
    mbedCli->setProcessChannelMode(QProcess::ForwardedChannels);

    mbedCli->start("mbed", {"new", outputDir});
    mbedCli->waitForFinished();
    int exitCode = mbedCli->exitCode();
    if (exitCode != 0)
        return exitCode;

    mbedCli->setWorkingDirectory(outputDir);
    mbedCli->start("mbed", {"add", "mbed-mqtt"});
    mbedCli->waitForFinished();
    exitCode = mbedCli->exitCode();
    if (exitCode != 0)
        return exitCode;

    delete mbedCli;
    return 0;
}

int main(int argc, char* argv[])
{
    QString inputDir, outputDir, templateDestination, mbedDir;
    bool _template = false, _input_dir = false, _output_dir = false, _mbed = false;

    // Convert arguments to list of strings, for easier manipulation
    QList<QString> args;
    for (int i = 1; i < argc; i++)
        args.push_back(argv[i]);

    // No arguments -- print help
    if (args.empty() || args.contains("--help") || args.contains("-h")) {
        std::cout << "usage: devconf [-h] [-i IN_DIR] [-o OUT_DIR] [-t OUT] [-m [MBED_DIR]]\n\n"
                     "  -h, --help                show this help message and exit\n"
                     "  -t, --template OUT        copy JSON template into OUT dir\n"
                     "  -i, --input-dir IN_DIR    input directory containing JSON files\n"
                     "  -o, --output-dir OUT_DIR  output directory for mbed source\n"
                     "  -m, --mbed                initialize mbed workspace in output dir\n"
                  << std::endl;
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        if_option(args, "--template", "-t") {
            args.pop_front();
            _template = true;
            templateDestination = takeArg(args, ".");
        }
        else if_option(args, "--input-dir", "-i") {
            args.pop_front();
            _input_dir = true;
            inputDir = takeArg(args, ".");
        }
        else if_option(args, "--output-dir", "-o") {
            args.pop_front();
            _output_dir = true;
            outputDir = takeArg(args, ".");
        } else if_option(args, "--mbed", "-m") {
            args.pop_front();
            _mbed = true;
            if (!args.isEmpty() && !args.front().startsWith("-"))
                mbedDir = takeArg(args);
        }
    }

    if (_template && (_input_dir || _output_dir || _mbed)) {
        std::cerr << "devconf: error: --template can only be used alone"
                  << std::endl;
        return 1;
    }

    // Create template files in specified directory
    if (_template) {
        if (!copyTemplate(templateDestination))
            return 1;
        return 0;
    }

    if (!_input_dir && !_output_dir && !_mbed) {
        return 1;
    } else if (QFileInfo(inputDir).absoluteFilePath()
               == QFileInfo(outputDir).absoluteFilePath()) {
        std::cerr << "devconf: error: Input and output directory cannot be the same\n";
        return 1;
    }

    // Output dir not specified => default to ./
    if (_input_dir && !_output_dir)
        outputDir = ".";
    // Input dir not specified => default to ./
    if (!_input_dir && _output_dir)
        inputDir = ".";

    if (!inputDir.isEmpty()) {
        try {
            generateOutputFiles(inputDir, outputDir);
        } catch (std::exception &e) {
            std::cerr << "devconf: error: " << e.what();
            return 1;
        }
    }

    if (mbedDir.isEmpty())
        mbedDir = outputDir;

    if (_mbed) {
        initMbedWorkspace(mbedDir);
    }

    return 0;
}
