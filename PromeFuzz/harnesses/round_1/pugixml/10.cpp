// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
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
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Create XML document and parse some data if possible
    pugi::xml_document doc;
    
    // Try to parse the input as XML, but continue even if it fails
    doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Get the root node (or document node if parsing failed)
    pugi::xml_node node = doc.document_element();
    
    // If no document element, use the document node itself
    if (!node) {
        node = doc;
    }
    
    // Create some string names for testing
    const char* name1 = "test";
    const char* name2 = "node";
    const char* name3 = "child";
    
    std::string_view sv1 = "test";
    std::string_view sv2 = "node";
    std::string_view sv3 = "child";
    
    // Ensure null-terminated strings for char* parameters
    char safe_name1[5] = {'t', 'e', 's', 't', '\0'};
    char safe_name2[5] = {'n', 'o', 'd', 'e', '\0'};
    char safe_name3[6] = {'c', 'h', 'i', 'l', 'd', '\0'};
    
    // Call the functions in the specified order
    // 1. next_sibling (no arg)
    pugi::xml_node result1 = node.next_sibling();
    
    // 2. next_sibling (const char*)
    pugi::xml_node result2 = node.next_sibling(safe_name1);
    
    // 3. next_sibling (string_view)
    pugi::xml_node result3 = node.next_sibling(sv1);
    
    // 4. previous_sibling (no arg)
    pugi::xml_node result4 = node.previous_sibling();
    
    // 5. previous_sibling (const char*)
    pugi::xml_node result5 = node.previous_sibling(safe_name2);
    
    // 6. previous_sibling (string_view)
    pugi::xml_node result6 = node.previous_sibling(sv2);
    
    // 7-12. child (const char*) - 6 times as specified
    pugi::xml_node result7 = node.child(safe_name3);
    pugi::xml_node result8 = node.child(safe_name1);
    pugi::xml_node result9 = node.child(safe_name2);
    pugi::xml_node result10 = node.child(safe_name3);
    pugi::xml_node result11 = node.child(safe_name1);
    pugi::xml_node result12 = node.child(safe_name2);
    
    // 13. next_sibling (no arg) - on one of the child results
    if (result7) {
        pugi::xml_node result13 = result7.next_sibling();
    }
    
    // 14. next_sibling (const char*)
    if (result8) {
        pugi::xml_node result14 = result8.next_sibling(safe_name3);
    }
    
    // 15. previous_sibling (no arg)
    if (result9) {
        pugi::xml_node result15 = result9.previous_sibling();
    }
    
    // 16. previous_sibling (string_view)
    if (result10) {
        pugi::xml_node result16 = result10.previous_sibling(sv3);
    }
    
    // 17-20. next_sibling (4 times) - mix of overloads
    if (result11) {
        pugi::xml_node result17 = result11.next_sibling();
        pugi::xml_node result18 = result11.next_sibling(safe_name2);
        pugi::xml_node result19 = result11.next_sibling(sv1);
        pugi::xml_node result20 = result11.next_sibling();
    }
    
    // 21-24. previous_sibling (4 times) - mix of overloads
    if (result12) {
        pugi::xml_node result21 = result12.previous_sibling();
        pugi::xml_node result22 = result12.previous_sibling(safe_name3);
        pugi::xml_node result23 = result12.previous_sibling(sv2);
        pugi::xml_node result24 = result12.previous_sibling();
    }
    
    // Try calling the functions on different nodes to explore more states
    pugi::xml_node another_node = doc.first_child();
    if (another_node) {
        another_node.next_sibling();
        another_node.next_sibling(safe_name1);
        another_node.next_sibling(sv1);
        another_node.previous_sibling();
        another_node.previous_sibling(safe_name2);
        another_node.previous_sibling(sv2);
        another_node.child(safe_name3);
    }
    
    // Also test on empty nodes
    pugi::xml_node empty_node;
    empty_node.next_sibling();
    empty_node.next_sibling(safe_name1);
    empty_node.next_sibling(sv1);
    empty_node.previous_sibling();
    empty_node.previous_sibling(safe_name2);
    empty_node.previous_sibling(sv2);
    empty_node.child(safe_name3);
    
    return 0;
}