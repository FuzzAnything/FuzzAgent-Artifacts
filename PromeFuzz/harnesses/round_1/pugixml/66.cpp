// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
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
#include <fstream>
#include <sstream>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* Data, size_t Size) {
    // Write fuzz data to dummy file for potential file operations
    writeDummyFile(Data, Size);

    // Create XML document and try to parse the fuzz data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get the root node (could be empty if parsing failed)
    pugi::xml_node root = doc.document_element();
    
    // Prepare stream and writer objects
    std::stringstream ss;
    std::wstringstream wss;
    
    // Test xml_node::print overloads
    if (root) {
        // Test with std::basic_ostream<char>&
        try {
            root.print(ss, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
        } catch (...) {}
        
        // Test with xml_writer&
        try {
            pugi::xml_writer_stream writer(ss);
            root.print(writer, PUGIXML_TEXT("  "), pugi::format_indent, pugi::encoding_utf8, 1);
        } catch (...) {}
    }
    
    // Test xml_document::save overloads
    // Test with std::basic_ostream<char>&
    try {
        doc.save(ss, PUGIXML_TEXT("    "), pugi::format_raw, pugi::encoding_utf16_le);
    } catch (...) {}
    
    // Test with xml_writer&
    try {
        pugi::xml_writer_stream writer(ss);
        doc.save(writer, PUGIXML_TEXT(""), pugi::format_no_declaration, pugi::encoding_latin1);
    } catch (...) {}
    
    // Test xml_writer_file::write
    FILE* dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file) {
        try {
            pugi::xml_writer_file file_writer(dummy_file);
            // Use a subset of the data to avoid excessive writes
            size_t write_size = Size > 1024 ? 1024 : Size;
            file_writer.write(Data, write_size);
        } catch (...) {}
        fclose(dummy_file);
    }
    
    // Test xml_writer_stream::write
    // Test with narrow stream
    try {
        pugi::xml_writer_stream stream_writer(ss);
        // Use a subset of the data
        size_t write_size = Size > 512 ? 512 : Size;
        stream_writer.write(Data, write_size);
    } catch (...) {}
    
    // Test with wide stream - ensure size is multiple of sizeof(wchar_t)
    try {
        pugi::xml_writer_stream wide_writer(wss);
        size_t write_size = Size > 256 ? 256 : Size;
        // Align to wchar_t boundary
        write_size = (write_size / sizeof(wchar_t)) * sizeof(wchar_t);
        if (write_size > 0) {
            wide_writer.write(Data, write_size);
        }
    } catch (...) {}
    
    // Test with various encodings and flags for print/save
    static const pugi::xml_encoding encodings[] = {
        pugi::encoding_auto,
        pugi::encoding_utf8,
        pugi::encoding_utf16_le,
        pugi::encoding_utf16_be,
        pugi::encoding_utf16,
        pugi::encoding_utf32_le,
        pugi::encoding_utf32_be,
        pugi::encoding_utf32,
        pugi::encoding_wchar,
        pugi::encoding_latin1
    };
    
    static const unsigned int flags[] = {
        pugi::format_default,
        pugi::format_indent,
        pugi::format_raw,
        pugi::format_no_declaration,
        pugi::format_no_escapes,
        pugi::format_save_file_text,
        pugi::format_write_bom
    };
    
    // Test a few combinations
    for (int i = 0; i < 3 && i < (int)Size; ++i) {
        pugi::xml_encoding enc = encodings[Data[i] % (sizeof(encodings)/sizeof(encodings[0]))];
        unsigned int flag = flags[Data[(i+1) % Size] % (sizeof(flags)/sizeof(flags[0]))];
        
        std::stringstream test_ss;
        
        // Test xml_node::print with different parameters
        if (root) {
            try {
                const pugi::char_t* indent = (i % 3 == 0) ? PUGIXML_TEXT("\t") : 
                                            (i % 3 == 1) ? PUGIXML_TEXT("  ") : 
                                            PUGIXML_TEXT("");
                root.print(test_ss, indent, flag, enc, i % 4);
            } catch (...) {}
        }
        
        // Test xml_document::save with different parameters
        try {
            const pugi::char_t* indent = (i % 2 == 0) ? PUGIXML_TEXT("    ") : PUGIXML_TEXT("");
            doc.save(test_ss, indent, flag, enc);
        } catch (...) {}
    }
    
    return 0;
}