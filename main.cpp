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

struct IMAGE_NT_HEADERS {
    // Fields for NT Header
    uint32_t Signature;
};

struct IMAGE_FILE_HEADER {
    // Fields for File Header
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
};

struct IMAGE_OPTIONAL_HEADER {
    // Fields for Optional Header
    uint16_t Magic;
    struct {
        uint32_t VirtualAddress;
        uint32_t Size;
    } DataDirectory[16];
};

struct IMAGE_EXPORT_DIRECTORY {
    // Fields for Export Table
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint32_t Name;
    uint32_t Base;
    uint32_t NumberOfFunctions;
    uint32_t NumberOfNames;
    uint32_t AddressOfFunctions;
    uint32_t AddressOfNames;
    uint32_t AddressOfNameOrdinals;
};

struct IMAGE_IMPORT_DESCRIPTOR {
    // Fields for Import Table
    uint32_t OriginalFirstThunk;
    uint32_t TimeDateStamp;
    uint32_t ForwarderChain;
    uint32_t Name;
    uint32_t FirstThunk;
};

struct IMAGE_SECTION_HEADER {
    // Fields for Section Header
    char Name[8];
    uint32_t VirtualSize;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t Characteristics;
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

void readNtHeader(ifstream& file) {
    // Move to the NT Header position
    file.seekg(0x3C, ios::beg);
    uint32_t ntHeaderOffset;
    file.read(reinterpret_cast<char*>(&ntHeaderOffset), sizeof(uint32_t));

    file.seekg(ntHeaderOffset, ios::beg);

    IMAGE_NT_HEADERS ntHeaders;
    file.read(reinterpret_cast<char*>(&ntHeaders), sizeof(IMAGE_NT_HEADERS));

    cout << "\nNT Header:"
         << "\n  Signature: " << ntHeaders.Signature
         << endl;

    // Extract IMAGE_FILE_HEADER and IMAGE_OPTIONAL_HEADER from ntHeaders
    IMAGE_FILE_HEADER fileHeader;
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(IMAGE_FILE_HEADER));

    cout << "  File Header:"
         <<"\n    Machine: " << fileHeader.Machine
         <<"\n    Number of Sections: " << fileHeader.NumberOfSections
         <<"\n    Time Date Stamp: " << hex << showbase << fileHeader.TimeDateStamp
         <<"\n    Pointer To Symbol Table:  "<<hex<<showbase<<fileHeader.PointerToSymbolTable
         <<"\n    Number Of Symbols:  "<<hex<<showbase<<fileHeader.NumberOfSymbols
         <<"\n    Characteristics:  "<<hex<<showbase<<fileHeader.Characteristics
         << endl;

    IMAGE_OPTIONAL_HEADER optionalHeader;
    file.read(reinterpret_cast<char*>(&optionalHeader), sizeof(IMAGE_OPTIONAL_HEADER));

    cout << "  Optional Header:"
         << "\n    Magic: " << optionalHeader.Magic
         << endl;

    // Read and print Export Table
    file.seekg(optionalHeader.DataDirectory[0].VirtualAddress, ios::beg);
    IMAGE_EXPORT_DIRECTORY exportDirectory;
    file.read(reinterpret_cast<char*>(&exportDirectory), sizeof(IMAGE_EXPORT_DIRECTORY));

    cout << "\nExport Table:"
         << "\n  Characteristics: " << exportDirectory.Characteristics
         << "\n  TimeDateStamp: " << exportDirectory.TimeDateStamp
         << "\n  Major Version: " << exportDirectory.MajorVersion
         << "\n  Minor Version: " << exportDirectory.MinorVersion
         << "\n  Name: " << exportDirectory.Name
         << "\n  Base: " << exportDirectory.Base
         << "\n  Number of Functions: " << exportDirectory.NumberOfFunctions
         << "\n  Number of Names: " << exportDirectory.NumberOfNames
         << "\n  Address of Functions: " << exportDirectory.AddressOfFunctions
         << "\n  Address of Names: " << exportDirectory.AddressOfNames
         << "\n  Address of Name Ordinals: " << exportDirectory.AddressOfNameOrdinals
         << endl;

    // Read and print Import Table
    file.seekg(optionalHeader.DataDirectory[1].VirtualAddress, ios::beg);
    IMAGE_IMPORT_DESCRIPTOR importDescriptor;
    file.read(reinterpret_cast<char*>(&importDescriptor), sizeof(IMAGE_IMPORT_DESCRIPTOR));

    cout << "\nImport Table:"
         << "\n  Name: " << importDescriptor.Name
         << "\n  OriginalFirstThunk: " << importDescriptor.OriginalFirstThunk
         << "\n  TimeDateStamp: " << importDescriptor.TimeDateStamp
         << "\n  ForwarderChain: " << importDescriptor.ForwarderChain
         << "\n  Name: " << importDescriptor.Name
         << "\n  FirstThunk: " << importDescriptor.FirstThunk
         << endl;


    // Read and print Section Headers
    file.seekg(sizeof(IMAGE_DOS_HEADER) + fileHeader.SizeOfOptionalHeader + sizeof(IMAGE_FILE_HEADER), ios::beg);
    for (int i = 0; i < fileHeader.NumberOfSections; ++i) {
        IMAGE_SECTION_HEADER sectionHeader;
        file.read(reinterpret_cast<char*>(&sectionHeader), sizeof(IMAGE_SECTION_HEADER));

        cout << "\nSection Header " << i + 1 << ":"
             << "\n  Name: " << sectionHeader.Name
             << "\n  VirtualSize: " << sectionHeader.VirtualSize
             << "\n  VirtualAddress: " << sectionHeader.VirtualAddress
             << "\n  SizeOfRawData: " << sectionHeader.SizeOfRawData
             << "\n  PointerToRawData: " << sectionHeader.PointerToRawData
             << "\n  Characteristics: " << sectionHeader.Characteristics
             << endl;
    }
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

    // Read and print NT Header, Export Table, Import Table, and Section Headers
    readNtHeader(file);

    file.close();

    return 0;
}
