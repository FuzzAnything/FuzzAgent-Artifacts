// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include <string_view>

static void initialize_document(pugi::xml_document& doc) {
    // Create a root node to work with
    pugi::xml_node root = doc.append_child("root");
    
    // Add some initial structure to have children to search for
    root.append_child("child1");
    root.append_child("child2");
    root.append_child("child3");
    
    // Add a child with attributes for more complex structure
    pugi::xml_node child_with_attr = root.append_child("child_with_attr");
    child_with_attr.append_attribute("attr1") = "value1";
    child_with_attr.append_attribute("attr2") = "value2";
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Initialize pugi::xml_document
    pugi::xml_document doc;
    initialize_document(doc);
    
    // Get the root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }
    
    // 1. pugi::xml_node::child
    // Create a null-terminated string from fuzzer input for child name
    std::string child_name;
    if (Size > 0) {
        size_t name_len = Size > 100 ? 100 : Size; // Limit name length
        child_name.assign(reinterpret_cast<const char*>(Data), name_len);
        // Ensure null termination
        if (child_name.back() != '\0') {
            child_name.push_back('\0');
        }
    } else {
        child_name = "default";
    }
    
    pugi::xml_node found_child = root.child(child_name.c_str());
    
    // 2. pugi::xml_node::append_child
    // Use first part of data for first append_child name
    std::string append_name1;
    if (Size > 0) {
        size_t name_len = Size > 50 ? 50 : Size;
        append_name1.assign(reinterpret_cast<const char*>(Data), name_len);
        if (append_name1.back() != '\0') {
            append_name1.push_back('\0');
        }
    } else {
        append_name1 = "append1";
    }
    
    pugi::xml_node appended1 = root.append_child(append_name1.c_str());
    
    // 3. pugi::xml_node::append_child (second call)
    // Use different part of data for second append_child name
    std::string append_name2;
    if (Size > 10) {
        size_t offset = Size > 100 ? 50 : Size/2;
        size_t name_len = (Size - offset) > 50 ? 50 : Size - offset;
        append_name2.assign(reinterpret_cast<const char*>(Data) + offset, name_len);
        if (append_name2.back() != '\0') {
            append_name2.push_back('\0');
        }
    } else {
        append_name2 = "append2";
    }
    
    pugi::xml_node appended2 = appended1.append_child(append_name2.c_str());
    
    // 4. pugi::xml_text::set (both overloads)
    if (appended2) {
        pugi::xml_text text_node = appended2.text();
        
        // Convert part of fuzzer data to long long
        long long ll_value = 0;
        if (Size >= sizeof(long long)) {
            std::memcpy(&ll_value, Data, sizeof(long long));
        } else if (Size > 0) {
            // Use available bytes
            std::memcpy(&ll_value, Data, Size);
        }
        text_node.set(ll_value);
        
        // Convert part of fuzzer data to unsigned long long
        unsigned long long ull_value = 0;
        if (Size >= sizeof(unsigned long long)) {
            size_t offset = Size > sizeof(unsigned long long) ? Size - sizeof(unsigned long long) : 0;
            std::memcpy(&ull_value, Data + offset, sizeof(unsigned long long));
        } else if (Size > 0) {
            std::memcpy(&ull_value, Data, Size);
        }
        text_node.set(ull_value);
    }
    
    // 5. pugi::xml_node::append_child (third call)
    std::string append_name3;
    if (Size > 20) {
        size_t offset = Size > 150 ? 100 : Size/3;
        size_t name_len = (Size - offset) > 30 ? 30 : Size - offset;
        append_name3.assign(reinterpret_cast<const char*>(Data) + offset, name_len);
        if (append_name3.back() != '\0') {
            append_name3.push_back('\0');
        }
    } else {
        append_name3 = "append3";
    }
    
    pugi::xml_node appended3 = root.append_child(append_name3.c_str());
    
    // 6. pugi::xml_node::append_child (fourth call)
    std::string append_name4;
    if (Size > 30) {
        size_t offset = Size > 200 ? 150 : Size * 2 / 3;
        size_t name_len = (Size - offset) > 20 ? 20 : Size - offset;
        append_name4.assign(reinterpret_cast<const char*>(Data) + offset, name_len);
        if (append_name4.back() != '\0') {
            append_name4.push_back('\0');
        }
    } else {
        append_name4 = "append4";
    }
    
    pugi::xml_node appended4 = appended3.append_child(append_name4.c_str());
    
    // 7. pugi::xml_text::set (both overloads again)
    if (appended4) {
        pugi::xml_text text_node2 = appended4.text();
        
        // Use different parts of data for second set of values
        long long ll_value2 = 0;
        if (Size >= sizeof(long long) * 2) {
            size_t offset = sizeof(long long);
            std::memcpy(&ll_value2, Data + offset, sizeof(long long));
        } else if (Size > sizeof(long long)) {
            size_t offset = Size - sizeof(long long);
            std::memcpy(&ll_value2, Data + offset, Size - offset);
        }
        text_node2.set(ll_value2);
        
        unsigned long long ull_value2 = 0;
        if (Size >= sizeof(unsigned long long) * 2) {
            size_t offset = sizeof(unsigned long long);
            std::memcpy(&ull_value2, Data + offset, sizeof(unsigned long long));
        } else if (Size > sizeof(unsigned long long)) {
            size_t offset = Size - sizeof(unsigned long long);
            std::memcpy(&ull_value2, Data + offset, Size - offset);
        }
        text_node2.set(ull_value2);
    }
    
    // Clean up - let RAII handle document destruction
    return 0;
}