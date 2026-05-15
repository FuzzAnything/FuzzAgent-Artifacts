// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
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
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // First, try to parse the input as XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get the root node
    pugi::xml_node root = doc.root();
    
    // Create string streams for output
    std::stringstream ss_narrow;
    std::basic_ostringstream<pugi::char_t> ss_wide;
    
    // Test xml_node::print overloads
    if (result) {
        // Overload 1: print to narrow stream
        try {
            root.print(ss_narrow, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
        } catch (...) {
            // Ignore exceptions during fuzzing
        }
        
        // Overload 2: print to writer object
        try {
            pugi::xml_writer_stream writer_narrow(ss_narrow);
            root.print(writer_narrow, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
        } catch (...) {
            // Ignore exceptions during fuzzing
        }
        
        // Test with different encodings and flags
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
            pugi::format_no_declaration,
            pugi::format_no_escapes,
            pugi::format_save_file_text,
            pugi::format_write_bom,
            pugi::format_raw,
            pugi::format_indent_attributes
        };
        
        // Sample a few combinations based on input data
        if (Size > 0) {
            size_t enc_idx = Data[0] % (sizeof(encodings)/sizeof(encodings[0]));
            size_t flags_idx = Size > 1 ? Data[1] % (sizeof(flags)/sizeof(flags[0])) : 0;
            
            try {
                root.print(ss_narrow, PUGIXML_TEXT("  "), flags[flags_idx], encodings[enc_idx], 0);
            } catch (...) {
                // Ignore exceptions during fuzzing
            }
        }
    }
    
    // Test xml_document::save overloads
    // Overload 1: save to narrow stream
    try {
        doc.save(ss_narrow, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Overload 2: save to writer object
    try {
        pugi::xml_writer_stream writer_narrow(ss_narrow);
        doc.save(writer_narrow, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto);
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Test xml_writer_file::write
    // Create a dummy file
    static const char* dummy_file = "./dummy_file";
    FILE* file = fopen(dummy_file, "wb");
    if (file) {
        try {
            pugi::xml_writer_file writer_file(file);
            // Write the input data to the file using the writer
            writer_file.write(Data, Size);
            
            // Also write some XML content if we parsed successfully
            if (result) {
                std::string xml_content = ss_narrow.str();
                writer_file.write(xml_content.data(), xml_content.size());
            }
        } catch (...) {
            // Ignore exceptions during fuzzing
        }
        fclose(file);
    }
    
    // Test xml_writer_stream::write
    try {
        pugi::xml_writer_stream writer_stream(ss_narrow);
        
        // Write the input data
        writer_stream.write(Data, Size);
        
        // Also write some XML content if we parsed successfully
        if (result) {
            std::string xml_content = ss_narrow.str();
            writer_stream.write(xml_content.data(), xml_content.size());
        }
        
        // Test with wide stream if PUGIXML_WCHAR_MODE is enabled
        #ifdef PUGIXML_WCHAR_MODE
        // For wide streams, ensure size is multiple of sizeof(wchar_t)
        size_t aligned_size = Size - (Size % sizeof(wchar_t));
        if (aligned_size > 0) {
            pugi::xml_writer_stream writer_wide(ss_wide);
            writer_wide.write(Data, aligned_size);
        }
        #endif
        
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    // Clean up dummy file
    std::remove(dummy_file);
    
    return 0;
}