#include "buffer.hpp"
#include "common_types.hpp"
#include "jpcToJson.hpp"
#include "jsonToJpc.hpp"
#include <iomanip>
#include <filesystem>
#include <iostream>
#include <algorithm>

using namespace std;
// found this style here for basic use for now https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c 
bool does_cmd_exist(char** begin, char** end, string option)
{
    return find(begin, end, option) != end;
}
string get_cmd_option(char** begin, char** end, string option)
{
    if (!does_cmd_exist(begin, end, option))
        return "";
    char** iterator = find(begin, end, option);
    if (++iterator != end)
    {
        string variable = string(*iterator);
        if (variable.at(0) == '-')
        {
            cout << "The " << option << " expects a non-option argument. Use -h for info" << endl;
            return "";
        }
        return variable;
    } else {
        cout << "Input not specifed for " << option << " use -h for info." << endl;
        return "";
    }
}

int main(int argc, char** argv)
{
    bool dump_textures = false;
    bool src_file_specified = false;
    bool dest_folder_specified = false;
    bool texure_folder_specified = false;
    bool change_file_specified = false;

    if (does_cmd_exist(argv, argv+argc, "-h"))
    {
        cout << "To use this program a few files need to be specifed. Their arguments are below." << endl;
        cout << "-h\tThis menu" << endl;
        cout << "-i\tThe input jpc file. This must be JPC version 2-10 or 2-11" << endl;
        cout << "-o\tThe output destination jpc file." << endl;
        cout << "-t\tThe texture folder for replacements" << endl;
        cout << "  \tFor Best use, keep a copy of all textures here too along with your edits." << endl;
        cout << "-c\tThe change file for the source of replacements" << endl;
        cout << "-d\tThis command will dump textures to the folder specifed and output \"full_[input name]_info.json\" for reference" << endl;
        cout << "  \tI recommend using this once. Be carefule as it will replace itself if used multiple times." << endl;
        cout << "  \tIf you are making texture edits, copy the contents into a seperate folder then specify that. This command WILL overwrite textures." << endl;
        cout << "" << endl;
        cout << "Core Commands required are: -i -o -c for input, output, and changes." << endl;
        return 0;
    }
    else
    {
        dump_textures = does_cmd_exist(argv, argv+argc, "-d");
        src_file_specified = does_cmd_exist(argv, argv+argc, "-i");
        dest_folder_specified = does_cmd_exist(argv, argv+argc, "-o");
        texure_folder_specified = does_cmd_exist(argv, argv+argc, "-t");
        change_file_specified = does_cmd_exist(argv, argv+argc, "-c");
        // check core input commands
        if (src_file_specified &&  dest_folder_specified && change_file_specified)
        {

        } else if (dump_textures && src_file_specified) {
            cout << "-d will only dump textures and all jpc info" << endl;
            string dump_folder = get_cmd_option(argv, argv + argc, "-d");
            if (dump_folder == "")
                return 0;
            string src_file_specified = get_cmd_option(argv, argv+argc, "-i");
            if (src_file_specified == "")
                return 0;
            
            // TOD0: check if files are valid


        } else if (dump_textures) {
            cout << "use -i to specify a source file to dump" << endl;
        } else {
            cout << "Not all needed commands are specified " << endl;
            cout << "Need: -i, -o, -c to operate " << endl;
        }
    
    }

    JPAC orig = read_jpc("C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\Common.jpc");
    // read_jpc("Common.jpc", "jpc.json", "dump\\");
    // JPAC jpc = load_from_file("C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\jpc.json", "C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\dump\\", orig);
    edit_from_file(orig, "C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\testChanges.json");
    write_to_file(orig, "C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\output\\Common.jpc");
    return 0;
}