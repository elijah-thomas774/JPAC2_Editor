#include "buffer.hpp"
#include "common_types.hpp"
#include "jpcToJson.hpp"
#include "jsonToJpc.hpp"
#include <iomanip>
#include <iostream>

using namespace std;


void print_buffer(Buffer &buffer)
{   

    vector<u8> data = buffer.data;
    for (i32 i = 0; i < data.size(); i++)
    {
        if (i%16 == 0 && i != 0)
        {
            cout << endl;
        }
        cout << setw(2) << hex << setfill('0') << (u32)data.at(i) << " ";
    }
    cout << endl;
    cout << endl;

}


int main()
{
    JPAC orig = read_jpc("C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\Common.jpc");
    // read_jpc("Common.jpc", "jpc.json", "dump\\");
    // JPAC jpc = load_from_file("C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\jpc.json", "C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\dump\\", orig);
    edit_from_file(orig, "C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\testChanges.json");
    write_to_file(orig, "C:\\Users\\elija\\Documents\\GitHub\\JPAC2_Editor\\output\\Common.jpc");
}