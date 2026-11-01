// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
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
#include <cstring>
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");

    // Test with parsed document
    pugi::xml_node root_node = doc.root();
    if (root_node) {
        // Test xml_node::root
        pugi::xml_node r = root_node.root();
        (void)r;

        // Test xml_node::parent
        pugi::xml_node p = root_node.parent();
        (void)p;

        // Test xml_node::next_sibling (overload 1)
        pugi::xml_node ns1 = root_node.next_sibling();
        (void)ns1;

        // Test xml_node::next_sibling (overload 2)
        std::string_view name_sv = "test";
        pugi::xml_node ns2 = root_node.next_sibling(name_sv);
        (void)ns2;
    }

    // Test with empty node
    pugi::xml_node empty_node;
    pugi::xml_node r_empty = empty_node.root();
    (void)r_empty;

    pugi::xml_node p_empty = empty_node.parent();
    (void)p_empty;

    pugi::xml_node ns1_empty = empty_node.next_sibling();
    (void)ns1_empty;

    std::string_view name_sv_empty = "test";
    pugi::xml_node ns2_empty = empty_node.next_sibling(name_sv_empty);
    (void)ns2_empty;

    // Custom walker to test begin and end
    struct TestWalker : public pugi::xml_tree_walker {
        bool begin(pugi::xml_node& node) override {
            // Test logic: always continue
            return true;
        }

        bool end(pugi::xml_node& node) override {
            // Test logic: always continue
            return true;
        }

        bool for_each(pugi::xml_node& node) override {
            // Required pure virtual method
            return true;
        }
    };

    if (root_node) {
        TestWalker walker;
        root_node.traverse(walker);
    }

    // Another walker that stops traversal
    struct StopWalker : public pugi::xml_tree_walker {
        bool begin(pugi::xml_node& node) override {
            return false;
        }

        bool end(pugi::xml_node& node) override {
            return false;
        }

        bool for_each(pugi::xml_node& node) override {
            return false;
        }
    };

    if (root_node) {
        StopWalker stopper;
        root_node.traverse(stopper);
    }

    return 0;
}