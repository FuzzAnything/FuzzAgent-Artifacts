// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <fstream>

#include "pugixml.hpp"

static void create_dummy_xml(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    
    // Write a minimal XML structure with some attributes and children
    file << "<root attr1=\"value1\" attr2=\"value2\" attr3=\"value3\">";
    file << "<child1/><child2/><child3/>";
    file << "</root>";
    file.close();
}

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "";
    
    const char* str_start = reinterpret_cast<const char*>(data + offset);
    size_t remaining = size - offset;
    
    // Find null terminator within bounds
    const char* null_pos = static_cast<const char*>(memchr(str_start, '\0', remaining));
    if (null_pos) {
        return std::string_view(str_start, null_pos - str_start);
    }
    
    // No null terminator found, use remaining bytes
    return std::string_view(str_start, remaining);
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* data, size_t size) {
    // Create dummy XML file
    create_dummy_xml(data, size);
    
    // Load XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    if (!result) {
        // If file loading fails, create a simple document in memory
        doc.append_child("root").append_attribute("attr1") = "value1";
        doc.child("root").append_attribute("attr2") = "value2";
        doc.child("root").append_attribute("attr3") = "value3";
        doc.child("root").append_child("child1");
        doc.child("root").append_child("child2");
    }
    
    // Get root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        // Create root if it doesn't exist
        root = doc.append_child("root");
        root.append_attribute("test") = "value";
        root.append_child("testchild");
    }
    
    // Generate string views from fuzzer input for function calls
    // We'll use different offsets to get varied inputs
    std::string_view sv1 = get_string_view(data, size, 0);
    std::string_view sv2 = get_string_view(data, size, sv1.length() + 1);
    std::string_view sv3 = get_string_view(data, size, sv1.length() + sv2.length() + 2);
    std::string_view sv4 = get_string_view(data, size, sv1.length() + sv2.length() + sv3.length() + 3);
    std::string_view sv5 = get_string_view(data, size, sv1.length() + sv2.length() + sv3.length() + sv4.length() + 4);
    std::string_view sv6 = get_string_view(data, size, sv1.length() + sv2.length() + sv3.length() + sv4.length() + sv5.length() + 5);
    std::string_view sv7 = get_string_view(data, size, sv1.length() + sv2.length() + sv3.length() + sv4.length() + sv5.length() + sv6.length() + 6);
    
    // Call functions in specified order
    try {
        // 1. First attribute call
        pugi::xml_attribute attr1 = root.attribute(sv1);
        
        // 2. Child call
        pugi::xml_node child = root.child(sv2);
        
        // 3. Second attribute call
        pugi::xml_attribute attr3 = root.attribute(sv3);
        
        // 4. Third attribute call
        pugi::xml_attribute attr4 = root.attribute(sv4);
        
        // 5. Fourth attribute call
        pugi::xml_attribute attr5 = root.attribute(sv5);
        
        // 6. Last attribute call
        pugi::xml_attribute last_attr = root.last_attribute();
        
        // 7. Fifth attribute call
        pugi::xml_attribute attr7 = root.attribute(sv6);
        
        // 8. Sixth attribute call
        pugi::xml_attribute attr8 = root.attribute(sv7);
        
        // Use the results to avoid optimization
        (void)attr1;
        (void)child;
        (void)attr3;
        (void)attr4;
        (void)attr5;
        (void)last_attr;
        (void)attr7;
        (void)attr8;
        
    } catch (...) {
        // Catch any exceptions to prevent crash
    }
    
    // Cleanup
    std::remove("./dummy_file");
    
    return 0;
}