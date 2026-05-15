// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>

static void test_functions(pugi::xml_node node, const std::vector<std::string>& strings) {
    // Test find_child_by_attribute
    if (strings.size() >= 3) {
        node.find_child_by_attribute(strings[0].c_str(), strings[1].c_str(), strings[2].c_str());
    }
    
    // Test parent
    node.parent();
    
    // Test next_sibling overloads
    node.next_sibling();
    if (!strings.empty()) {
        node.next_sibling(strings[0].c_str());
    }
    
    // Test child
    if (!strings.empty()) {
        pugi::string_view_t sv(strings[0].c_str(), strings[0].size());
        node.child(sv);
    }
    
    // Test first_element_by_path
    if (!strings.empty()) {
        node.first_element_by_path(strings[0].c_str());
        if (strings.size() >= 2 && !strings[1].empty()) {
            node.first_element_by_path(strings[0].c_str(), strings[1][0]);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Parse XML from input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        // If parsing fails, try with a dummy file
        const char* filename = "./dummy_file";
        FILE* f = fopen(filename, "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
            doc.load_file(filename);
            remove(filename);
        }
    }
    
    // Extract strings from input data for function arguments
    std::vector<std::string> strings;
    const char* start = reinterpret_cast<const char*>(Data);
    const char* end = start + Size;
    const char* current = start;
    
    while (current < end) {
        const char* null_pos = static_cast<const char*>(memchr(current, '\0', end - current));
        if (null_pos) {
            strings.emplace_back(current, null_pos - current);
            current = null_pos + 1;
        } else {
            strings.emplace_back(current, end - current);
            break;
        }
    }
    
    // Test functions on document root and traverse children
    pugi::xml_node root = doc.document_element();
    if (root) {
        test_functions(root, strings);
        
        // Traverse some children and test functions on them
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            test_functions(child, strings);
            
            // Test on grandchildren too
            for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                test_functions(grandchild, strings);
            }
        }
    }
    
    // Also test on empty nodes
    pugi::xml_node empty_node;
    test_functions(empty_node, strings);
    
    // Test on nodes created from the document
    test_functions(doc, strings);
    
    return 0;
}