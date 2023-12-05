#include <iostream>
#include <fstream>
#include <cstdint>


using namespace std;
// Changes the default struct packing alignment to 1 byte
#pragma pack(push, 1)

struct IMAGE_DOS_HEADER {
    uint16_t e_magic;
    int32_t e_lfanew;
};

// Restores the default struct packing alignment.
#pragma pack(pop)

void readDosHeader(ifstream& file) {
    IMAGE_DOS_HEADER dosHeader;
    file.read(reinterpret_cast<char*>(&dosHeader), sizeof(IMAGE_DOS_HEADER));

    cout << "DOS Header:" << hex << uppercase
         << "\n  Magic Number: " << dosHeader.e_magic
         << "\n  Address of New Exe Header: " << dosHeader.e_lfanew
         << endl;
}

int main() {
    const char* filePath = "G:\\test.exe";
    // Opens the file in binary mode
    ifstream file(filePath, ios::binary);

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    // Read and print DOS Header
    readDosHeader(file);

    file.close();

    return 0;
}
