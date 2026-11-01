// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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

static bool fuzzer_tree_walker_begin(pugi::xml_node& node) {
    (void)node;
    return true;
}

static bool fuzzer_tree_walker_end(pugi::xml_node& node) {
    (void)node;
    return true;
}

static bool fuzzer_tree_walker_for_each(pugi::xml_node& node) {
    (void)node;
    return true;
}

class FuzzerTreeWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        return fuzzer_tree_walker_begin(node);
    }

    bool end(pugi::xml_node& node) override {
        return fuzzer_tree_walker_end(node);
    }

    bool for_each(pugi::xml_node& node) override {
        return fuzzer_tree_walker_for_each(node);
    }
};

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;

    std::string xml_string(reinterpret_cast<const char*>(Data), Size);
    if (xml_string.back() != '\0') {
        xml_string.push_back('\0');
    }

    pugi::xml_parse_result result = doc.load_string(xml_string.c_str());
    (void)result;

    pugi::xml_node root_node = doc.root();

    if (!root_node.empty()) {
        pugi::xml_node root_check = root_node.root();
        (void)root_check;
    }

    if (!root_node.empty()) {
        pugi::xml_node parent_node = root_node.parent();
        (void)parent_node;
    }

    if (!root_node.empty()) {
        pugi::xml_node next_sibling_no_name = root_node.next_sibling();
        (void)next_sibling_no_name;
    }

    if (!root_node.empty()) {
        std::string_view name_view = "test_name";
        pugi::xml_node next_sibling_with_name = root_node.next_sibling(name_view);
        (void)next_sibling_with_name;
    }

    FuzzerTreeWalker walker;
    if (!root_node.empty()) {
        root_node.traverse(walker);
    }

    return 0;
}