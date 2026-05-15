// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
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
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

static void create_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void remove_dummy_file() {
    std::remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* Data, size_t Size) {
    // Create a dummy file for file-based operations
    create_dummy_file(Data, Size);

    try {
        // Parse XML document from fuzz data
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer(Data, Size);
        
        if (result) {
            // Get root node for testing
            pugi::xml_node root = doc.root();
            
            // Test xml_node::print overloads
            {
                // Test with wchar_t stream
                std::basic_ostringstream<wchar_t> woss;
                root.print(woss, PUGIXML_TEXT("  "), pugi::format_default, 0);
                
                // Test with xml_writer
                std::basic_ostringstream<char> oss;
                pugi::xml_writer_stream writer(oss);
                root.print(writer, PUGIXML_TEXT("\t"), pugi::format_indent, pugi::encoding_utf8, 0);
            }
            
            // Test xml_document::save overloads
            {
                // Test with wchar_t stream
                std::basic_ostringstream<wchar_t> woss;
                doc.save(woss, PUGIXML_TEXT("  "), pugi::format_default);
                
                // Test with xml_writer
                std::basic_ostringstream<char> oss;
                pugi::xml_writer_stream writer(oss);
                doc.save(writer, PUGIXML_TEXT("\t"), pugi::format_indent, pugi::encoding_utf8);
            }
            
            // Test xml_writer_stream::write
            {
                // Test with narrow stream
                std::basic_ostringstream<char> oss;
                pugi::xml_writer_stream writer(oss);
                
                // Write fuzz data with proper alignment for testing
                size_t write_size = Size;
                if (write_size > 0) {
                    writer.write(Data, write_size);
                }
                
                // Test with wide stream
                std::basic_ostringstream<wchar_t> woss;
                pugi::xml_writer_stream wwriter(woss);
                
                // Ensure size is multiple of sizeof(wchar_t) for wide stream
                size_t aligned_size = (Size / sizeof(wchar_t)) * sizeof(wchar_t);
                if (aligned_size > 0) {
                    wwriter.write(Data, aligned_size);
                }
            }
        }
        
        // Test xml_writer_file::write
        {
            FILE* file = fopen("./dummy_file", "rb");
            if (file) {
                pugi::xml_writer_file file_writer(file);
                
                // Write some test data
                const char test_data[] = "test";
                file_writer.write(test_data, sizeof(test_data) - 1);
                
                fclose(file);
            }
        }
        
        // Test with empty document
        pugi::xml_document empty_doc;
        {
            std::basic_ostringstream<wchar_t> woss;
            empty_doc.save(woss);
            
            std::basic_ostringstream<char> oss;
            pugi::xml_writer_stream writer(oss);
            empty_doc.save(writer);
        }
        
        // Test move constructor
        pugi::xml_document moved_doc(std::move(doc));
        
        // Test with different encodings
        if (result) {
            pugi::xml_document doc2;
            doc2.load_buffer(Data, Size);
            
            std::basic_ostringstream<char> oss;
            pugi::xml_writer_stream writer(oss);
            
            // Test various encoding options
            pugi::xml_encoding encodings[] = {
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
            
            for (auto enc : encodings) {
                doc2.save(writer, PUGIXML_TEXT(""), pugi::format_raw, enc);
            }
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}