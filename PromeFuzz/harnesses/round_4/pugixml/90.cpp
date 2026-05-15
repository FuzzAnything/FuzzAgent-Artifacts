// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Only proceed if parsing succeeded (at least partially)
    if (result) {
        pugi::xml_node root = doc.root();
        
        // Test 1: find_child with various predicates
        // Predicate that always returns true
        static auto always_true = [](pugi::xml_node) { return true; };
        root.find_child(always_true);
        
        // Predicate that checks if node has children
        static auto has_children = [](pugi::xml_node node) { return !node.empty(); };
        root.find_child(has_children);
        
        // Predicate that checks attribute value (example from description)
        static auto attr_less_than_20 = [](pugi::xml_node node) {
            for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
                if (attr.as_int() < 20) return true;
            }
            return false;
        };
        root.find_child(attr_less_than_20);
        
        // Test 2: find_node with various predicates
        root.find_node(always_true);
        root.find_node(has_children);
        
        // Predicate that checks node name
        if (Size > 0) {
            // Create null-terminated string from fuzzer input for name comparison
            char name[101];
            size_t name_len = Size > 100 ? 100 : Size;
            memcpy(name, Data, name_len);
            name[name_len] = '\0';
            
            static auto has_specific_name = [](pugi::xml_node node, const char* target_name) {
                return std::string_view(node.name()) == target_name;
            };
            root.find_node([&name](pugi::xml_node node) { return has_specific_name(node, name); });
        }
        
        // Test 3: first_child
        pugi::xml_node first = root.first_child();
        
        // Test 4: previous_sibling
        if (!first.empty()) {
            first.previous_sibling();
            
            // Also test on a deeper node
            pugi::xml_node second = first.next_sibling();
            if (!second.empty()) {
                second.previous_sibling();
            }
        }
        
        // Test 5: child with name
        if (Size > 0) {
            // Create null-terminated string from fuzzer input
            char child_name[101];
            size_t child_name_len = Size > 100 ? 100 : Size;
            memcpy(child_name, Data, child_name_len);
            child_name[child_name_len] = '\0';
            
            root.child(child_name);
            
            // Also test on child nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                child.child(child_name);
            }
        }
        
        // Test 6: last_child
        root.last_child();
        
        // Also test on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            child.last_child();
        }
    }
    
    // Additional tests with empty/default constructed nodes
    pugi::xml_node empty_node;
    empty_node.first_child();
    empty_node.last_child();
    empty_node.previous_sibling();
    empty_node.child("");
    
    static auto always_false = [](pugi::xml_node) { return false; };
    empty_node.find_child(always_false);
    empty_node.find_node(always_false);
    
    return 0;
}