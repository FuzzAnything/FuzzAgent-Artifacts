// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
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
#include <vector>
#include <algorithm>

static void initialize_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    // Try to parse the input as XML first
    pugi::xml_parse_result result = doc.load_buffer(data, size, pugi::parse_default | pugi::parse_fragment);
    
    // If parsing fails or produces empty document, create a default structure
    if (!result || doc.first_child().empty()) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        
        // Create some initial nodes for testing
        for (int i = 0; i < 3; ++i) {
            pugi::xml_node child = root.append_child(("node" + std::to_string(i)).c_str());
            child.append_attribute(("attr" + std::to_string(i)).c_str()) = "value";
            
            // Add grandchildren
            for (int j = 0; j < 2; ++j) {
                child.append_child(("grandchild" + std::to_string(j)).c_str());
            }
        }
    }
}

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "";
    size_t length = std::min(size - offset, static_cast<size_t>(32)); // Limit string length
    return std::string_view(reinterpret_cast<const char*>(data + offset), length);
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t* data, size_t size) {
    if (size == 0) return 0;
    
    // Create main document
    pugi::xml_document doc1;
    initialize_document(doc1, data, size);
    
    // Create second document for cross-document operations
    pugi::xml_document doc2;
    pugi::xml_node root2 = doc2.append_child("doc2_root");
    root2.append_child("doc2_child1");
    root2.append_child("doc2_child2");
    
    // Get all nodes from both documents for testing
    std::vector<pugi::xml_node> nodes;
    
    // Collect nodes from doc1
    for (pugi::xml_node node = doc1.first_child(); node; node = node.next_sibling()) {
        nodes.push_back(node);
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            nodes.push_back(child);
        }
    }
    
    // Add nodes from doc2
    for (pugi::xml_node node = doc2.first_child(); node; node = node.next_sibling()) {
        nodes.push_back(node);
    }
    
    // Add empty nodes
    pugi::xml_node empty_node;
    nodes.push_back(empty_node);
    
    if (nodes.empty()) return 0;
    
    // Use input data to determine which operations to perform
    size_t data_index = 0;
    
    // Test insert_move_after
    if (nodes.size() >= 2) {
        size_t idx1 = data[data_index % size] % nodes.size();
        size_t idx2 = data[(data_index + 1) % size] % nodes.size();
        data_index += 2;
        
        try {
            pugi::xml_node target = nodes[idx1];
            pugi::xml_node moved = nodes[idx2];
            
            if (!target.empty() && !moved.empty()) {
                target.insert_move_after(moved, target);
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test insert_child_before
    if (nodes.size() >= 1) {
        size_t idx = data[data_index % size] % nodes.size();
        data_index += 1;
        
        try {
            pugi::xml_node target = nodes[idx];
            if (!target.empty()) {
                std::string_view name = get_string_view(data, size, data_index);
                data_index = (data_index + name.length()) % size;
                
                target.insert_child_before(name, target.first_child());
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test insert_child_after
    if (nodes.size() >= 1) {
        size_t idx = data[data_index % size] % nodes.size();
        data_index += 1;
        
        try {
            pugi::xml_node target = nodes[idx];
            if (!target.empty()) {
                std::string_view name = get_string_view(data, size, data_index);
                data_index = (data_index + name.length()) % size;
                
                target.insert_child_after(name, target.first_child());
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test append_child
    if (nodes.size() >= 1) {
        size_t idx = data[data_index % size] % nodes.size();
        data_index += 1;
        
        try {
            pugi::xml_node target = nodes[idx];
            if (!target.empty()) {
                std::string_view name = get_string_view(data, size, data_index);
                data_index = (data_index + name.length()) % size;
                
                target.append_child(name);
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test prepend_child
    if (nodes.size() >= 1) {
        size_t idx = data[data_index % size] % nodes.size();
        data_index += 1;
        
        try {
            pugi::xml_node target = nodes[idx];
            if (!target.empty()) {
                std::string_view name = get_string_view(data, size, data_index);
                data_index = (data_index + name.length()) % size;
                
                target.prepend_child(name);
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test prepend_move
    if (nodes.size() >= 2) {
        size_t idx1 = data[data_index % size] % nodes.size();
        size_t idx2 = data[(data_index + 1) % size] % nodes.size();
        data_index += 2;
        
        try {
            pugi::xml_node target = nodes[idx1];
            pugi::xml_node moved = nodes[idx2];
            
            if (!target.empty() && !moved.empty()) {
                target.prepend_move(moved);
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test operations with empty nodes
    try {
        pugi::xml_node empty1;
        pugi::xml_node empty2;
        empty1.insert_move_after(empty2, empty1);
        empty1.insert_child_before("test", empty2);
        empty1.insert_child_after("test", empty2);
        empty1.append_child("test");
        empty1.prepend_child("test");
        empty1.prepend_move(empty2);
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test operations with nodes from different documents
    if (nodes.size() >= 2) {
        try {
            pugi::xml_node node_from_doc1 = doc1.first_child();
            pugi::xml_node node_from_doc2 = doc2.first_child();
            
            if (!node_from_doc1.empty() && !node_from_doc2.empty()) {
                node_from_doc1.insert_move_after(node_from_doc2, node_from_doc1);
                node_from_doc1.prepend_move(node_from_doc2);
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test operations with various string inputs
    if (nodes.size() >= 1) {
        size_t idx = data[data_index % size] % nodes.size();
        
        try {
            pugi::xml_node target = nodes[idx];
            if (!target.empty()) {
                // Test with empty string
                target.append_child("");
                target.prepend_child("");
                
                // Test with special characters
                target.append_child("node with spaces");
                target.append_child("node\twith\ttabs");
                target.append_child("node\nwith\nnewlines");
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    return 0;
}