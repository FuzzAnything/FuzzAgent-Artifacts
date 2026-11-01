// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <string_view>

static pugi::xml_document* g_doc = nullptr;

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    
    // Ensure data is null-terminated for parsing
    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    if (xml_str.back() != '\0') {
        xml_str += '\0';
    }

    // Parse the input
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    if (!result) {
        // Invalid XML is fine for fuzzing, but we need a valid document structure to test navigation
        // If parsing fails completely, we might not have nodes to walk, but we can still test empty states
    }

    // Create a custom walker to test begin and end
    struct TestWalker : public pugi::xml_tree_walker {
        virtual bool begin(pugi::xml_node& node) override {
            // Test parent()
            pugi::xml_node p = node.parent();
            (void)p; // Suppress unused warning

            // Test root()
            pugi::xml_node r = node.root();
            (void)r;

            // Test next_sibling() overloads
            pugi::xml_node ns1 = node.next_sibling();
            (void)ns1;
            
            std::string_view a_str("test");
            pugi::xml_node ns2 = node.next_sibling(a_str.data());
            (void)ns2;

            // Randomly decide to skip children or continue
            // Use node name length to make decision deterministic based on input
            if (node.name() && strlen(node.name()) % 2 == 0) {
                return true;
            }
            return false;
        }

        virtual bool end(pugi::xml_node& node) override {
            // Test parent() again
            pugi::xml_node p = node.parent();
            (void)p;

            // Test root() again
            pugi::xml_node r = node.root();
            (void)r;

            // Test next_sibling() overloads again
            pugi::xml_node ns1 = node.next_sibling();
            (void)ns1;
            
            std::string_view a_str("test");
            pugi::xml_node ns2 = node.next_sibling(a_str.data());
            (void)ns2;

            // Randomly decide to stop traversal
            if (node.name() && strlen(node.name()) % 3 == 0) {
                return false;
            }
            return true;
        }

        // Implement the missing pure virtual method
        virtual bool for_each(pugi::xml_node& node) override {
            // Test parent()
            pugi::xml_node p = node.parent();
            (void)p;

            // Test root()
            pugi::xml_node r = node.root();
            (void)r;

            // Test next_sibling() overloads
            pugi::xml_node ns1 = node.next_sibling();
            (void)ns1;
            
            std::string_view a_str("test");
            pugi::xml_node ns2 = node.next_sibling(a_str.data());
            (void)ns2;

            return true;
        }
    };

    TestWalker walker;

    // Traverse the document if it has a root
    if (doc.root()) {
        doc.root().traverse(walker);
    }

    // Test navigation on root node directly
    pugi::xml_node root = doc.root();
    // Check if node is empty (invalid) using .empty() instead of .is_null()
    if (!root.empty()) {
        pugi::xml_node p = root.parent();
        (void)p;
        
        pugi::xml_node r = root.root();
        (void)r;

        pugi::xml_node ns1 = root.next_sibling();
        (void)ns1;

        std::string_view a_str("test");
        pugi::xml_node ns2 = root.next_sibling(a_str.data());
        (void)ns2;
    }

    // Test on an empty node (default constructed)
    pugi::xml_node empty_node;
    pugi::xml_node p_empty = empty_node.parent();
    (void)p_empty;
    pugi::xml_node r_empty = empty_node.root();
    (void)r_empty;
    pugi::xml_node ns_empty1 = empty_node.next_sibling();
    (void)ns_empty1;
    std::string_view a_str_empty("test");
    pugi::xml_node ns_empty2 = empty_node.next_sibling(a_str_empty.data());
    (void)ns_empty2;

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}