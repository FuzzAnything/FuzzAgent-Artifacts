// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

static void test_node_functions(pugi::xml_node node, const uint8_t* data, size_t size) {
    // Test name()
    const char* name_result = node.name();
    (void)name_result;
    
    // Test value()
    const char* value_result = node.value();
    (void)value_result;
    
    // Test type()
    pugi::xml_node_type node_type = node.type();
    (void)node_type;
    
    // Test print() with wide stream
    std::basic_stringstream<wchar_t> wss;
    try {
        node.print(wss, PUGIXML_TEXT("\t"), pugi::format_default, 0);
    } catch (...) {
        // Ignore exceptions from print
    }
    
    // Test set_value() if we have enough data
    if (size > 0) {
        // Create a null-terminated string from fuzzer input
        size_t str_len = size > 100 ? 100 : size;
        std::string str_val(reinterpret_cast<const char*>(data), str_len);
        str_val.push_back('\0');
        
        bool set_value_result = node.set_value(str_val.c_str());
        (void)set_value_result;
    }
    
    // Test set_name() if we have enough data
    if (size > 0) {
        // Create a null-terminated string from fuzzer input
        size_t str_len = size > 100 ? 100 : size;
        std::string str_name(reinterpret_cast<const char*>(data), str_len);
        str_name.push_back('\0');
        
        bool set_name_result = node.set_name(str_name.c_str());
        (void)set_name_result;
    }
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input
    const char* dummy_file = "./dummy_file";
    std::ofstream outfile(dummy_file, std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(Data), Size);
        outfile.close();
    }
    
    // Parse XML from the dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(dummy_file);
    
    if (result) {
        // Test functions on document node
        test_node_functions(doc, Data, Size);
        
        // Test functions on root node
        pugi::xml_node root = doc.document_element();
        if (root) {
            test_node_functions(root, Data, Size);
            
            // Test functions on child nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                test_node_functions(child, Data, Size);
                
                // Test functions on grandchildren
                for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                    test_node_functions(grandchild, Data, Size);
                }
            }
        }
        
        // Also test functions on various node types by traversing the tree
        for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
            test_node_functions(node, Data, Size);
        }
    }
    
    // Test with empty document
    pugi::xml_document empty_doc;
    test_node_functions(empty_doc, Data, Size);
    
    // Test with null node
    pugi::xml_node null_node;
    test_node_functions(null_node, Data, Size);
    
    // Clean up dummy file
    std::remove(dummy_file);
    
    return 0;
}