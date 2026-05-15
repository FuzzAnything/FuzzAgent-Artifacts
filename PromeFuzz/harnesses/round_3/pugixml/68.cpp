// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
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
#include <cstdio>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* Data, size_t Size) {
    // Parse XML document from fuzz input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get root node for testing
    pugi::xml_node root = doc.root();
    
    // Test xml_node::print overloads
    // 1. Print to narrow stream
    try {
        std::ostringstream oss_narrow;
        root.print(oss_narrow);
    } catch (...) {}
    
    // 2. Print to writer object (using xml_writer_stream with narrow stream)
    try {
        std::ostringstream oss_writer;
        pugi::xml_writer_stream writer_narrow(oss_writer);
        root.print(writer_narrow);
    } catch (...) {}
    
    // Test xml_writer_stream::write
    // Narrow stream version
    try {
        std::ostringstream oss_write;
        pugi::xml_writer_stream writer_stream(oss_write);
        writer_stream.write(Data, Size);
    } catch (...) {}
    
    // Wide stream version (with size alignment)
    try {
        std::wostringstream woss;
        pugi::xml_writer_stream writer_wide(woss);
        size_t aligned_size = Size - (Size % sizeof(wchar_t));
        if (aligned_size > 0) {
            writer_wide.write(Data, aligned_size);
        }
    } catch (...) {}
    
    // Test xml_document::save overloads
    // 1. Save to narrow stream
    try {
        std::ostringstream oss_save_narrow;
        doc.save(oss_save_narrow);
    } catch (...) {}
    
    // 2. Save to writer object
    try {
        std::ostringstream oss_save_writer;
        pugi::xml_writer_stream writer_save(oss_save_writer);
        doc.save(writer_save);
    } catch (...) {}
    
    // Test xml_writer_file::write
    // Create dummy file with fuzz data
    writeDummyFile(Data, Size);
    
    try {
        FILE* file = fopen("./dummy_file", "rb");
        if (file) {
            pugi::xml_writer_file writer_file(file);
            writer_file.write(Data, Size);
            fclose(file);
        }
    } catch (...) {}
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}