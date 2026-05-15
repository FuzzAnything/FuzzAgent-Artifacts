// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
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
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static void cleanup_dummy_file() {
    std::remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* Data, size_t Size) {
    // Create dummy file for file operations
    create_dummy_file(Data, Size);
    
    // Initialize XML document with fuzz data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Test pugi::xml_document::save_file
    if (Size > 0) {
        // Test with default parameters
        doc.save_file("./dummy_file");
        
        // Test with different formatting options
        doc.save_file("./dummy_file", PUGIXML_TEXT("  "), pugi::format_indent);
        doc.save_file("./dummy_file", PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);
        doc.save_file("./dummy_file", PUGIXML_TEXT(""), pugi::format_raw, pugi::encoding_auto);
    }
    
    // Test pugi::xml_writer_file::write
    {
        FILE* file = fopen("./dummy_file", "wb");
        if (file) {
            pugi::xml_writer_file writer(file);
            // Write fuzz data directly
            writer.write(Data, Size);
            // Write some XML content
            if (result) {
                std::stringstream ss;
                doc.save(ss);
                std::string content = ss.str();
                writer.write(content.c_str(), content.size());
            }
            fclose(file);
        }
    }
    
    // Test pugi::xml_writer_stream::write
    {
        // Test with narrow stream
        std::stringstream narrow_stream;
        pugi::xml_writer_stream narrow_writer(narrow_stream);
        narrow_writer.write(Data, Size);
        
        // Test with wide stream - ensure size is multiple of sizeof(wchar_t)
        std::wstringstream wide_stream;
        pugi::xml_writer_stream wide_writer(wide_stream);
        size_t aligned_size = (Size / sizeof(wchar_t)) * sizeof(wchar_t);
        if (aligned_size > 0) {
            wide_writer.write(Data, aligned_size);
        }
    }
    
    // Test pugi::xml_node::print
    if (result) {
        pugi::xml_node root = doc.root();
        
        // Test with narrow stream
        std::stringstream narrow_stream;
        root.print(narrow_stream);
        root.print(narrow_stream, PUGIXML_TEXT("  "), pugi::format_indent);
        root.print(narrow_stream, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);
        
        // Test with wide stream and wchar_t encoding
        std::wstringstream wide_stream;
        root.print(wide_stream, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_wchar);
    }
    
    // Test pugi::xml_document::save overloads
    {
        // First overload: save to stream
        std::stringstream stream;
        doc.save(stream);
        doc.save(stream, PUGIXML_TEXT("  "), pugi::format_indent);
        doc.save(stream, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);
        
        // Second overload: save to writer
        std::stringstream writer_stream;
        pugi::xml_writer_stream writer(writer_stream);
        doc.save(writer);
        doc.save(writer, PUGIXML_TEXT("  "), pugi::format_indent);
        doc.save(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);
        
        // Test with file writer
        FILE* file = fopen("./dummy_file", "wb");
        if (file) {
            pugi::xml_writer_file file_writer(file);
            doc.save(file_writer);
            fclose(file);
        }
    }
    
    // Cleanup
    cleanup_dummy_file();
    
    return 0;
}