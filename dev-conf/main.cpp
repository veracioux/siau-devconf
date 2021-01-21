#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <list>

#include "json/json.h"
#include "symbols.h"

/* @brief Write the start of a class declaration to the specified stream.
 *
 * @param  className The name of the class.
 * @param  stream Stream object to which to write.
 */
void declareClass(const std::string &className, std::ostream &stream)
{
    // TODO implement
    stream << "class " << className << " : public Device\n"
        << "{\n";
}

struct Data
{
    std::map<std::string, std::string> attr;
    //std::string name, vendorId, deviceId, serialNo;
};

Data parseJSON(const std::string &filename)
{
    Json::Value root;
    if (filename[0] != '\0')
    {
        std::ifstream file(filename);
        file >> root;
    }
    else
        std::cin >> root;

    Data data;
    // Get class name
    data.attr["name"] = root.get("name", "Device").asString();
    data.attr["vendorId"] = root.get("vendorId", "").asString();
    data.attr["deviceId"] = root.get("deviceId", "").asString();

    return data;
}

void write(const Data &data, std::istream &in, std::ostream &out)
{
    while (!in.eof()) // Read the input file line by line
    {
        // Read a line from the file
        std::string str;
        getline(in, str);

        // Perform substitutions on this line
        for (const auto &pair : data.attr)
        {
            int i = str.find("$" + pair.first, 0);
            if (i != std::string::npos)
                str.replace(i, pair.first.length()+1, pair.second);
        }

        // Write the line to the output file
        out << str << '\n';
    }
}

void write(const Data &data, const std::string &input_file,
        const std::string &output_file)
{
    std::ifstream in(input_file);
    std::ofstream out;
    out.open(output_file, std::ios::out);

    write(data, in, out);
}

void copy_file(const std::string &source, const std::string &destination)
{
    std::ifstream src(source);
    std::ofstream dest(destination);
    dest << src.rdbuf();
}

std::string take_arg(std::list<std::string> &arglist,
        const std::string &defaultValue = "")
{
    if (arglist.empty())
        return defaultValue;

    std::string str;
    str = arglist.front();
    arglist.pop_front();

    return str;
}

void setup(const std::string &setup_dir)
{
    // Datoteka factory_device.json
    copy_file(TEMPLATE_DIR "/factory_device.json",
            setup_dir + "/factory_device.json");

    // Datoteka user_device.json
    copy_file(TEMPLATE_DIR "/user_device.json",
            setup_dir + "/user_device.json");

    std::cout << "Template JSON files created in " << setup_dir << std::endl;
}

#define is_opt(args, o1, o2) ( !args.empty() \
        && (args.front() == (o1) || args.front() == (o2)))

#define if_opt(args, o1, o2) if (is_opt(args, o1, o2))

int main(int argc, char *argv[])
{
    std::string input_dir = "", output_dir = "";
    bool arg_setup = false;

    // Convert arguments to list of strings, for easier manipulation
    std::list<std::string> args;
    for (int i = 1; i < argc; i++)
        args.push_back(argv[i]);

    // No arguments -- print help
    if (args.empty() || is_opt(args, "--help", "-h"))
        return std::cout << "TODO print help" << std::endl, 1;

    // Option setup, create template files in specified directory
    if_opt (args, "--setup", "-s")
    {
        args.pop_front();
        setup(take_arg(args, "."));
    }

    // Option input-dir, specified directory contains configured JSON files
    if_opt (args, "--input-dir", "-i")
    {
        args.pop_front();
        input_dir = take_arg(args, ".");
    }

    // Option output-dir, specified directory contains generated source files
    if_opt (args, "--output-dir", "-o")
    {
        args.pop_front();
        output_dir = take_arg(args, ".");
    }

    if (input_dir == "" && output_dir == "")
        return 0;

    if (input_dir != "" && output_dir == "")
        output_dir = ".";
    if (output_dir != "" && input_dir == "")
        input_dir = ".";

    // Parse JSON into device.h
    Data data = parseJSON(input_dir + "/factory_device.json");
    write(data, TEMPLATE_DIR "/device.h.in", output_dir + "/device.h");

    // TODO just copy the other files for now
    copy_file(TEMPLATE_DIR "/main.cpp.in", output_dir + "/main.cpp");
    copy_file(input_dir + "/factory_device.json", output_dir + "/factory_device.json");
    copy_file(input_dir + "/user_device.json", output_dir + "/user_device.json");

    return 0;
}
   
