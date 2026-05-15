// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstddef>
#include <string_view>

static class test_walker : public pugi::xml_tree_walker
{
public:
    virtual bool begin(pugi::xml_node& node) override
    {
        (void)node.parent();
        (void)node.next_sibling();
        (void)node.root();
        return true;
    }

    virtual bool for_each(pugi::xml_node& node) override
    {
        (void)node.parent();
        (void)node.next_sibling();
        (void)node.root();
        return true;
    }

    virtual bool end(pugi::xml_node& node) override
    {
        (void)node.parent();
        (void)node.next_sibling();
        (void)node.root();
        return true;
    }
} walker_instance;

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size)
{
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    
    (void)root.parent();
    (void)root.next_sibling();
    (void)root.root();

    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        (void)child.parent();
        (void)child.next_sibling();
        (void)child.root();
        
        std::string_view name_view;
        (void)child.next_sibling(name_view);
        
        if (child.name()) {
            std::string_view node_name(child.name());
            (void)child.next_sibling(node_name);
        }
    }

    pugi::xml_node empty_node;
    (void)empty_node.parent();
    (void)empty_node.next_sibling();
    (void)empty_node.root();

    root.traverse(walker_instance);

    return 0;
}