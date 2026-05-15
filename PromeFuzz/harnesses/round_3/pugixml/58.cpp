// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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
#include <fstream>
#include <string>
#include <vector>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static std::string getRandomString(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "";
    size_t len = data[offset] % 32;
    if (offset + len >= size) len = size - offset - 1;
    if (len == 0) return "";
    std::string result(reinterpret_cast<const char*>(data + offset), len);
    result.push_back('\0');
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;

    writeDummyFile(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (!result) {
        return 0;
    }

    std::vector<pugi::xml_node> nodes;
    nodes.push_back(doc.root());
    
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        nodes.push_back(node);
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            nodes.push_back(child);
        }
    }

    for (const auto& node : nodes) {
        if (!node) continue;

        try {
            std::string path_result = node.path('/');
            path_result = node.path('.');
            
            pugi::xml_node parent_node = node.parent();
            
            pugi::xml_node last_child_node = node.last_child();
            
            size_t name_offset = Size > 1 ? Data[0] % Size : 0;
            std::string child_name = getRandomString(Data, Size, name_offset);
            if (!child_name.empty()) {
                pugi::xml_node child_node = node.child(child_name.c_str());
            }
            
            pugi::xml_node prev_sibling = node.previous_sibling();
            
            size_t sibling_name_offset = Size > 2 ? Data[1] % Size : 0;
            std::string sibling_name = getRandomString(Data, Size, sibling_name_offset);
            if (!sibling_name.empty()) {
                pugi::xml_node named_prev_sibling = node.previous_sibling(sibling_name.c_str());
            }
            
            pugi::xml_node first_child = node.first_child();
            
            if (!parent_node.empty()) {
                std::string parent_path = parent_node.path('/');
                pugi::xml_node parent_parent = parent_node.parent();
            }
            
            if (!last_child_node.empty()) {
                pugi::xml_node last_child_child = last_child_node.first_child();
            }
            
            if (!first_child.empty()) {
                pugi::xml_node first_child_parent = first_child.parent();
            }
            
        } catch (...) {
        }
    }

    return 0;
}