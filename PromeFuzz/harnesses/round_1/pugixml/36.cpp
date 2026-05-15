// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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

static void initialize_dummy_file(const uint8_t* data, size_t size) {
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        fwrite(data, 1, size, file);
        fclose(file);
    }
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* Data, size_t Size) {
    // Initialize dummy file if needed
    initialize_dummy_file(Data, Size);
    
    // Create XML document
    pugi::xml_document doc;
    
    // Try to load from buffer first
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, create a simple document structure
    if (!result) {
        pugi::xml_node root = doc.append_child("root");
        pugi::xml_node child1 = root.append_child("child1");
        child1.append_child("subchild1");
        pugi::xml_node child2 = root.append_child("child2");
        child2.append_child("subchild2");
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    if (root.empty()) {
        root = doc.append_child("root");
    }
    
    // Prepare string views for node names
    std::string_view child_name = "test_child";
    std::string_view append_name1 = "append1";
    std::string_view append_name2 = "append2";
    std::string_view append_name3 = "append3";
    std::string_view append_name4 = "append4";
    
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name.data());
    
    // If child doesn't exist, create one to continue the chain
    if (child_node.empty()) {
        child_node = root.append_child(child_name.data());
    }
    
    // 2. pugi::xml_node::append_child
    pugi::xml_node appended1 = child_node.append_child(append_name1.data());
    
    // 3. pugi::xml_node::append_child
    pugi::xml_node appended2 = child_node.append_child(append_name2.data());
    
    // 4. pugi::xml_text::set (both overloads)
    if (!appended1.empty()) {
        pugi::xml_text text1 = appended1.text();
        // Use some data from fuzzer input for values
        long long_val = 0;
        unsigned long ulong_val = 0;
        
        // Extract values from fuzzer input if available
        if (Size >= sizeof(long) + sizeof(unsigned long)) {
            memcpy(&long_val, Data, sizeof(long));
            memcpy(&ulong_val, Data + sizeof(long), sizeof(unsigned long));
        } else if (Size >= sizeof(long)) {
            memcpy(&long_val, Data, sizeof(long));
        } else if (Size > 0) {
            long_val = static_cast<long>(Data[0]);
            ulong_val = static_cast<unsigned long>(Data[0]);
        }
        
        text1.set(long_val);
        text1.set(ulong_val);
    }
    
    // 5. pugi::xml_node::append_child
    pugi::xml_node appended3 = child_node.append_child(append_name3.data());
    
    // 6. pugi::xml_node::append_child
    pugi::xml_node appended4 = child_node.append_child(append_name4.data());
    
    // 7. pugi::xml_text::set (both overloads)
    if (!appended3.empty()) {
        pugi::xml_text text3 = appended3.text();
        // Use different data from fuzzer input for values
        long long_val2 = 0;
        unsigned long ulong_val2 = 0;
        
        // Extract values from different part of fuzzer input if available
        size_t offset = sizeof(long) + sizeof(unsigned long);
        if (Size >= offset + sizeof(long) + sizeof(unsigned long)) {
            memcpy(&long_val2, Data + offset, sizeof(long));
            memcpy(&ulong_val2, Data + offset + sizeof(long), sizeof(unsigned long));
        } else if (Size >= offset + sizeof(long)) {
            memcpy(&long_val2, Data + offset, sizeof(long));
        } else if (Size > offset) {
            long_val2 = static_cast<long>(Data[offset]);
            ulong_val2 = static_cast<unsigned long>(Data[offset]);
        } else if (Size > 0) {
            // Fallback to first byte
            long_val2 = static_cast<long>(Data[0]);
            ulong_val2 = static_cast<unsigned long>(Data[0]);
        }
        
        text3.set(long_val2);
        text3.set(ulong_val2);
    }
    
    return 0;
}