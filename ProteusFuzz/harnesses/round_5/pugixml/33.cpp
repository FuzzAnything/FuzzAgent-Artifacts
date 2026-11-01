// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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

static bool parse_input_to_document(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return doc.load_string("<root/>");
    }
    
    // Ensure null termination for safety
    std::string input(reinterpret_cast<const char*>(data), size);
    // Append a simple root if the input is not valid XML or empty
    if (input.empty()) {
        return doc.load_string("<root/>");
    }
    
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    if (!result) {
        // Fallback to a valid document if parsing fails
        return doc.load_string("<root/>");
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* Data, size_t Size) {
    pugi::xml_document doc;
    
    // Step 0: Prepare the environment
    if (!parse_input_to_document(Data, Size, doc)) {
        return 0;
    }

    // Ensure we have a root to work with
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::child
    // Find a child. If not found, we get an empty node, which is valid for testing.
    pugi::xml_node target_node = root.child("child");
    
    // If the child doesn't exist, create one to ensure subsequent calls operate on a valid node
    if (!target_node) {
        target_node = root.append_child("child");
        if (!target_node) {
            return 0;
        }
    }

    // 2. pugi::xml_node::append_child (First call)
    pugi::xml_node node1 = target_node.append_child("node1");
    if (!node1) {
        // Allocation failure or invalid state, stop here
        return 0;
    }

    // 3. pugi::xml_node::append_child (Second call)
    pugi::xml_node node2 = target_node.append_child("node2");
    if (!node2) {
        return 0;
    }

    // 4. pugi::xml_text::set (First overload: long)
    // We need a text node. append_child("node1") creates an element. 
    // We can get the text node of node1 or create a text node.
    // The API pugi::xml_text::set is called on an xml_text object.
    // pugi::xml_node::text() returns an xml_text object.
    pugi::xml_text text1 = node1.text();
    // Invoke set(long)
    text1.set(12345L);

    // 5. pugi::xml_text::set (Second overload: unsigned long)
    // Invoke set(unsigned long)
    text1.set(67890UL);

    // 6. pugi::xml_node::append_child (Third call)
    pugi::xml_node node3 = target_node.append_child("node3");
    if (!node3) {
        return 0;
    }

    // 7. pugi::xml_node::append_child (Fourth call)
    pugi::xml_node node4 = target_node.append_child("node4");
    if (!node4) {
        return 0;
    }

    // 8. pugi::xml_text::set (First overload: long) on a different node
    pugi::xml_text text2 = node3.text();
    text2.set(-999L);

    // 9. pugi::xml_text::set (Second overload: unsigned long) on a different node
    pugi::xml_text text3 = node4.text();
    text3.set(0xFFFFFFFFUL);

    // Cleanup is handled by the destructor of xml_document when it goes out of scope.
    // No explicit cleanup needed for this fuzz driver.

    return 0;
}