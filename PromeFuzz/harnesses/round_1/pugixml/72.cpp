// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
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
#include <string>
#include <fstream>

// Static helper functions to avoid name conflicts
static bool predicate_always_true(const pugi::xml_node&) {
    return true;
}

static bool predicate_always_false(const pugi::xml_node&) {
    return false;
}

static bool predicate_has_attributes(const pugi::xml_node& node) {
    return node.first_attribute();
}

static bool predicate_name_contains_a(const pugi::xml_node& node) {
    const char* name = node.name();
    if (!name) return false;
    return std::string(name).find('a') != std::string::npos;
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    // Try to parse the input as XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // If parsing fails, create a minimal valid XML structure for testing
    if (!result) {
        const char* default_xml = "<root><child1 attr='1'/><child2>text</child2><child3/><nested><leaf/></nested></root>";
        doc.load_string(default_xml);
    }
    
    // Get the root node
    pugi::xml_node root = doc.root();
    if (!root) {
        // If no root, create one
        root = doc.append_child("root");
        root.append_child("child1");
        root.append_child("child2").append_attribute("attr") = "value";
    }
    
    // Test 1: find_child with various predicates
    root.find_child(predicate_always_true);
    root.find_child(predicate_always_false);
    root.find_child(predicate_has_attributes);
    root.find_child(predicate_name_contains_a);
    
    // Test 2: first_child
    pugi::xml_node first = root.first_child();
    if (first) {
        // Test on child node as well
        first.first_child();
    }
    
    // Test 3: child() with name parameter
    // Extract a potential node name from fuzzer input
    std::string node_name;
    if (Size > 0) {
        size_t name_len = Size > 100 ? 100 : Size;
        node_name.assign(reinterpret_cast<const char*>(Data), name_len);
        // Ensure null termination for C-string usage
        if (!node_name.empty() && node_name.back() != '\0') {
            node_name.push_back('\0');
        }
    } else {
        node_name = "child";
    }
    
    // Test with std::string_view
    std::string_view name_view(node_name.c_str(), node_name.length());
    root.child(name_view);
    
    // Test with C-string (ensure null-terminated)
    root.child(node_name.c_str());
    
    // Test 4: find_node with various predicates
    root.find_node(predicate_always_true);
    root.find_node(predicate_always_false);
    root.find_node(predicate_has_attributes);
    root.find_node(predicate_name_contains_a);
    
    // Test 5: previous_sibling
    if (first) {
        first.previous_sibling();
        // Test with name parameter
        first.previous_sibling(name_view);
        first.previous_sibling(node_name.c_str());
        
        // Test on a deeper node
        pugi::xml_node deep = root.first_child().first_child();
        if (deep) {
            deep.previous_sibling();
        }
    }
    
    // Test 6: last_child
    root.last_child();
    if (first) {
        first.last_child();
    }
    
    // Additional tests: chain operations and test on different nodes
    pugi::xml_node current = root;
    for (int i = 0; i < 5 && current; ++i) {
        current = current.first_child();
        if (current) {
            current.last_child();
            current.find_child(predicate_has_attributes);
            current.child("test");
        }
    }
    
    // Test with a file if needed (though we're using buffer parsing)
    // Write dummy data to file and try to parse it
    std::ofstream out_file("./dummy_file", std::ios::binary);
    if (out_file) {
        if (Size > 0) {
            out_file.write(reinterpret_cast<const char*>(Data), Size);
        } else {
            out_file << "<root><test/></root>";
        }
        out_file.close();
        
        pugi::xml_document file_doc;
        file_doc.load_file("./dummy_file");
        pugi::xml_node file_root = file_doc.root();
        if (file_root) {
            file_root.first_child();
            file_root.last_child();
        }
    }
    
    // Cleanup
    remove("./dummy_file");
    
    return 0;
}