// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
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

static void fuzz_first_element_by_path(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size < 1) return;
    // Create a null-terminated path string from input data
    char path[256];
    size_t copy_size = (size < 255) ? size : 255;
    std::memcpy(path, data, copy_size);
    path[copy_size] = '\0';
    
    // Try with default delimiter
    pugi::xml_node result1 = node.first_element_by_path(path);
    (void)result1;
    
    // Try with different delimiters
    pugi::xml_node result2 = node.first_element_by_path(path, '\\');
    (void)result2;
    pugi::xml_node result3 = node.first_element_by_path(path, '.');
    (void)result3;
}

static void fuzz_append_child(pugi::xml_node& node, const uint8_t* data, size_t size) {
    // Use different node types based on input data
    if (size > 0) {
        pugi::xml_node_type type = static_cast<pugi::xml_node_type>(data[0] % 8);
        pugi::xml_node result = node.append_child(type);
        (void)result;
    }
}

static void fuzz_parent(pugi::xml_node& node) {
    pugi::xml_node parent = node.parent();
    (void)parent;
    
    // Navigate up multiple times
    pugi::xml_node current = node;
    for (int i = 0; i < 10; ++i) {
        if (current.type() == pugi::node_null) break;
        current = current.parent();
    }
    (void)current;
}

static void fuzz_find_child_by_attribute(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size < 2) return;
    
    // Create attribute name and value from input data
    // Ensure we have enough space for the null terminator
    char attr_name[64];
    char attr_value[64];
    
    size_t name_len = (size > 63) ? 63 : size;
    std::memcpy(attr_name, data, name_len);
    attr_name[name_len] = '\0';
    
    size_t remaining = size - name_len;
    size_t val_len = (remaining > 63) ? 63 : remaining;
    std::memcpy(attr_value, data + name_len, val_len);
    attr_value[val_len] = '\0';
    
    // Try both overloads
    pugi::xml_node result1 = node.find_child_by_attribute(attr_name, attr_value);
    (void)result1;
    
    pugi::xml_node result2 = node.find_child_by_attribute("test", attr_name, attr_value);
    (void)result2;
}

static void fuzz_insert_child_before(pugi::xml_node& parent, pugi::xml_node& sibling, const uint8_t* data, size_t size) {
    if (size < 1) return;
    
    // Create a name from input data
    char name[64];
    size_t name_len = (size < 63) ? size : 63;
    std::memcpy(name, data, name_len);
    name[name_len] = '\0';
    
    // Try to insert before sibling
    pugi::xml_node result = parent.insert_child_before(name, sibling);
    (void)result;
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Parse XML from input data
    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, create a minimal document manually
        pugi::xml_node root_node = doc.append_child(pugi::node_element);
        root_node.set_name("root");
        pugi::xml_attribute attr = root_node.append_attribute("id");
        attr.set_value("root");
    }
    
    pugi::xml_node root = doc.child("root");
    if (root.type() == pugi::node_null) {
        root = doc.first_child();
    }
    if (root.type() == pugi::node_null) {
        root = doc;
    }
    
    // Fuzz first_element_by_path
    fuzz_first_element_by_path(root, Data, Size);
    
    // Fuzz append_child
    fuzz_append_child(root, Data, Size);
    
    // Fuzz parent
    fuzz_parent(root);
    
    // Fuzz find_child_by_attribute
    fuzz_find_child_by_attribute(root, Data, Size);
    
    // Fuzz insert_child_before
    // Create some children to work with
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    
    if (child1.type() != pugi::node_null && child2.type() != pugi::node_null) {
        fuzz_insert_child_before(root, child2, Data, Size);
    }
    
    // Also try with different nodes
    pugi::xml_node temp = root.first_element_by_path("child1");
    if (temp.type() != pugi::node_null) {
        fuzz_insert_child_before(root, temp, Data, Size);
    }
    
    return 0;
}