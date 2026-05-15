// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Create a document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("root");
    
    // Add some dummy child nodes for removal operations
    for (int i = 0; i < 10; ++i) {
        node.append_child("child1");
        node.append_child("child2");
        node.append_child("child3");
    }
    
    // Use the fuzzer input to generate string parameters
    // Ensure we have at least some data to work with
    if (Size == 0) {
        return 0;
    }
    
    // Create null-terminated strings from fuzzer input
    std::string str1, str2, str3;
    if (Size >= 1) {
        str1 = std::string(1, static_cast<char>(Data[0] % 128));
        str1 += '\0';
    } else {
        str1 = "a\0";
    }
    
    if (Size >= 2) {
        str2 = std::string(1, static_cast<char>(Data[1 % Size] % 128));
        str2 += '\0';
    } else {
        str2 = "b\0";
    }
    
    if (Size >= 3) {
        str3 = std::string(1, static_cast<char>(Data[2 % Size] % 128));
        str3 += '\0';
    } else {
        str3 = "c\0";
    }
    
    // Create string views
    std::string_view sv1(str1.c_str(), 1);
    std::string_view sv2(str2.c_str(), 1);
    std::string_view sv3(str3.c_str(), 1);
    
    // Get some child nodes for the const xml_node& overload
    pugi::xml_node child1 = node.child("child1");
    pugi::xml_node child2 = node.child("child2");
    pugi::xml_node child3 = node.child("child3");
    
    // Call 1: remove_child with string_view_t
    while (node.remove_child(sv1)) {}
    
    // Call 2: remove_child with const xml_node&
    if (child1) {
        node.remove_child(child1);
    }
    
    // Call 3: child with const char*
    pugi::xml_node temp1 = node.child(str1.c_str());
    
    // Call 4: child with const char*
    pugi::xml_node temp2 = node.child(str2.c_str());
    
    // Call 5: remove_child with const char*
    while (node.remove_child(str3.c_str())) {}
    
    // Call 6: remove_child with string_view_t
    while (node.remove_child(sv2)) {}
    
    // Call 7: child with const char*
    pugi::xml_node temp3 = node.child(str3.c_str());
    
    // Call 8: remove_child with const xml_node&
    if (child2) {
        node.remove_child(child2);
    }
    
    // Call 9: child with const char*
    pugi::xml_node temp4 = node.child(str1.c_str());
    
    // Call 10: remove_child with const char*
    while (node.remove_child(str2.c_str())) {}
    
    // Call 11: remove_child with string_view_t
    while (node.remove_child(sv3)) {}
    
    // Call 12: remove_child with const xml_node&
    if (child3) {
        node.remove_child(child3);
    }
    
    // Call 13: remove_child with const char*
    while (node.remove_child(str1.c_str())) {}
    
    // Call 14: remove_child with string_view_t
    while (node.remove_child(sv1)) {}
    
    // Call 15: remove_child with const xml_node&
    // Get another child if available
    pugi::xml_node child4 = node.child("child1");
    if (child4) {
        node.remove_child(child4);
    }
    
    return 0;
}