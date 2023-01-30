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

    if (argc == 1 || does_cmd_exist(argv, argv+argc, "-h"))
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
            string src_file = get_cmd_option(argv, argv+argc, "-i");
            string dest_file = get_cmd_option(argv, argv+argc, "-o");
            string edit_file = get_cmd_option(argv, argv+argc, "-c");
            string tex_folder = get_cmd_option(argv, argv+argc, "-t");
            if (src_file == ""){
                cout << "Source file is invalid" << endl;
                return 0;
            }
            if (dest_file == ""){
                cout << "Destination file is invalid" << endl;
                return 0;
            }
            if (edit_file == ""){
                cout << "Edit file is invalid" << endl;
                return 0;
            }
            if (tex_folder == "" && texure_folder_specified){
                cout << "Texture Folder is invalid" << endl;
                return 0;
            }
            filesystem::path src_path(src_file);
            filesystem::path dest_path(dest_file);
            filesystem::path edit_path(edit_file);
            filesystem::path tex_path;
            if (texure_folder_specified) tex_path = filesystem::path(tex_folder);
            if (!filesystem::exists(src_path) || !filesystem::is_regular_file(src_path) || src_path.extension().string().compare(".jpc") != 0){
                cout << "Soruce file is invalid" << endl;
                return 0;
            }
            if (texure_folder_specified && (!filesystem::exists(tex_path) || !filesystem::is_directory(tex_path))){
                cout << "Texture folder is invalid" << endl;
                return 0;
            }
            if (!filesystem::exists(edit_path) || !filesystem::is_regular_file(edit_path) || edit_path.extension().string().compare(".json") != 0){
                cout << "Edit file is invalid" << endl;
                return 0;
            }
            cout << "Reading source file." << endl;
            JPAC orig = read_jpc(src_file);
            if (orig.version.compare("BAD") == 0){
                return 0;
            }
            if (texure_folder_specified)
                load_from_file(tex_folder, orig);
            cout << "Editing Resources" << endl;
            edit_from_file(orig, edit_file);
            write_to_file(orig, dest_file);
            cout << "Done" << endl;
            return 0;
        } else if (dump_textures && src_file_specified) {
            cout << "-d will only dump textures and all jpc info" << endl;
            string dump_folder = get_cmd_option(argv, argv + argc, "-t");
            if (dump_folder == ""){
                cout << "Specify a folder to dump textures" << endl;
                return 0;
            }
            string src_file = get_cmd_option(argv, argv+argc, "-i");
            if (src_file == "")
                return 0;
            
            // TOD0: check if files are valid
            filesystem::path dump_folder_path(dump_folder);
            filesystem::path src_file_specified_path(src_file);
            src_file_specified_path.assign(src_file);
            if (!filesystem::exists(src_file_specified_path))
            {
                cout << "Input File does could not be read. Make sure you specified the correct path." << endl;
                return 0;
            }
            if (!filesystem::exists(dump_folder_path)){
                cout << "Folder specified for texture dump does not exist. Make sure its entered properly and folder is already created." << endl;
                return 0;
            }
            if (!filesystem::is_directory(dump_folder_path)){
                cout << "The Filedump location requires to be a folder" << endl;
                return 0;
            }
            if (!filesystem::is_regular_file(src_file_specified_path)){
                cout << "The soruce File specified is not a file" << endl;
                return 0;
            }
            if (src_file_specified_path.extension().string().compare(".jpc") != 0){
                cout << "The source file is expected to be a .jpc but got "<< src_file_specified_path.extension().string() << endl;
                return 0;
            }
            cout << "Starting dump process" << endl;
            if (dest_folder_specified)
            {
                string out_file = get_cmd_option(argv, argv+argc, "-o");
                if (out_file == ""){
                    cout << "You need to specify an output file for -o" << endl;
                    return 0;
                }
                read_jpc(src_file, out_file, dump_folder);
                cout << "Finished" << endl;
                return 0;
            }
            string out_file = "full_";
            out_file.append(src_file_specified_path.stem().string());
            out_file.append("_info.json");
            cout << "Outputting to this directory with name: " << out_file << endl;
            read_jpc(src_file, out_file, dump_folder);
            cout << "Finished" << endl;
            return 0;
        } else if (dump_textures) {
            cout << "use -i to specify a source file to dump" << endl;
        } else {
            cout << "Not all needed commands are specified " << endl;
            cout << "Need: -i, -o, -c to operate " << endl;
        }
    }
    return 0;
}