// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
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
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file operations
    writeDummyFile(Data, Size);
    
    // Parse XML document from input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get root node for testing
    pugi::xml_node root = doc.root();
    
    // Test xml_node::print overloads
    try {
        // Narrow stream version
        std::stringstream narrow_stream;
        root.print(narrow_stream, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
        
        // Wide stream version
        std::wstringstream wide_stream;
        root.print(wide_stream, PUGIXML_TEXT("\t"), pugi::format_default, 0);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test xml_document::save overloads
    try {
        // Narrow stream version with different encodings
        std::stringstream narrow_save_stream;
        doc.save(narrow_save_stream, PUGIXML_TEXT("  "), pugi::format_default, pugi::encoding_utf8);
        
        // Wide stream version
        std::wstringstream wide_save_stream;
        doc.save(wide_save_stream, PUGIXML_TEXT("  "), pugi::format_default);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test xml_writer_stream::write
    try {
        // Narrow stream writer
        std::stringstream narrow_oss;
        pugi::xml_writer_stream narrow_writer(narrow_oss);
        narrow_writer.write(Data, Size);
        
        // Wide stream writer with size alignment for wchar_t
        std::wstringstream wide_oss;
        pugi::xml_writer_stream wide_writer(wide_oss);
        size_t aligned_size = (Size / sizeof(wchar_t)) * sizeof(wchar_t);
        if (aligned_size > 0) {
            wide_writer.write(Data, aligned_size);
        }
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test xml_writer_file::write
    try {
        FILE* dummy_file = fopen("./dummy_file", "rb");
        if (dummy_file) {
            pugi::xml_writer_file file_writer(dummy_file);
            file_writer.write(Data, Size);
            fclose(dummy_file);
        }
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test with different formatting flags
    try {
        std::stringstream ss1;
        root.print(ss1, PUGIXML_TEXT("  "), pugi::format_indent | pugi::format_write_bom);
        
        std::stringstream ss2;
        doc.save(ss2, PUGIXML_TEXT("\t"), pugi::format_raw | pugi::format_no_declaration, pugi::encoding_utf16_le);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test with empty document
    try {
        pugi::xml_document empty_doc;
        std::stringstream empty_stream;
        empty_doc.save(empty_stream);
        
        std::wstringstream empty_wstream;
        empty_doc.save(empty_wstream);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test with different indent strings
    try {
        std::stringstream ss3;
        root.print(ss3, PUGIXML_TEXT("    "), pugi::format_default, pugi::encoding_latin1, 0);
        
        std::wstringstream ss4;
        root.print(ss4, PUGIXML_TEXT(""), pugi::format_default, 0);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    return 0;
}