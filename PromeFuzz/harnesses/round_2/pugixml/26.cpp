// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
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
#include <vector>

static std::vector<std::string> split_data(const uint8_t* data, size_t size, size_t chunk_count) {
    std::vector<std::string> chunks;
    if (size == 0 || chunk_count == 0) return chunks;
    
    size_t chunk_size = size / chunk_count;
    for (size_t i = 0; i < chunk_count; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == chunk_count - 1) ? size : start + chunk_size;
        if (start < end && start < size) {
            chunks.emplace_back(reinterpret_cast<const char*>(data + start), 
                              std::min(end - start, size - start));
        } else {
            chunks.emplace_back("");
        }
    }
    return chunks;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* data, size_t size) {
    // Split input into chunks for various string parameters
    auto chunks = split_data(data, size, 20); // We need multiple strings
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // First sequence: append_child (4 times)
    // Call both overloads alternating
    pugi::xml_node child1 = root.append_child(pugi::node_element);
    pugi::xml_node child2 = root.append_child(chunks[0].c_str());
    pugi::xml_node child3 = root.append_child(pugi::node_comment);
    pugi::xml_node child4 = root.append_child(chunks[1].c_str());
    
    // Second sequence: append_attribute (3 times)
    if (!chunks[2].empty()) {
        pugi::xml_attribute attr1 = root.append_attribute(chunks[2].c_str());
        if (attr1 && !chunks[5].empty()) {
            attr1.set_value(chunks[5].c_str());
        }
    }
    
    if (!chunks[3].empty()) {
        pugi::xml_attribute attr2 = root.append_attribute(chunks[3].c_str());
        if (attr2 && !chunks[6].empty()) {
            attr2.set_value(chunks[6].c_str());
        }
    }
    
    if (!chunks[4].empty()) {
        pugi::xml_attribute attr3 = root.append_attribute(chunks[4].c_str());
        if (attr3 && !chunks[7].empty()) {
            attr3.set_value(chunks[7].c_str());
        }
    }
    
    // Add attributes to children to make find operations more interesting
    if (child1 && !chunks[8].empty()) {
        pugi::xml_attribute child_attr1 = child1.append_attribute(chunks[8].c_str());
        if (child_attr1 && !chunks[9].empty()) {
            child_attr1.set_value(chunks[9].c_str());
        }
    }
    
    if (child2 && !chunks[10].empty()) {
        pugi::xml_attribute child_attr2 = child2.append_attribute(chunks[10].c_str());
        if (child_attr2 && !chunks[11].empty()) {
            child_attr2.set_value(chunks[11].c_str());
        }
    }
    
    // Third sequence: find_child_by_attribute (8 times)
    // Alternate between the two overloads
    for (int i = 0; i < 8; ++i) {
        size_t base_idx = 12 + (i * 2);
        if (i % 2 == 0) {
            // Two-parameter overload: (attr_name, attr_value)
            if (base_idx < chunks.size() && base_idx + 1 < chunks.size()) {
                root.find_child_by_attribute(
                    chunks[base_idx].c_str(),
                    chunks[base_idx + 1].c_str()
                );
            }
        } else {
            // Three-parameter overload: (name, attr_name, attr_value)
            if (base_idx < chunks.size() && base_idx + 2 < chunks.size()) {
                root.find_child_by_attribute(
                    chunks[base_idx].c_str(),
                    chunks[base_idx + 1].c_str(),
                    chunks[base_idx + 2].c_str()
                );
            }
        }
    }
    
    return 0;
}