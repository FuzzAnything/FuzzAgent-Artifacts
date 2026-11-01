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

static bool fuzzer_tree_walker_begin(pugi::xml_node& node) {
    return true;
}

static bool fuzzer_tree_walker_end(pugi::xml_node& node) {
    return true;
}

static bool fuzzer_tree_walker_for_each(pugi::xml_node& node) {
    return true;
}

class FuzzTreeWalker : public pugi::xml_tree_walker {
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

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    pugi::xml_node root_node = doc.root();
    
    // Test pugi::xml_node::root()
    if (!root_node.empty()) {
        pugi::xml_node retrieved_root = root_node.root();
        (void)retrieved_root;
    }
    
    // Test pugi::xml_node::parent()
    if (!root_node.empty()) {
        pugi::xml_node parent_node = root_node.parent();
        (void)parent_node;
    }
    
    // Test pugi::xml_node::next_sibling() without name
    if (!root_node.empty()) {
        pugi::xml_node next_sibling_node = root_node.next_sibling();
        (void)next_sibling_node;
    }
    
    // Test pugi::xml_node::next_sibling() with name
    if (!root_node.empty()) {
        std::string_view name_view = "test";
        pugi::xml_node next_sibling_named = root_node.next_sibling(name_view.data());
        (void)next_sibling_named;
    }
    
    // Test pugi::xml_tree_walker::begin and end via traversal
    FuzzTreeWalker walker;
    doc.traverse(walker);
    
    return 0;
}