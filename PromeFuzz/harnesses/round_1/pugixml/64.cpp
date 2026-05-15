// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
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
#include <string>
#include <string_view>

static void test_xml_node_empty() {
    // Test with default constructed (empty) node
    pugi::xml_node node1;
    bool empty1 = node1.empty();
    
    // Test with node from a document
    pugi::xml_document doc;
    pugi::xml_node node2 = doc.append_child("test");
    bool empty2 = node2.empty();
    
    // Test with invalid node
    pugi::xml_node node3;
    pugi::xml_node node4 = node3.child("nonexistent");
    bool empty3 = node4.empty();
}

static void test_xml_text_empty() {
    // Test with default constructed (empty) text
    pugi::xml_text text1;
    bool empty1 = text1.empty();
    
    // Test with text from a document
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test");
    pugi::xml_text text2 = node.append_child(pugi::node_pcdata).text();
    bool empty2 = text2.empty();
    
    // Test with invalid text
    pugi::xml_text text3;
    pugi::xml_text text4 = pugi::xml_text();
    bool empty3 = text4.empty();
}

static void test_xml_text_as_bool(const uint8_t* data, size_t size) {
    // Create document with text node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test");
    pugi::xml_text text = node.append_child(pugi::node_pcdata).text();
    
    // Set text content from fuzzer data if we have enough
    if (size > 0) {
        std::string str(reinterpret_cast<const char*>(data), size);
        text.set(str.c_str());
        
        // Test as_bool with default parameter
        bool bool1 = text.as_bool();
        
        // Test as_bool with explicit true default
        bool bool2 = text.as_bool(true);
        
        // Test as_bool with explicit false default
        bool bool3 = text.as_bool(false);
    }
    
    // Test with empty text (should return default)
    pugi::xml_text empty_text;
    bool bool4 = empty_text.as_bool();
    bool bool5 = empty_text.as_bool(true);
    bool bool6 = empty_text.as_bool(false);
}

static void test_xml_text_set(const uint8_t* data, size_t size) {
    // Create document with text node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test");
    pugi::xml_text text = node.append_child(pugi::node_pcdata).text();
    
    // Test set(bool) overload
    bool bool_val = (size > 0) ? (data[0] & 1) : false;
    bool result1 = text.set(bool_val);
    
    // Test set(string_view_t) overload
    if (size > 0) {
        std::string_view sv(reinterpret_cast<const char*>(data), size);
        bool result2 = text.set(sv);
    }
    
    // Test set(const char_t*) overload
    if (size > 0) {
        // Create null-terminated string
        std::string str(reinterpret_cast<const char*>(data), size);
        bool result3 = text.set(str.c_str());
    }
    
    // Test with empty text object (should return false)
    pugi::xml_text empty_text;
    bool result4 = empty_text.set(true);
    
    if (size > 0) {
        std::string_view sv(reinterpret_cast<const char*>(data), size);
        bool result5 = empty_text.set(sv);
        
        std::string str(reinterpret_cast<const char*>(data), size);
        bool result6 = empty_text.set(str.c_str());
    }
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* data, size_t size) {
    // Test all target API functions
    
    // 1. Test pugi::xml_node::empty
    test_xml_node_empty();
    
    // 2. Test pugi::xml_text::empty
    test_xml_text_empty();
    
    // 3. Test pugi::xml_text::as_bool
    test_xml_text_as_bool(data, size);
    
    // 4. Test pugi::xml_text::set (all 3 overloads)
    test_xml_text_set(data, size);
    
    return 0;
}