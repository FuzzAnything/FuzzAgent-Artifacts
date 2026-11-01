// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
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

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    // Treat input as XML string, handling potential null terminators
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Ensure null termination for safety, though parse_string handles it
    if (!xml_str.empty() && xml_str.back() != '\0') {
        xml_str += '\0';
    }
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    (void)result; // Ignore result for fuzzing, just want the structure
    return doc;
}

static pugi::xml_tree_walker* create_walker(const uint8_t* data, size_t size) {
    // Simple walker implementation for testing begin/end
    struct TestWalker : public pugi::xml_tree_walker {
        bool begin(pugi::xml_node& node) override {
            // Randomly decide based on input data to skip or not
            // Use node name length or value to make decision deterministic per input
            if (node.name() && node.name()[0]) {
                return (node.name()[0] % 2 == 0);
            }
            return true;
        }
        bool end(pugi::xml_node& node) override {
            if (node.name() && node.name()[0]) {
                return (node.name()[1] % 2 == 0);
            }
            return true;
        }
        bool for_each(pugi::xml_node& node) override {
            // Required pure virtual method implementation
            // Just return true to continue traversal
            return true;
        }
    };
    return new TestWalker();
}

extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc = create_document_from_input(Data, Size);
    
    // Test xml_node::root
    pugi::xml_node root_node = doc.root();
    if (root_node) {
        pugi::xml_node r = root_node.root();
        (void)r; // Ensure call is made
    }

    // Test xml_node::next_sibling (both overloads)
    pugi::xml_node current = doc.first_child();
    while (current) {
        // Overload 1: next_sibling()
        pugi::xml_node next = current.next_sibling();
        (void)next;
        
        // Overload 2: next_sibling(name)
        std::string_view name_view = "test";
        pugi::xml_node next_named = current.next_sibling(name_view);
        (void)next_named;
        
        current = current.next_sibling();
    }

    // Test xml_node::parent
    current = doc.first_child();
    while (current) {
        pugi::xml_node parent = current.parent();
        (void)parent;
        current = current.next_sibling();
    }

    // Test xml_tree_walker::begin and xml_tree_walker::end
    pugi::xml_tree_walker* walker = create_walker(Data, Size);
    if (walker && doc.root()) {
        // Trigger traversal which calls begin and end
        doc.root().traverse(*walker);
    }
    delete walker;

    return 0;
}