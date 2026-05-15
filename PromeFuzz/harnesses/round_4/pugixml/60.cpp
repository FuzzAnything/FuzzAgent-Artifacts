// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
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
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

static pugi::xml_node create_test_node(pugi::xml_node_type node_type) {
    static pugi::xml_document doc;
    
    switch (node_type) {
        case pugi::node_element:
            return doc.append_child(pugi::node_element);
        case pugi::node_pcdata:
            return doc.append_child(pugi::node_pcdata);
        case pugi::node_cdata:
            return doc.append_child(pugi::node_cdata);
        case pugi::node_comment:
            return doc.append_child(pugi::node_comment);
        case pugi::node_pi:
            return doc.append_child(pugi::node_pi);
        case pugi::node_declaration:
            return doc.append_child(pugi::node_declaration);
        case pugi::node_doctype:
            return doc.append_child(pugi::node_doctype);
        case pugi::node_document:
            return doc;
        default:
            return pugi::xml_node();
    }
}

static void write_to_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Prepare test data
    std::string_view test_data(reinterpret_cast<const char*>(Data), Size);
    write_to_dummy_file(Data, Size);
    
    // Test with different node types
    pugi::xml_node_type node_types[] = {
        pugi::node_element,
        pugi::node_pcdata,
        pugi::node_cdata,
        pugi::node_comment,
        pugi::node_pi,
        pugi::node_declaration,
        pugi::node_doctype,
        pugi::node_document,
        pugi::node_null
    };
    
    for (auto node_type : node_types) {
        pugi::xml_node node = create_test_node(node_type);
        
        // Test set_value
        if (test_data.size() > 0) {
            // Use null-terminated version
            std::string null_term_str(test_data);
            node.set_value(null_term_str.c_str());
            
            // Test with explicit size version
            node.set_value(test_data.data(), test_data.size());
        }
        
        // Test type()
        pugi::xml_node_type retrieved_type = node.type();
        (void)retrieved_type;
        
        // Test name()
        const char* node_name = node.name();
        (void)node_name;
        
        // Test value()
        const char* node_value = node.value();
        (void)node_value;
        
        // Test set_name
        if (test_data.size() > 0) {
            std::string name_str(test_data.substr(0, std::min(test_data.size(), size_t(100))));
            node.set_name(name_str.c_str());
        }
        
        // Test print() with different configurations
        try {
            // Test with string stream writer
            std::stringstream ss;
            pugi::xml_writer_stream writer(ss);
            node.print(writer, "\t", pugi::format_default, pugi::encoding_auto, 0);
            
            // Test with different encodings
            pugi::xml_encoding encodings[] = {
                pugi::encoding_auto,
                pugi::encoding_utf8,
                pugi::encoding_latin1
            };
            
            for (auto encoding : encodings) {
                std::stringstream ss2;
                pugi::xml_writer_stream writer2(ss2);
                node.print(writer2, "  ", pugi::format_raw, encoding, 0);
            }
            
            // Test with file writer
            FILE* file = fopen("./dummy_file", "r");
            if (file) {
                pugi::xml_writer_file file_writer(file);
                node.print(file_writer, "", pugi::format_indent, pugi::encoding_utf8, 0);
                fclose(file);
            }
        } catch (...) {
            // Ignore exceptions during fuzzing
        }
        
        // Clean up for next iteration
        if (node_type != pugi::node_document && node_type != pugi::node_null) {
            node.parent().remove_child(node);
        }
    }
    
    // Additional test: parse XML from fuzz data and test nodes
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    
    if (result) {
        // Traverse all nodes and test APIs
        pugi::xml_node root = doc.document_element();
        if (root) {
            for (pugi::xml_node node = root; node; node = node.next_sibling()) {
                // Test all functions on parsed nodes
                node.type();
                node.name();
                node.value();
                
                if (test_data.size() > 0) {
                    std::string test_str(test_data.substr(0, std::min(test_data.size(), size_t(50))));
                    node.set_value(test_str.c_str());
                    node.set_name(test_str.c_str());
                }
                
                // Test print on parsed nodes
                std::stringstream ss;
                pugi::xml_writer_stream writer(ss);
                node.print(writer, "\t", pugi::format_default, pugi::encoding_auto, 0);
            }
        }
    }
    
    return 0;
}