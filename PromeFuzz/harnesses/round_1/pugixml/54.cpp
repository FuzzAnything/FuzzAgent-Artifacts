// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
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
#include <string_view>
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_trim_pcdata);
    
    // If parsing fails, create a simple document for testing
    if (!result) {
        const char* xml_content = "<root><child1 attr1='val1' attr2='val2'/><child2/><child1 attr3='val3'/></root>";
        doc.load_string(xml_content);
    }

    // Get root node
    pugi::xml_node root = doc.root();
    
    // Test empty() function
    bool is_empty = root.empty();
    (void)is_empty; // Avoid unused variable warning
    
    // Test remove_children()
    bool children_removed = root.remove_children();
    (void)children_removed;
    
    // Reload document to have children again
    if (!result) {
        const char* xml_content = "<root><child1 attr1='val1' attr2='val2'/><child2/><child1 attr3='val3'/></root>";
        doc.load_string(xml_content);
    }
    
    // Test remove_attributes()
    bool attrs_removed = root.remove_attributes();
    (void)attrs_removed;
    
    // Reload document to have attributes again
    if (!result) {
        const char* xml_content = "<root><child1 attr1='val1' attr2='val2'/><child2/><child1 attr3='val3'/></root>";
        doc.load_string(xml_content);
    }
    
    // Create string from fuzzer data for remove_child and remove_attribute
    std::string child_name(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(100)));
    std::string attr_name(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(50)));
    
    // Test remove_child(const char_t*) - remove all matching children
    while (root.remove_child(child_name.c_str())) {}
    
    // Test remove_child(string_view_t)
    std::string_view child_sv(child_name);
    // Need to reload document to have children again
    if (!result) {
        const char* xml_content = "<root><child1 attr1='val1' attr2='val2'/><child2/><child1 attr3='val3'/></root>";
        doc.load_string(xml_content);
    }
    while (root.remove_child(child_sv)) {}
    
    // Test remove_attribute(const char_t*)
    // First add an attribute to remove
    root.append_attribute("test_attr") = "test_value";
    bool attr_removed = root.remove_attribute("test_attr");
    (void)attr_removed;
    
    // Also try removing with fuzzer-generated name
    root.remove_attribute(attr_name.c_str());
    
    // Test on child nodes as well
    for (pugi::xml_node child : root.children()) {
        // Test empty() on child
        bool child_empty = child.empty();
        (void)child_empty;
        
        // Test remove_attributes() on child
        child.remove_attributes();
        
        // Test remove_children() on child
        child.remove_children();
        
        // Test remove_child() on child (if it has children)
        for (pugi::xml_node grandchild : child.children()) {
            (void)grandchild;
            // This would normally be called if child had children
        }
        
        // Test remove_attribute() on child
        child.remove_attribute("nonexistent");
    }
    
    // Create a dummy file if needed (not used in this case, but following instructions)
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        fwrite(Data, 1, Size, dummy);
        fclose(dummy);
    }
    
    return 0;
}