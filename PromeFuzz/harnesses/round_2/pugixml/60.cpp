// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
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
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static std::string extractString(const uint8_t* data, size_t size, size_t max_len = 256) {
    size_t len = size < max_len ? size : max_len;
    std::string result;
    for (size_t i = 0; i < len; ++i) {
        if (data[i] == 0) break;
        result.push_back(static_cast<char>(data[i]));
    }
    if (result.empty() && len > 0) {
        result.push_back('a');
    }
    return result;
}

static void traverseNode(pugi::xml_node node, const uint8_t* Data, size_t Size) {
    // Test name()
    const char* name = node.name();
    (void)name;
    
    // Test value()
    const char* value = node.value();
    (void)value;
    
    // Test type()
    pugi::xml_node_type node_type = node.type();
    (void)node_type;
    
    // Test print() with string stream
    std::ostringstream oss;
    pugi::xml_writer_stream writer(oss);
    try {
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
    } catch (...) {
        // Ignore exceptions from print
    }
    
    // Test print() with file writer
    FILE* dummy_file = fopen("./dummy_file", "a");
    if (dummy_file) {
        pugi::xml_writer_file file_writer(dummy_file);
        try {
            node.print(file_writer, PUGIXML_TEXT("  "), pugi::format_raw, pugi::encoding_utf8, 0);
        } catch (...) {
            // Ignore exceptions
        }
        fclose(dummy_file);
    }
    
    // Test set_value() on appropriate node types
    if (node_type == pugi::node_pcdata || node_type == pugi::node_cdata ||
        node_type == pugi::node_comment || node_type == pugi::node_pi ||
        node_type == pugi::node_doctype) {
        std::string test_value = extractString(Data, Size, 100);
        node.set_value(test_value.c_str());
    }
    
    // Test set_name() on appropriate node types
    if (node_type == pugi::node_element || node_type == pugi::node_pi ||
        node_type == pugi::node_declaration) {
        std::string test_name = extractString(Data + Size/2, Size/2, 50);
        node.set_name(test_name.c_str());
    }
    
    // Recursively process children
    for (pugi::xml_node child : node.children()) {
        traverseNode(child, Data, Size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to dummy file for potential file-based parsing
    writeDummyFile(Data, Size);

    // Parse XML from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (result) {
        traverseNode(doc.root(), Data, Size);
    }
    
    // Also test on null/empty nodes
    pugi::xml_node null_node;
    null_node.name();
    null_node.value();
    null_node.type();
    
    // Test on newly created nodes
    pugi::xml_document new_doc;
    pugi::xml_node new_node = new_doc.append_child(pugi::node_element);
    new_node.set_name("test_element");
    new_node.set_value("test_value");
    
    // Test all functions on the new node
    new_node.name();
    new_node.value();
    new_node.type();
    
    std::ostringstream oss2;
    pugi::xml_writer_stream writer2(oss2);
    try {
        new_node.print(writer2, PUGIXML_TEXT("\t"), pugi::format_indent, pugi::encoding_utf8, 0);
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test with different encodings for print
    std::vector<pugi::xml_encoding> encodings = {
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
    
    for (auto encoding : encodings) {
        std::ostringstream oss3;
        pugi::xml_writer_stream writer3(oss3);
        try {
            new_node.print(writer3, PUGIXML_TEXT(""), pugi::format_raw, encoding, 0);
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test with different formatting flags
    unsigned int flags[] = {
        pugi::format_default,
        pugi::format_indent,
        pugi::format_raw,
        pugi::format_no_declaration,
        pugi::format_no_escapes,
        pugi::format_save_file_text,
        pugi::format_write_bom
    };
    
    for (auto flag : flags) {
        std::ostringstream oss4;
        pugi::xml_writer_stream writer4(oss4);
        try {
            new_node.print(writer4, PUGIXML_TEXT("  "), flag, pugi::encoding_auto, 0);
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    return 0;
}