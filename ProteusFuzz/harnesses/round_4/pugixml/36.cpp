// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
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
#include <pugixml.hpp>

// Helper to get the correct char_t type from pugixml namespace
using pugi::char_t;

static void fuzz_next_sibling(pugi::xml_node node, const uint8_t* data, size_t size) {
    // Overload 1: next_sibling()
    (void)node.next_sibling();
    
    // Overload 2: next_sibling(const char_t*)
    if (size > 0) {
        const char_t* name = reinterpret_cast<const char_t*>(data);
        (void)node.next_sibling(name);
    }
    
    // Overload 3: next_sibling(string_view_t)
    if (size > 0) {
        pugi::string_view_t a_str(reinterpret_cast<const char_t*>(data), size);
        (void)node.next_sibling(a_str);
    }
}

static void fuzz_previous_sibling(pugi::xml_node node, const uint8_t* data, size_t size) {
    // Overload 1: previous_sibling()
    (void)node.previous_sibling();
    
    // Overload 2: previous_sibling(const char_t*)
    if (size > 0) {
        const char_t* name = reinterpret_cast<const char_t*>(data);
        (void)node.previous_sibling(name);
    }
    
    // Overload 3: previous_sibling(string_view_t)
    if (size > 0) {
        pugi::string_view_t a_str(reinterpret_cast<const char_t*>(data), size);
        (void)node.previous_sibling(a_str);
    }
}

static void fuzz_child(pugi::xml_node node, const uint8_t* data, size_t size) {
    // Overload: child(const char_t*)
    if (size > 0) {
        const char_t* name = reinterpret_cast<const char_t*>(data);
        (void)node.child(name);
    }
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare environment: parse XML from input data
    pugi::xml_document doc;
    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    
    // Ensure the input is valid XML by wrapping it if necessary
    // We try to parse directly first
    if (!doc.load_string(xml_str.c_str())) {
        // If parsing fails, try to create a minimal valid XML structure
        // by using a subset of data or a fallback
        std::string fallback = "<root>";
        if (Size > 10) {
            fallback.append(reinterpret_cast<const char*>(Data), 10);
        }
        fallback.append("</root>");
        if (!doc.load_string(fallback.c_str())) {
            // If still failing, return early
            return 0;
        }
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // Try to get the first element child if root is not named "root"
        // Use first_child() and check node type
        pugi::xml_node child = doc.first_child();
        while (child && child.type() != pugi::node_element) {
            child = child.next_sibling();
        }
        root = child;
    }
    if (!root) {
        // Fallback to document node itself if no children
        root = doc;
    }

    // Create a vector of nodes to traverse, starting with root
    std::vector<pugi::xml_node> nodes;
    nodes.push_back(root);
    
    // Add some children if they exist to increase state exploration
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        nodes.push_back(child);
    }

    // Execute the required sequence of API calls
    // We iterate through available nodes to maximize state coverage
    for (const auto& node : nodes) {
        // 1. next_sibling (3 overloads)
        fuzz_next_sibling(node, Data, Size);
        
        // 2. next_sibling (3 overloads)
        fuzz_next_sibling(node, Data, Size);
        
        // 3. previous_sibling (3 overloads)
        fuzz_previous_sibling(node, Data, Size);
        
        // 4. previous_sibling (3 overloads)
        fuzz_previous_sibling(node, Data, Size);
        
        // 5-10. child (6 times)
        for (int i = 0; i < 6; ++i) {
            fuzz_child(node, Data, Size);
        }
        
        // 11-12. next_sibling (2 times)
        fuzz_next_sibling(node, Data, Size);
        fuzz_next_sibling(node, Data, Size);
        
        // 13-14. previous_sibling (2 times)
        fuzz_previous_sibling(node, Data, Size);
        fuzz_previous_sibling(node, Data, Size);
        
        // 15-18. next_sibling (4 times)
        fuzz_next_sibling(node, Data, Size);
        fuzz_next_sibling(node, Data, Size);
        fuzz_next_sibling(node, Data, Size);
        fuzz_next_sibling(node, Data, Size);
        
        // 19-22. previous_sibling (4 times)
        fuzz_previous_sibling(node, Data, Size);
        fuzz_previous_sibling(node, Data, Size);
        fuzz_previous_sibling(node, Data, Size);
        fuzz_previous_sibling(node, Data, Size);
    }

    // Cleanup is handled by RAII (pugi::xml_document destructor)
    return 0;
}