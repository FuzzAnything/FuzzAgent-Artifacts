// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void writeDummyFileW(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file_w", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;

    // Prepare dummy files
    writeDummyFile(Data, Size);
    writeDummyFileW(Data, Size);

    // Extract parameters from fuzzer input
    size_t offset = 0;
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= sizeof(unsigned int)) {
        options = *reinterpret_cast<const unsigned int*>(Data);
        offset += sizeof(unsigned int);
    }
    
    if (offset < Size) {
        uint8_t enc_byte = Data[offset] % 10; // Ensure 0-9
        encoding = static_cast<pugi::xml_encoding>(enc_byte);
        offset++;
    }

    // Create XML document
    pugi::xml_document doc;
    
    // 1. Test load_string
    if (Size - offset > 0) {
        const char* xml_content = reinterpret_cast<const char*>(Data + offset);
        std::vector<char> null_terminated(xml_content, xml_content + (Size - offset));
        if (!null_terminated.empty()) {
            null_terminated.push_back('\0');
            doc.load_string(null_terminated.data(), options);
        }
    }

    // 2. Test save_file overloads
    const char* indent = "\t";
    unsigned int flags = pugi::format_default;
    
    // char* version
    doc.save_file("./dummy_file_save", indent, flags, encoding);
    
    // wchar_t* version
    std::wstring wpath = L"./dummy_file_save_w";
    doc.save_file(wpath.c_str(), indent, flags, encoding);

    // 3. Test load from stream
    if (Size > 0) {
        std::istringstream iss(std::string(reinterpret_cast<const char*>(Data), Size));
        pugi::xml_document doc2;
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc2.load(iss, options, encoding);
        }
    }

    // 4. Test save to stream
    std::ostringstream oss;
    doc.save(oss, indent, flags, encoding);

    // 5. Test load_file
    pugi::xml_document doc3;
    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
        doc3.load_file("./dummy_file", options, encoding);
    }

    // Clean up temporary files (optional)
    std::remove("./dummy_file");
    std::remove("./dummy_file_w");
    std::remove("./dummy_file_save");
    std::remove("./dummy_file_save_w");

    return 0;
}