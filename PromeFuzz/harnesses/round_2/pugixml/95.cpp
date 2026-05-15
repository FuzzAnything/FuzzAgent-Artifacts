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
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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

static void test_xml_node_functions(pugi::xml_node node, 
                                   const std::string& str1,
                                   const std::string& str2,
                                   const std::string& str3) {
    // Test next_sibling
    pugi::xml_node next_sib = node.next_sibling(str1.c_str());
    (void)next_sib;
    
    // Test child
    pugi::xml_node child = node.child(str1.c_str());
    (void)child;
    
    // Test find_child_by_attribute - first overload (3 parameters)
    pugi::xml_node found1 = node.find_child_by_attribute(str1.c_str(), str2.c_str(), str3.c_str());
    (void)found1;
    
    // Test find_child_by_attribute - second overload (2 parameters)
    pugi::xml_node found2 = node.find_child_by_attribute(str2.c_str(), str3.c_str());
    (void)found2;
    
    // Test first_element_by_path
    pugi::xml_node path_node = node.first_element_by_path(str1.c_str());
    (void)path_node;
    
    // Test parent
    pugi::xml_node parent = node.parent();
    (void)parent;
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a dummy XML document with some structure
    const char* xml_content = 
        "<?xml version=\"1.0\"?>"
        "<root>"
        "  <child1 attr1=\"value1\">Text1</child1>"
        "  <child2 attr2=\"value2\">Text2</child2>"
        "  <child3 attr1=\"value3\" attr2=\"value4\">"
        "    <grandchild>Grandchild text</grandchild>"
        "  </child3>"
        "</root>";
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content);
    
    if (!result) {
        return 0;
    }
    
    // Get the root node
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }
    
    // Extract strings from fuzzer input
    std::vector<std::string> strings;
    const uint8_t* current = Data;
    const uint8_t* end = Data + Size;
    
    while (current < end) {
        const uint8_t* start = current;
        while (current < end && *current != '\0') {
            ++current;
        }
        
        // Create string from this segment
        std::string str(reinterpret_cast<const char*>(start), current - start);
        strings.push_back(str);
        
        // Skip the null terminator
        if (current < end) {
            ++current;
        }
    }
    
    // Ensure we have at least 3 strings (pad with empty strings if needed)
    while (strings.size() < 3) {
        strings.push_back("");
    }
    
    // Test functions on root node
    test_xml_node_functions(root, strings[0], strings[1], strings[2]);
    
    // Test functions on child nodes
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        test_xml_node_functions(child, strings[0], strings[1], strings[2]);
        
        // Test functions on grandchildren
        for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
            test_xml_node_functions(grandchild, strings[0], strings[1], strings[2]);
        }
    }
    
    // Test with empty node
    pugi::xml_node empty_node;
    test_xml_node_functions(empty_node, strings[0], strings[1], strings[2]);
    
    return 0;
}