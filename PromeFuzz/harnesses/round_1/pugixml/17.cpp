// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create XML document with some structure
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Add some attributes to work with
    root.append_attribute("attr1");
    root.append_attribute("attr2");
    root.append_attribute("attr3");
    root.append_attribute("attr4");
    root.append_attribute("attr5");
    
    // Add some child nodes
    pugi::xml_node child1 = root.append_child("child1");
    child1.append_attribute("childattr1");
    pugi::xml_node child2 = root.append_child("child2");
    child2.append_attribute("childattr2");
    
    // Create string views from fuzzer data
    std::string_view sv1, sv2, sv3, sv4, sv5, sv6, sv7, sv8, sv9, sv10, sv11, sv12, sv13;
    
    // Split input into multiple strings for different calls
    const char* data_ptr = reinterpret_cast<const char*>(Data);
    size_t offset = 0;
    
    auto get_string = [&](size_t max_len) -> std::string_view {
        if (offset >= Size) return "";
        size_t len = std::min(max_len, Size - offset);
        std::string_view result(data_ptr + offset, len);
        offset += len;
        return result;
    };
    
    // Get strings for all calls
    sv1 = get_string(10);
    sv2 = get_string(10);
    sv3 = get_string(10);
    sv4 = get_string(10);
    sv5 = get_string(10);
    sv6 = get_string(10);
    sv7 = get_string(10);
    sv8 = get_string(10);
    sv9 = get_string(10);
    sv10 = get_string(10);
    sv11 = get_string(10);
    sv12 = get_string(10);
    sv13 = get_string(10);
    
    // Call 1: remove_attribute with string_view
    root.remove_attribute(sv1);
    
    // Call 2: remove_attribute with const char*
    if (!sv2.empty()) {
        char* null_term_str = new char[sv2.size() + 1];
        std::memcpy(null_term_str, sv2.data(), sv2.size());
        null_term_str[sv2.size()] = '\0';
        root.remove_attribute(null_term_str);
        delete[] null_term_str;
    }
    
    // Call 3: child with const char*
    if (!sv3.empty()) {
        char* null_term_str = new char[sv3.size() + 1];
        std::memcpy(null_term_str, sv3.data(), sv3.size());
        null_term_str[sv3.size()] = '\0';
        root.child(null_term_str);
        delete[] null_term_str;
    }
    
    // Call 4: child with const char*
    if (!sv4.empty()) {
        char* null_term_str = new char[sv4.size() + 1];
        std::memcpy(null_term_str, sv4.data(), sv4.size());
        null_term_str[sv4.size()] = '\0';
        root.child(null_term_str);
        delete[] null_term_str;
    }
    
    // Call 5: remove_attribute with xml_attribute
    pugi::xml_attribute attr1 = root.attribute("attr1");
    root.remove_attribute(attr1);
    
    // Call 6: remove_attribute with string_view
    root.remove_attribute(sv5);
    
    // Call 7: attribute with const char*
    if (!sv6.empty()) {
        char* null_term_str = new char[sv6.size() + 1];
        std::memcpy(null_term_str, sv6.data(), sv6.size());
        null_term_str[sv6.size()] = '\0';
        root.attribute(null_term_str);
        delete[] null_term_str;
    }
    
    // Call 8: remove_attribute with xml_attribute
    pugi::xml_attribute attr2 = root.attribute("attr2");
    root.remove_attribute(attr2);
    
    // Call 9: attribute with const char*
    if (!sv7.empty()) {
        char* null_term_str = new char[sv7.size() + 1];
        std::memcpy(null_term_str, sv7.data(), sv7.size());
        null_term_str[sv7.size()] = '\0';
        root.attribute(null_term_str);
        delete[] null_term_str;
    }
    
    // Call 10: remove_attribute with const char*
    if (!sv8.empty()) {
        char* null_term_str = new char[sv8.size() + 1];
        std::memcpy(null_term_str, sv8.data(), sv8.size());
        null_term_str[sv8.size()] = '\0';
        root.remove_attribute(null_term_str);
        delete[] null_term_str;
    }
    
    // Call 11: remove_attribute with string_view
    root.remove_attribute(sv9);
    
    // Call 12: remove_attribute with xml_attribute
    pugi::xml_attribute attr3 = root.attribute("attr3");
    root.remove_attribute(attr3);
    
    // Call 13: remove_attribute with xml_attribute
    pugi::xml_attribute attr4 = root.attribute("attr4");
    root.remove_attribute(attr4);
    
    return 0;
}