// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: parse the input data as XML
    pugi::xml_document doc;
    
    // Convert input data to std::string for parsing
    // Ensure null termination for safety
    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    // Try to load the string. If it fails, the document will be empty,
    // but we proceed to test API robustness on invalid/empty nodes.
    doc.load_string(input.c_str());

    // Start with the root node. 
    // xml_document::child_node() does not exist. We use child() with an empty name
    // to get the first child, or just use the document itself if we want to treat it as a node.
    // However, usually we want the root element. Let's try to get the first child.
    // If the input is just text or empty, root will be null.
    // Correct way: doc.child() with a name, or iterate. 
    // To get the root element, we can use doc.child("") which returns the first child node.
    // Or simply use doc as the node if it has a root element.
    // Let's use doc.child("") to get the first child node (the root element).
    pugi::xml_node root = doc.child("");

    // If no root found (e.g., input was not a valid XML document with a root element),
    // we can still test on the document node itself or a null node.
    // To ensure we have a node to operate on for the sequence, we check validity.
    // If root is null, we might want to try to create a dummy structure or just proceed with null checks.
    // The prompt asks to invoke functions in order. We will proceed with the node we have (even if null).
    
    // If the document is empty, root is null. The API calls on null nodes should be safe (return empty handles).
    
    // 1. pugi::xml_node::attribute
    std::string_view attr_name1 = "id";
    pugi::xml_attribute attr1 = root.attribute(attr_name1);

    // 2. pugi::xml_node::child
    std::string_view child_name = "item";
    pugi::xml_node child_node = root.child(child_name);

    // 3. pugi::xml_node::attribute (on child_node)
    std::string_view attr_name2 = "type";
    pugi::xml_attribute attr2 = child_node.attribute(attr_name2);

    // 4. pugi::xml_node::attribute (on child_node again)
    std::string_view attr_name3 = "value";
    pugi::xml_attribute attr3 = child_node.attribute(attr_name3);

    // 5. pugi::xml_node::attribute (on child_node again)
    std::string_view attr_name4 = "name";
    pugi::xml_attribute attr4 = child_node.attribute(attr_name4);

    // 6. pugi::xml_node::last_attribute (on child_node)
    pugi::xml_attribute last_attr = child_node.last_attribute();

    // 7. pugi::xml_node::attribute (on child_node again)
    std::string_view attr_name5 = "class";
    pugi::xml_attribute attr5 = child_node.attribute(attr_name5);

    // 8. pugi::xml_node::attribute (on child_node again)
    std::string_view attr_name6 = "style";
    pugi::xml_attribute attr6 = child_node.attribute(attr_name6);

    // Cleanup is handled automatically by RAII (xml_document destructor)
    // No explicit cleanup needed for attributes or nodes as they are handles.

    return 0;
}