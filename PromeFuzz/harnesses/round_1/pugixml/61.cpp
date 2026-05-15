// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
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
#include <string>
#include <string_view>

static void initialize_document(pugi::xml_document& doc) {
    const char* xml_content = R"(<?xml version="1.0"?>
<root>
    <child1 attr="value1">Text1</child1>
    <child2 attr="value2">Text2</child2>
    <child3 attr="value3">Text3</child3>
    <child4 attr="value4">Text4</child4>
</root>)";
    doc.load_string(xml_content);
}

static std::string create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len = 100) {
    size_t len = size < max_len ? size : max_len;
    std::string result(reinterpret_cast<const char*>(data), len);
    if (!result.empty() && result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    initialize_document(doc);
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // Create null-terminated strings from fuzzer input
    std::string name1 = create_null_terminated_string(Data, Size);
    std::string name2 = create_null_terminated_string(Data + (Size / 2), Size - (Size / 2));
    
    // Convert to string_view for API calls
    std::string_view name1_sv(name1.c_str(), name1.size() - 1);
    std::string_view name2_sv(name2.c_str(), name2.size() - 1);

    // Test 1: parent()
    try {
        pugi::xml_node parent1 = root.parent();
        pugi::xml_node child = root.child("child1");
        if (child) {
            pugi::xml_node parent2 = child.parent();
        }
    } catch (...) {
        // Ignore exceptions
    }

    // Test 2: insert_move_after()
    try {
        pugi::xml_node node1 = root.child("child1");
        pugi::xml_node node2 = root.child("child2");
        pugi::xml_node node3 = root.child("child3");
        
        if (node1 && node2 && node3) {
            // Move node1 after node2
            root.insert_move_after(node1, node2);
            // Move node3 after node1 (which is now after node2)
            root.insert_move_after(node3, node1);
        }
    } catch (...) {
        // Ignore exceptions
    }

    // Test 3: child()
    try {
        pugi::xml_node found_child = root.child(name1_sv);
        pugi::xml_node found_child2 = root.child(name2_sv);
    } catch (...) {
        // Ignore exceptions
    }

    // Test 4: append_child()
    try {
        pugi::xml_node new_child = root.append_child(name1_sv);
        if (new_child) {
            pugi::xml_node grandchild = new_child.append_child(name2_sv);
        }
    } catch (...) {
        // Ignore exceptions
    }

    // Test 5: last_child()
    try {
        pugi::xml_node last = root.last_child();
        if (last) {
            pugi::xml_node last_of_last = last.last_child();
        }
    } catch (...) {
        // Ignore exceptions
    }

    // Test 6: prepend_child()
    try {
        pugi::xml_node prepended = root.prepend_child(name1_sv);
        if (prepended) {
            pugi::xml_node inner_prepended = prepended.prepend_child(name2_sv);
        }
    } catch (...) {
        // Ignore exceptions
    }

    return 0;
}