// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
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

// Helper to create a valid xml_document and populate it with nodes for testing
static pugi::xml_document create_test_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try to parse the input data as XML
    if (size > 0) {
        std::string xml_str(reinterpret_cast<const char*>(data), size);
        // Ensure null termination for safety
        std::vector<char> buffer(xml_str.begin(), xml_str.end());
        buffer.push_back('\0');
        
        pugi::xml_parse_result result = doc.load_string(buffer.data());
        if (!result) {
            // If parsing fails, create a simple document structure manually
            pugi::xml_node root = doc.append_child("root");
            pugi::xml_node child = root.append_child("child");
            pugi::xml_node text_node = child.append_child("text");
            text_node.set_value("initial text");
            
            // Create comment and PI nodes manually as append_child doesn't support type+value directly
            pugi::xml_node comment_node = child.append_child(pugi::node_comment);
            comment_node.set_value("test");
            
            pugi::xml_node pi_node = child.append_child(pugi::node_pi);
            pi_node.set_name("target");
            pi_node.set_value("target");
        }
    } else {
        // Create a minimal document if input is empty
        pugi::xml_node root = doc.append_child("root");
        pugi::xml_node child = root.append_child("child");
        pugi::xml_node text_node = child.append_child("text");
        text_node.set_value("initial text");
        
        pugi::xml_node comment_node = child.append_child(pugi::node_comment);
        comment_node.set_value("test");
        
        pugi::xml_node pi_node = child.append_child(pugi::node_pi);
        pi_node.set_name("target");
        pi_node.set_value("target");
    }
    
    return doc;
}

static void test_set_value_overloads(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Prepare a null-terminated C-string for set_value(const char_t*)
    std::vector<pugi::char_t> buffer(size + 1);
    if (sizeof(pugi::char_t) == 1) {
        std::memcpy(buffer.data(), data, size);
        buffer[size] = 0;
    } else {
        // For wide mode, copy carefully ensuring we don't read past bounds
        size_t wide_chars = size / sizeof(wchar_t);
        if (wide_chars > 0) {
            std::memcpy(buffer.data(), data, wide_chars * sizeof(wchar_t));
            buffer[wide_chars] = 0;
        } else {
            buffer[0] = 0;
        }
    }
    
    // Invoke set_value(const char_t*)
    node.set_value(buffer.data());
    
    // Invoke set_value(string_view_t)
    // string_view_t is std::basic_string_view<pugi::char_t>
    if (sizeof(pugi::char_t) == 1) {
        std::basic_string_view<pugi::char_t> sv(reinterpret_cast<const pugi::char_t*>(data), size);
        node.set_value(sv);
    } else {
        size_t wide_chars = size / sizeof(wchar_t);
        if (wide_chars > 0) {
            std::basic_string_view<pugi::char_t> sv(reinterpret_cast<const pugi::char_t*>(data), wide_chars);
            node.set_value(sv);
        }
    }
}

static void test_as_utf8_overloads(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Test as_utf8(const wchar_t*)
    // We need to ensure we have valid wide characters
    if (size >= sizeof(wchar_t)) {
        size_t wide_count = size / sizeof(wchar_t);
        if (wide_count > 0) {
            const wchar_t* wide_ptr = reinterpret_cast<const wchar_t*>(data);
            // Create a temporary wide string to ensure validity
            std::wstring ws(wide_ptr, wide_count);
            std::string result1 = pugi::as_utf8(ws);
            (void)result1;
            
            // Also test with pointer to the wide string
            std::string result2 = pugi::as_utf8(ws.c_str());
            (void)result2;
        }
    }
    
    // Test as_utf8(const std::wstring&)
    if (size >= sizeof(wchar_t)) {
        size_t wide_count = size / sizeof(wchar_t);
        if (wide_count > 0) {
            std::wstring ws(reinterpret_cast<const wchar_t*>(data), wide_count);
            std::string result = pugi::as_utf8(ws);
            (void)result;
        }
    }
}

static void test_as_wide_overloads(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Test as_wide(const char*)
    // Ensure null termination
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = 0;
    std::wstring result1 = pugi::as_wide(buffer.data());
    (void)result1;
    
    // Test as_wide(const std::string&)
    std::string ns(reinterpret_cast<const char*>(data), size);
    std::wstring result2 = pugi::as_wide(ns);
    (void)result2;
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a document to get valid nodes
    pugi::xml_document doc = create_test_document(Data, Size);
    
    // Test set_value on various node types
    // Root
    test_set_value_overloads(doc, Data, Size);
    
    // First child
    if (doc.first_child()) {
        test_set_value_overloads(doc.first_child(), Data, Size);
    }
    
    // Iterate through all nodes to maximize coverage
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        test_set_value_overloads(node, Data, Size);
        
        // Also test children
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            test_set_value_overloads(child, Data, Size);
        }
    }
    
    // Test as_utf8 overloads
    test_as_utf8_overloads(Data, Size);
    
    // Test as_wide overloads
    test_as_wide_overloads(Data, Size);
    
    return 0;
}