// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Prepare environment: Create a document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    if (!root) return 0;

    // Create a child node to test child() and attribute operations
    pugi::xml_node childNode = root.append_child("child");
    if (!childNode) return 0;

    // 1. pugi::xml_node::child
    // The input buffer is not guaranteed to be null-terminated.
    // We must create a null-terminated string from the input to pass to child().
    // We limit the length to avoid excessive allocation and ensure safety.
    size_t safe_len = (Size < 256) ? Size : 256;
    
    // Create a temporary buffer for the name
    char name_buf[257];
    std::memcpy(name_buf, Data, safe_len);
    name_buf[safe_len] = '\0';

    pugi::xml_node foundNode = root.child(name_buf);

    // If child() returns an empty node (name not found), fallback to the explicitly created childNode
    if (!foundNode) {
        foundNode = childNode;
    }

    // 2. pugi::xml_node::append_attribute (First call)
    pugi::xml_attribute attr1 = foundNode.append_attribute("attr1");
    if (!attr1) return 0;

    // 3. pugi::xml_node::append_attribute (Second call)
    pugi::xml_attribute attr2 = foundNode.append_attribute("attr2");
    if (!attr2) return 0;

    // 4. pugi::xml_attribute::set_value (Overload 1: long)
    // Use input data to generate a long value
    long valLong = 0;
    if (Size >= 4) {
        valLong = static_cast<long>(Data[0]) | (static_cast<long>(Data[1]) << 8) | 
                  (static_cast<long>(Data[2]) << 16) | (static_cast<long>(Data[3]) << 24);
    }
    attr2.set_value(valLong);

    // 5. pugi::xml_node::append_attribute (Third call)
    pugi::xml_attribute attr3 = foundNode.append_attribute("attr3");
    if (!attr3) return 0;

    // 6. pugi::xml_node::append_attribute (Fourth call)
    pugi::xml_attribute attr4 = foundNode.append_attribute("attr4");
    if (!attr4) return 0;

    // 7. pugi::xml_attribute::set_value (Overload 2: unsigned long)
    // Use input data to generate an unsigned long value
    unsigned long valULong = 0;
    if (Size >= 4) {
        valULong = static_cast<unsigned long>(Data[0]) | (static_cast<unsigned long>(Data[1]) << 8) | 
                   (static_cast<unsigned long>(Data[2]) << 16) | (static_cast<unsigned long>(Data[3]) << 24);
    }
    attr4.set_value(valULong);

    // Cleanup is handled automatically by pugi::xml_document destructor
    return 0;
}