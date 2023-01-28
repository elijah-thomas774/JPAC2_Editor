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
    read_jpc("Common.jpc", "jpc.json", "dump\\");
}