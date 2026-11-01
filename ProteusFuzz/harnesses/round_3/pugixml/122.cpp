// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
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
#include <string_view>

static bool test_tree_walker(pugi::xml_node& node) {
    class TestWalker : public pugi::xml_tree_walker {
    public:
        TestWalker() : depth(0) {}
        virtual ~TestWalker() = default;

        bool begin(pugi::xml_node& n) override {
            depth++;
            return true;
        }

        bool end(pugi::xml_node& n) override {
            depth--;
            return true;
        }

        bool for_each(pugi::xml_node& n) override {
            return true;
        }

    private:
        int depth;
    };

    TestWalker walker;
    node.traverse(walker);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure the input is null-terminated to prevent buffer overflows in string parsing
    // We create a temporary buffer with a guaranteed null terminator
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    // If parsing failed, we still proceed to test API behavior on potentially empty or partial documents
    
    pugi::xml_node root = doc.root();
    
    // Test 1: xml_node::root()
    if (!root.empty()) {
        pugi::xml_node root2 = root.root();
        (void)root2; // Suppress unused variable warning
    }

    // Test 2: xml_node::next_sibling() overloads
    if (!root.empty()) {
        pugi::xml_node next = root.next_sibling();
        (void)next;
        
        std::string_view name_view = "test";
        pugi::xml_node next_named = root.next_sibling(name_view);
        (void)next_named;
    }

    // Test 3 & 4: xml_tree_walker::begin and xml_tree_walker::end
    if (!root.empty()) {
        test_tree_walker(root);
    }

    // Test 5: xml_node::parent()
    if (!root.empty()) {
        pugi::xml_node parent = root.parent();
        (void)parent;
        
        // Try traversing children to test parent on non-root nodes
        for (pugi::xml_node child = root.first_child(); !child.empty(); child = child.next_sibling()) {
            pugi::xml_node child_parent = child.parent();
            (void)child_parent;
            
            // Test next_sibling on children
            pugi::xml_node child_next = child.next_sibling();
            (void)child_next;
            
            std::string_view child_name_view = "dummy";
            pugi::xml_node child_next_named = child.next_sibling(child_name_view);
            (void)child_next_named;
        }
    }

    return 0;
}