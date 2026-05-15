// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

static void test_xml_writer_stream_write(const uint8_t* data, size_t size) {
    // Test with narrow stream
    {
        std::stringstream ss;
        pugi::xml_writer_stream writer(ss);
        writer.write(data, size);
    }
    
    // Test with wide stream - ensure size is multiple of sizeof(wchar_t)
    {
        std::wstringstream wss;
        pugi::xml_writer_stream writer(wss);
        size_t adjusted_size = (size / sizeof(wchar_t)) * sizeof(wchar_t);
        if (adjusted_size > 0) {
            writer.write(data, adjusted_size);
        }
    }
}

static void test_xml_writer_file_write(const uint8_t* data, size_t size) {
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        pugi::xml_writer_file writer(file);
        writer.write(data, size);
        fclose(file);
    }
}

static void test_xml_document_save_file(pugi::xml_document& doc) {
    // Test with char* path
    doc.save_file("./dummy_file");
    
    // Test with different encodings and flags
    doc.save_file("./dummy_file", PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto);
    doc.save_file("./dummy_file", PUGIXML_TEXT("  "), pugi::format_indent, pugi::encoding_utf8);
    doc.save_file("./dummy_file", PUGIXML_TEXT(""), pugi::format_raw, pugi::encoding_utf16_le);
}

static void test_xml_node_print(pugi::xml_node& node) {
    // Test with narrow stream writer
    {
        std::stringstream ss;
        pugi::xml_writer_stream writer(ss);
        node.print(writer);
        
        // Test with different parameters
        node.print(writer, PUGIXML_TEXT("  "), pugi::format_indent, pugi::encoding_utf8);
        node.print(writer, PUGIXML_TEXT(""), pugi::format_raw, pugi::encoding_auto, 0);
    }
    
    // Test with wide stream writer and wide-compatible encoding
    {
        std::wstringstream wss;
        pugi::xml_writer_stream writer(wss);
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_wchar);
    }
}

static void test_xml_document_save(pugi::xml_document& doc) {
    // Test with xml_writer overload
    {
        std::stringstream ss;
        pugi::xml_writer_stream writer(ss);
        doc.save(writer);
        
        // Test with different parameters
        doc.save(writer, PUGIXML_TEXT("  "), pugi::format_indent, pugi::encoding_utf8);
    }
    
    // Test with basic_ostream<char> overload
    {
        std::stringstream ss;
        doc.save(ss);
        
        // Test with different parameters
        doc.save(ss, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto);
    }
    
    // Test with wide stream writer and wide-compatible encoding
    {
        std::wstringstream wss;
        pugi::xml_writer_stream writer(wss);
        doc.save(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_wchar);
    }
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for file-based operations
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        fwrite(Data, 1, Size, dummy);
        fclose(dummy);
    }
    
    // Test xml_writer_stream::write with raw data
    test_xml_writer_stream_write(Data, Size);
    
    // Test xml_writer_file::write with raw data
    test_xml_writer_file_write(Data, Size);
    
    // Try to parse the input as XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    
    // If parsing succeeded, test document and node functions
    if (result) {
        // Test xml_document::save_file
        test_xml_document_save_file(doc);
        
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Test xml_node::print if we have a valid node
        if (root) {
            test_xml_node_print(root);
        }
        
        // Test xml_document::save (both overloads)
        test_xml_document_save(doc);
    } else {
        // Even if parsing failed, create an empty document and test some functions
        pugi::xml_document empty_doc;
        pugi::xml_node decl = empty_doc.append_child(pugi::node_declaration);
        decl.append_attribute("version") = "1.0";
        decl.append_attribute("encoding") = "UTF-8";
        
        pugi::xml_node root = empty_doc.append_child("root");
        root.append_child("child").append_attribute("attr") = "value";
        
        // Test with the empty document
        test_xml_document_save_file(empty_doc);
        test_xml_node_print(root);
        test_xml_document_save(empty_doc);
    }
    
    return 0;
}