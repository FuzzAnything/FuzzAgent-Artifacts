// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static std::string_view getStringView(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "";
    size_t length = std::min(size - offset, static_cast<size_t>(256));
    return std::string_view(reinterpret_cast<const char*>(data + offset), length);
}

static const char* getNullTerminated(const uint8_t* data, size_t size, size_t offset) {
    static thread_local char buffer[257];
    if (offset >= size) return "";
    size_t length = std::min(size - offset, static_cast<size_t>(256));
    std::memcpy(buffer, data + offset, length);
    buffer[length] = '\0';
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* data, size_t size) {
    if (size == 0) return 0;

    // Create dummy file for potential file-based parsing
    writeDummyFile(data, size);

    // Parse XML from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    // Get root node
    pugi::xml_node root = doc.document_element();
    
    // Test parent() function
    if (root) {
        pugi::xml_node parent = root.parent();
        (void)parent; // Use result to avoid unused variable warning
    }
    
    // Test find_child_by_attribute() function
    if (root) {
        const char* name = getNullTerminated(data, size, 0);
        const char* attr_name = getNullTerminated(data, size, size/3);
        const char* attr_value = getNullTerminated(data, size, 2*size/3);
        pugi::xml_node found = root.find_child_by_attribute(name, attr_name, attr_value);
        (void)found;
    }
    
    // Test next_sibling() function
    if (root) {
        const char* name = getNullTerminated(data, size, size/4);
        pugi::xml_node sibling = root.next_sibling(name);
        (void)sibling;
        
        // Also test on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node next = child.next_sibling(name);
            (void)next;
        }
    }
    
    // Test first_element_by_path() function
    if (root) {
        const char* path = getNullTerminated(data, size, size/2);
        char delimiter = size > 0 ? static_cast<char>(data[0]) : '/';
        pugi::xml_node path_node = root.first_element_by_path(path, delimiter);
        (void)path_node;
    }
    
    // Test first_child() function
    if (root) {
        pugi::xml_node first = root.first_child();
        (void)first;
        
        // Chain first_child() calls
        if (first) {
            pugi::xml_node grandchild = first.first_child();
            (void)grandchild;
        }
    }
    
    // Test child() function
    if (root) {
        // Test with string_view
        std::string_view name_view = getStringView(data, size, size/5);
        pugi::xml_node child1 = root.child(name_view);
        (void)child1;
        
        // Test with null-terminated string
        const char* name_cstr = getNullTerminated(data, size, size/6);
        pugi::xml_node child2 = root.child(name_cstr);
        (void)child2;
        
        // Test on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node grandchild = child.child(name_view);
            (void)grandchild;
        }
    }
    
    // Test on empty nodes
    pugi::xml_node empty_node;
    (void)empty_node.parent();
    (void)empty_node.find_child_by_attribute("", "", "");
    (void)empty_node.next_sibling("");
    (void)empty_node.first_element_by_path("");
    (void)empty_node.first_child();
    (void)empty_node.child("");
    
    // Test on various nodes in the tree
    try {
        pugi::xml_node node = doc.first_child();
        while (node) {
            // Test all functions on this node
            (void)node.parent();
            
            const char* name1 = getNullTerminated(data, size, 0);
            const char* attr_name1 = getNullTerminated(data, size, size % 256);
            const char* attr_value1 = getNullTerminated(data, size, (size * 2) % 256);
            (void)node.find_child_by_attribute(name1, attr_name1, attr_value1);
            
            const char* name2 = getNullTerminated(data, size, (size * 3) % 256);
            (void)node.next_sibling(name2);
            
            const char* path = getNullTerminated(data, size, (size * 4) % 256);
            (void)node.first_element_by_path(path);
            
            (void)node.first_child();
            
            std::string_view name_view = getStringView(data, size, (size * 5) % 256);
            (void)node.child(name_view);
            
            node = node.next_sibling();
        }
    } catch (...) {
        // Catch any exceptions to continue fuzzing
    }
    
    return 0;
}