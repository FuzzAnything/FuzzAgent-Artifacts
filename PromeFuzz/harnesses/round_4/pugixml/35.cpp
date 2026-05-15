// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Create null-terminated string from fuzzer input
    std::string xml_data(reinterpret_cast<const char*>(Data), Size);
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer_inplace(xml_data.data(), xml_data.size());
    
    if (!result) {
        // If parsing fails, we can still try with empty document
        // or return early. Let's continue with empty document.
        doc.reset();
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare names for child/attribute lookups
    // We'll use fixed names to ensure we have null-terminated strings
    const char* child_name1 = "child1";
    const char* child_name2 = "child2";
    const char* attr_name1 = "attr1";
    const char* attr_name2 = "attr2";
    
    // First call: pugi::xml_node::child
    pugi::xml_node node1 = root.child(child_name1);
    
    // Second call: pugi::xml_node::attribute
    pugi::xml_attribute attr1 = node1.attribute(attr_name1);
    
    // Third call: pugi::xml_node::child
    pugi::xml_node node2 = node1.child(child_name2);
    
    // Fourth call: pugi::xml_node::attribute
    pugi::xml_attribute attr2 = node2.attribute(attr_name2);
    
    // Fifth call: pugi::xml_attribute::next_attribute
    pugi::xml_attribute attr3 = attr2.next_attribute();
    
    // Sixth call: pugi::xml_attribute::next_attribute
    pugi::xml_attribute attr4 = attr3.next_attribute();
    
    // Seventh call: pugi::xml_attribute::previous_attribute
    pugi::xml_attribute attr5 = attr4.previous_attribute();
    
    // Eighth call: pugi::xml_attribute::previous_attribute
    pugi::xml_attribute attr6 = attr5.previous_attribute();
    
    // Ninth call: pugi::xml_attribute::next_attribute
    pugi::xml_attribute attr7 = attr6.next_attribute();
    
    // Tenth call: pugi::xml_attribute::previous_attribute
    pugi::xml_attribute attr8 = attr7.previous_attribute();
    
    // Also test with string_view versions if available
    // (though not in the required sequence, but to explore more states)
    std::string_view child_sv1(child_name1);
    std::string_view child_sv2(child_name2);
    std::string_view attr_sv1(attr_name1);
    std::string_view attr_sv2(attr_name2);
    
    // Try alternative calls with string_view
    static auto test_string_view = [](pugi::xml_node node, std::string_view name) {
        // Convert string_view to null-terminated string for compatibility
        std::string temp_name(name);
        return node.child(temp_name.c_str());
    };
    
    // Test with different nodes from the document
    if (root) {
        test_string_view(root, child_sv1);
        test_string_view(root, child_sv2);
    }
    
    // Test attribute with string_view
    if (node1) {
        std::string temp_attr(attr_sv1);
        node1.attribute(temp_attr.c_str());
    }
    
    // Try to traverse all children and attributes to explore more states
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        // Call child() with various names
        child.child(child_name1);
        child.child(child_name2);
        
        // Call attribute() with various names
        child.attribute(attr_name1);
        child.attribute(attr_name2);
        
        // Traverse all attributes
        for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
            // Call next_attribute and previous_attribute in various sequences
            attr.next_attribute();
            attr.previous_attribute();
        }
    }
    
    return 0;
}