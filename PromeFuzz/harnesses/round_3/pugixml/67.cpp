// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static pugi::xml_node createTestNode(pugi::xml_document& doc, uint8_t node_type_byte) {
    pugi::xml_node node;
    
    switch (node_type_byte % 9) {
        case 0: node = doc.append_child(pugi::node_element); break;
        case 1: node = doc.append_child(pugi::node_pcdata); break;
        case 2: node = doc.append_child(pugi::node_cdata); break;
        case 3: node = doc.append_child(pugi::node_comment); break;
        case 4: node = doc.append_child(pugi::node_pi); break;
        case 5: node = doc.append_child(pugi::node_declaration); break;
        case 6: node = doc.append_child(pugi::node_doctype); break;
        case 7: node = doc.append_child(pugi::node_document); break;
        case 8: node = doc.append_child(pugi::node_null); break;
    }
    
    return node;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    pugi::xml_document doc;
    
    // Create a test node with type based on first byte
    uint8_t node_type = Data[0];
    pugi::xml_node node = createTestNode(doc, node_type);
    
    // Use remaining data for string operations
    size_t str_size = Size - 1;
    const uint8_t* str_data = Data + 1;
    
    // Ensure null termination for C-string APIs
    std::string cpp_str(reinterpret_cast<const char*>(str_data), str_size);
    const char* c_str = cpp_str.c_str();
    
    // Test set_name
    if (!node.empty()) {
        node.set_name(c_str);
    }
    
    // Test set_value
    if (!node.empty()) {
        node.set_value(c_str);
    }
    
    // Test value() accessor
    if (!node.empty()) {
        const char* node_value = node.value();
        (void)node_value; // Use to avoid unused variable warning
    }
    
    // Test name() accessor
    if (!node.empty()) {
        const char* node_name = node.name();
        (void)node_name; // Use to avoid unused variable warning
    }
    
    // Test type() accessor
    if (!node.empty()) {
        pugi::xml_node_type node_type_val = node.type();
        (void)node_type_val; // Use to avoid unused variable warning
    }
    
    // Test print function
    if (!node.empty()) {
        // Test with string stream
        std::stringstream ss;
        pugi::xml_writer_stream writer(ss);
        
        // Use different encodings based on input
        uint8_t encoding_choice = Data[0] % 12;
        pugi::xml_encoding encoding = pugi::encoding_auto;
        
        switch (encoding_choice) {
            case 0: encoding = pugi::encoding_auto; break;
            case 1: encoding = pugi::encoding_utf8; break;
            case 2: encoding = pugi::encoding_utf16_le; break;
            case 3: encoding = pugi::encoding_utf16_be; break;
            case 4: encoding = pugi::encoding_utf16; break;
            case 5: encoding = pugi::encoding_utf32_le; break;
            case 6: encoding = pugi::encoding_utf32_be; break;
            case 7: encoding = pugi::encoding_utf32; break;
            case 8: encoding = pugi::encoding_wchar; break;
            case 9: encoding = pugi::encoding_latin1; break;
            default: encoding = pugi::encoding_auto; break;
        }
        
        try {
            node.print(writer, "\t", pugi::format_default, encoding);
        } catch (...) {
            // Ignore exceptions during fuzzing
        }
        
        // Also test with file writer
        writeDummyFile(Data, Size);
        FILE* file = fopen("./dummy_file", "wb");
        if (file) {
            pugi::xml_writer_file file_writer(file);
            try {
                node.print(file_writer, "\t", pugi::format_default, encoding);
            } catch (...) {
                // Ignore exceptions during fuzzing
            }
            fclose(file);
        }
    }
    
    // Create another document with different content to test more states
    pugi::xml_document doc2;
    pugi::xml_node root = doc2.append_child("root");
    pugi::xml_node child = root.append_child("child");
    child.append_attribute("attr") = c_str;
    child.text() = c_str;
    
    // Test APIs on different nodes
    child.set_name("renamed");
    child.set_value("new_value");
    
    // Test print on document
    std::stringstream ss2;
    pugi::xml_writer_stream writer2(ss2);
    doc2.print(writer2);
    
    return 0;
}