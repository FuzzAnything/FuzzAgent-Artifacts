// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        fwrite(data, 1, size, file);
        fclose(file);
    }
}

static void cleanupDummyFile() {
    remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* Data, size_t Size) {
    // Write fuzz data to dummy file for potential file operations
    writeDummyFile(Data, Size);
    
    // Parse XML from fuzz data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get root node (may be empty if parsing failed)
    pugi::xml_node root = doc.root();
    
    // Test pugi::xml_node::print overloads
    try {
        // Narrow stream version
        std::stringstream narrow_stream;
        root.print(narrow_stream, PUGIXML_TEXT("\t"), pugi::format_default, 
                   pugi::encoding_auto, 0);
        
        // Wide stream version
        std::wstringstream wide_stream;
        root.print(wide_stream, PUGIXML_TEXT("\t"), pugi::format_default, 0);
        
        // Test with different parameters
        root.print(narrow_stream, PUGIXML_TEXT("  "), pugi::format_indent, 
                   pugi::encoding_utf8, 1);
        root.print(wide_stream, PUGIXML_TEXT("    "), pugi::format_raw, 2);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test pugi::xml_document::save overloads
    try {
        // Narrow stream version
        std::stringstream narrow_stream;
        doc.save(narrow_stream, PUGIXML_TEXT("\t"), pugi::format_default, 
                 pugi::encoding_auto);
        
        // Wide stream version
        std::wstringstream wide_stream;
        doc.save(wide_stream, PUGIXML_TEXT("\t"), pugi::format_default);
        
        // Test with different parameters
        doc.save(narrow_stream, PUGIXML_TEXT("  "), pugi::format_indent, 
                 pugi::encoding_utf8);
        doc.save(wide_stream, PUGIXML_TEXT("    "), pugi::format_raw);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test pugi::xml_writer_file::write
    try {
        FILE* dummy_file = fopen("./dummy_file", "rb");
        if (dummy_file) {
            pugi::xml_writer_file writer(dummy_file);
            writer.write(Data, Size);
            fclose(dummy_file);
        }
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test pugi::xml_writer_stream::write overloads
    try {
        // Narrow stream version
        std::stringstream narrow_stream;
        pugi::xml_writer_stream narrow_writer(narrow_stream);
        narrow_writer.write(Data, Size);
        
        // Wide stream version - ensure size is multiple of sizeof(wchar_t)
        std::wstringstream wide_stream;
        pugi::xml_writer_stream wide_writer(wide_stream);
        size_t aligned_size = (Size / sizeof(wchar_t)) * sizeof(wchar_t);
        if (aligned_size > 0) {
            wide_writer.write(Data, aligned_size);
        }
        
        // Test with partial data
        if (Size > 0) {
            size_t partial_size = Size > 10 ? 10 : Size;
            narrow_writer.write(Data, partial_size);
            
            aligned_size = (partial_size / sizeof(wchar_t)) * sizeof(wchar_t);
            if (aligned_size > 0) {
                wide_writer.write(Data, aligned_size);
            }
        }
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test with empty document/node
    try {
        pugi::xml_document empty_doc;
        pugi::xml_node empty_node;
        
        std::stringstream narrow_stream;
        std::wstringstream wide_stream;
        
        empty_node.print(narrow_stream);
        empty_node.print(wide_stream);
        empty_doc.save(narrow_stream);
        empty_doc.save(wide_stream);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Clean up dummy file
    cleanupDummyFile();
    
    return 0;
}