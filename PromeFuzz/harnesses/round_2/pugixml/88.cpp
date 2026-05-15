// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <string>
#include <vector>

static void test_functions(pugi::xml_node node, const std::string& name_str, 
                          const std::string& attr_name_str, const std::string& attr_value_str,
                          const std::string& path_str) {
    // Test next_sibling
    if (!name_str.empty()) {
        node.next_sibling(name_str.c_str());
    }
    
    // Test child
    if (!name_str.empty()) {
        node.child(name_str.c_str());
    }
    
    // Test find_child_by_attribute (both overloads)
    if (!attr_name_str.empty() && !attr_value_str.empty()) {
        if (!name_str.empty()) {
            node.find_child_by_attribute(name_str.c_str(), attr_name_str.c_str(), attr_value_str.c_str());
        }
        node.find_child_by_attribute(attr_name_str.c_str(), attr_value_str.c_str());
    }
    
    // Test first_element_by_path
    if (!path_str.empty()) {
        node.first_element_by_path(path_str.c_str(), '/');
        // Test with different delimiter
        node.first_element_by_path(path_str.c_str(), '.');
    }
    
    // Test parent
    node.parent();
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, create a simple document structure for testing
    if (!result) {
        static const char* dummy_xml = 
            "<root>"
            "<child1 attr1=\"value1\">text1</child1>"
            "<child2 attr2=\"value2\">text2</child2>"
            "<child3 attr3=\"value3\" attr4=\"value4\">text3</child3>"
            "<nested><inner>inner text</inner></nested>"
            "</root>";
        doc.load_string(dummy_xml);
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Extract strings from fuzzer input for function parameters
    // We'll split the input into 4 parts for different parameters
    size_t quarter = Size / 4;
    
    std::string name_str, attr_name_str, attr_value_str, path_str;
    
    // Ensure null termination for each string
    if (quarter > 0) {
        name_str = std::string(reinterpret_cast<const char*>(Data), 
                              std::min(quarter, Size));
        if (quarter * 2 <= Size) {
            attr_name_str = std::string(reinterpret_cast<const char*>(Data + quarter), 
                                       std::min(quarter, Size - quarter));
        }
        if (quarter * 3 <= Size) {
            attr_value_str = std::string(reinterpret_cast<const char*>(Data + quarter * 2), 
                                        std::min(quarter, Size - quarter * 2));
        }
        if (quarter * 4 <= Size) {
            path_str = std::string(reinterpret_cast<const char*>(Data + quarter * 3), 
                                  Size - quarter * 3);
        }
    }
    
    // Test functions on root node
    test_functions(root, name_str, attr_name_str, attr_value_str, path_str);
    
    // Test functions on child nodes
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        test_functions(child, name_str, attr_name_str, attr_value_str, path_str);
        
        // Also test on grandchildren
        for (pugi::xml_node grandchild = child.first_child(); grandchild; 
             grandchild = grandchild.next_sibling()) {
            test_functions(grandchild, name_str, attr_name_str, attr_value_str, path_str);
        }
    }
    
    // Test with empty node
    pugi::xml_node empty_node;
    test_functions(empty_node, name_str, attr_name_str, attr_value_str, path_str);
    
    // Test with document node
    pugi::xml_node doc_node = doc;
    test_functions(doc_node, name_str, attr_name_str, attr_value_str, path_str);
    
    return 0;
}