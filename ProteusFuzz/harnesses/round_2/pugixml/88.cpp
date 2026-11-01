// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
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
#include <cstring>

static bool custom_tree_walker_begin(pugi::xml_node& node) {
    (void)node;
    return true;
}

static bool custom_tree_walker_end(pugi::xml_node& node) {
    (void)node;
    return true;
}

static bool custom_tree_walker_for_each(pugi::xml_node& node) {
    (void)node;
    return true;
}

static class CustomTreeWalker : public pugi::xml_tree_walker {
public:
    CustomTreeWalker() : pugi::xml_tree_walker() {}

    bool begin(pugi::xml_node& node) override {
        return custom_tree_walker_begin(node);
    }

    bool end(pugi::xml_node& node) override {
        return custom_tree_walker_end(node);
    }

    bool for_each(pugi::xml_node& node) override {
        return custom_tree_walker_for_each(node);
    }
};

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    if (!result) {
        // Parsing failed, but we can still test with the document state
    }

    pugi::xml_node root_node = doc.root();
    
    if (root_node) {
        pugi::xml_node current = root_node.first_child();
        while (current) {
            pugi::xml_node parent_node = current.parent();
            (void)parent_node;

            pugi::xml_node next_sibling_no_name = current.next_sibling();
            (void)next_sibling_no_name;

            std::string_view a_str = "test";
            pugi::xml_node next_sibling_with_name = current.next_sibling(a_str.data());
            (void)next_sibling_with_name;

            current = next_sibling_no_name;
        }
    }

    CustomTreeWalker walker;
    doc.traverse(walker);

    return 0;
}