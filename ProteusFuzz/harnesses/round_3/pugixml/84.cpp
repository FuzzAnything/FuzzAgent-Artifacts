// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

static bool predicate_name(const pugi::xml_node& node) {
    if (node.type() != pugi::node_element) return false;
    return !node.name()[0]; // Simple predicate: match nodes with empty name (rare)
}

static bool predicate_any(const pugi::xml_node& node) {
    return node.type() == pugi::node_element;
}

static bool predicate_attribute(const pugi::xml_node& node) {
    return node.attribute("test").as_int(0) > 0;
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a string from the input data, ensuring it is null-terminated if possible
    // We try to parse it as XML. If it's not valid XML, we handle the error.
    std::string xml_data(reinterpret_cast<const char*>(Data), Size);
    
    // Ensure null termination for safety, though pugi can handle non-null-terminated if size is known
    // However, pugi::xml_document::load_string expects a null-terminated string or uses size.
    // We will use load_string with the raw data.
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());
    
    // Even if parsing fails partially, we might have a valid document structure to test
    // or an empty document. We proceed to test the APIs on whatever structure exists.
    
    pugi::xml_node root = doc;
    
    // Test 1: parent() on root (should be null)
    pugi::xml_node parent = root.parent();
    (void)parent; // Use to avoid unused variable warning
    
    // Test 2: first_child() on root
    pugi::xml_node first = root.first_child();
    
    // Test 3: last_child() on root
    pugi::xml_node last = root.last_child();
    
    // Test 4: child() with a name derived from input or static
    // We use a safe string_view_t approach. 
    // If input has a null byte, we can split, otherwise use a substring.
    std::string_view search_name;
    if (Size > 2) {
        // Take a small chunk as the name to search for
        search_name = std::string_view(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(10)));
    } else {
        search_name = std::string_view("test");
    }
    
    pugi::xml_node child = root.child(search_name);
    
    // Test 5: find_child with a predicate
    pugi::xml_node found_child = root.find_child(predicate_name);
    
    // Test 6: find_node with a predicate (depth-first)
    pugi::xml_node found_node = root.find_node(predicate_any);
    
    // Additional exploration: traverse children if they exist
    for (pugi::xml_node node = first; !node.empty(); node = node.next_sibling()) {
        // Test parent on child
        pugi::xml_node child_parent = node.parent();
        
        // Test first_child on child
        pugi::xml_node child_first = node.first_child();
        
        // Test last_child on child
        pugi::xml_node child_last = node.last_child();
        
        // Test child on child
        pugi::xml_node child_of_child = node.child(search_name);
        
        // Test find_child on child
        pugi::xml_node found_in_child = node.find_child(predicate_attribute);
        
        // Test find_node on child
        pugi::xml_node found_in_subtree = node.find_node(predicate_any);
    }
    
    // Test on an empty node (invalid) to ensure safety
    pugi::xml_node empty_node;
    (void)empty_node.first_child();
    (void)empty_node.last_child();
    (void)empty_node.child(search_name);
    (void)empty_node.parent();
    (void)empty_node.find_child(predicate_name);
    (void)empty_node.find_node(predicate_any);
    
    return 0;
}