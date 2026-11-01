// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    pugi::xml_document doc;
    
    // Ensure the input data is null-terminated for safe string operations
    // We create a temporary buffer to hold the data with a null terminator
    std::vector<char> buffer(Size + 1);
    if (Size > 0) {
        std::memcpy(buffer.data(), Data, Size);
    }
    buffer[Size] = '\0';

    // Try to load the document from the null-terminated buffer
    // If it fails, we construct a minimal valid document to test APIs
    if (!doc.load_string(buffer.data())) {
        if (!doc.append_child("root")) return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root found, try to create one
        root = doc.append_child("root");
        if (!root) return 0;
    }

    // Create some children and attributes to test with
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");

    if (child1) {
        child1.append_attribute("attr1").set_value("val1");
        child1.append_attribute("attr2").set_value("val2");
    }
    if (child2) {
        child2.append_attribute("attr1").set_value("val2");
    }

    // Test 1: insert_copy_after
    // Requires valid proto (attribute) and attr (attribute) siblings
    if (child1) {
        pugi::xml_attribute attr1 = child1.attribute("attr1");
        pugi::xml_attribute attr2 = child1.attribute("attr2");
        if (attr1 && attr2) {
            // Try to insert a copy of attr1 after attr2
            child1.insert_copy_after(attr1, attr2);
        }
        // Test with invalid proto
        pugi::xml_attribute invalid_attr;
        child1.insert_copy_after(invalid_attr, attr2);
        // Test with invalid attr
        child1.insert_copy_after(attr1, invalid_attr);
    }

    // Test 2: append_child
    // Use data to determine name if possible, otherwise use fixed names
    // We use the safe buffer to construct a name
    std::string safe_name = "fuzz_child";
    size_t name_len = 0;
    for (size_t i = 0; i < Size && name_len < 20; ++i) {
        char c = buffer[i];
        if (c >= 32 && c <= 126 && c != '<' && c != '>' && c != '&' && c != '"' && c != '\'') {
            safe_name += c;
            name_len++;
        } else {
            safe_name += 'x';
            name_len++;
        }
    }
    root.append_child(safe_name.c_str());

    // Test 3: last_child
    pugi::xml_node last = root.last_child();
    if (last) {
        // Do something with last to avoid unused warning
        last.child("dummy");
    }

    // Test 4: append_move
    // Move child3 to root (it's already a child, but let's move it to another child if exists)
    if (child1 && child3) {
        child1.append_move(child3);
    }
    // Try to move an invalid node
    pugi::xml_node invalid_node;
    root.append_move(invalid_node);

    // Test 5: insert_move_before
    // Move child2 before child1
    if (child1 && child2) {
        root.insert_move_before(child2, child1);
    }
    // Try with invalid node
    root.insert_move_before(invalid_node, child1);
    // Try with invalid target
    root.insert_move_before(child2, invalid_node);

    // Test 6: child
    // Search for a child by name
    pugi::xml_node found = root.child(safe_name.c_str());
    if (!found) {
        // Try finding "child1" which we created
        found = root.child("child1");
    }

    // Additional fuzzing: try to create a deep tree and test edge cases
    pugi::xml_node current = root;
    for (size_t i = 0; i < Size && i < 100; ++i) {
        std::string next_name = "nested";
        next_name += std::to_string(i);
        pugi::xml_node next = current.append_child(next_name.c_str());
        if (next) {
            current = next;
            // Test child on the new node
            current.child("nonexistent");
        }
    }

    return 0;
}