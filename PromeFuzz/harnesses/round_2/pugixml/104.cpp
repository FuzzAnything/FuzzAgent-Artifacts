// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>

static void create_dummy_xml(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    // Create a simple XML structure with the fuzzed data embedded
    auto root = doc.append_child("root");
    auto child1 = root.append_child("child1");
    child1.append_attribute("attr1").set_value("value1");
    
    // Add text content from fuzzed data (limited to avoid huge allocations)
    size_t text_size = size > 100 ? 100 : size;
    std::string text_content(reinterpret_cast<const char*>(data), text_size);
    child1.text().set(text_content.c_str());
    
    auto child2 = root.append_child("child2");
    child2.append_attribute("attr2").set_value("value2");
    
    // Add another node with more fuzzed data
    auto child3 = root.append_child("child3");
    if (size > 50) {
        std::string more_text(reinterpret_cast<const char*>(data) + 50, 
                             size - 50 > 50 ? 50 : size - 50);
        child3.text().set(more_text.c_str());
    }
}

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0; // Need at least some data
    
    // Create XML document
    pugi::xml_document doc;
    create_dummy_xml(doc, Data, Size);
    
    // Get root node for context
    pugi::xml_node root = doc.child("root");
    pugi::xpath_node context(root);
    
    // Create XPath query from fuzzed data
    // Use first part of data for XPath expression (max 256 chars)
    size_t xpath_size = Size > 256 ? 256 : Size;
    std::string xpath_str(reinterpret_cast<const char*>(Data), xpath_size);
    
    // Try to create XPath query
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(xpath_str.c_str());
    } catch (...) {
        // Query parsing failed, but we can still test with default/empty query
        // Create a simple default query
        query = pugi::xpath_query("//*");
    }
    
    // Test evaluate_string
    try {
        std::string result_str = query.evaluate_string(context);
        (void)result_str; // Use result to avoid optimization
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test evaluate_boolean
    try {
        bool result_bool = query.evaluate_boolean(context);
        (void)result_bool;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test evaluate_node_set
    try {
        pugi::xpath_node_set node_set = query.evaluate_node_set(context);
        (void)node_set;
        
        // Also test xml_node::select_nodes
        pugi::xpath_node_set selected = root.select_nodes(query);
        (void)selected;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test evaluate_number
    try {
        double result_num = query.evaluate_number(context);
        (void)result_num;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test evaluate_node
    try {
        pugi::xpath_node result_node = query.evaluate_node(context);
        (void)result_node;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test with different context nodes
    if (root.first_child()) {
        pugi::xpath_node child_context(root.first_child());
        
        // Test all functions again with child context
        try { (void)query.evaluate_string(child_context); } catch (...) {}
        try { (void)query.evaluate_boolean(child_context); } catch (...) {}
        try { (void)query.evaluate_node_set(child_context); } catch (...) {}
        try { (void)query.evaluate_number(child_context); } catch (...) {}
        try { (void)query.evaluate_node(child_context); } catch (...) {}
    }
    
    // Test with empty context
    pugi::xpath_node empty_context;
    try { (void)query.evaluate_string(empty_context); } catch (...) {}
    try { (void)query.evaluate_boolean(empty_context); } catch (...) {}
    try { (void)query.evaluate_node_set(empty_context); } catch (...) {}
    try { (void)query.evaluate_number(empty_context); } catch (...) {}
    try { (void)query.evaluate_node(empty_context); } catch (...) {}
    
    // Test with attribute context if available
    if (root.first_child().first_attribute()) {
        pugi::xml_attribute attr = root.first_child().first_attribute();
        pugi::xml_node parent_node = root.first_child();
        // Fix: xpath_node constructor takes attribute first, then parent node
        pugi::xpath_node attr_context(attr, parent_node);
        
        try { (void)query.evaluate_string(attr_context); } catch (...) {}
        try { (void)query.evaluate_boolean(attr_context); } catch (...) {}
        try { (void)query.evaluate_node_set(attr_context); } catch (...) {}
        try { (void)query.evaluate_number(attr_context); } catch (...) {}
        try { (void)query.evaluate_node(attr_context); } catch (...) {}
    }
    
    // Create additional XPath queries with different parts of fuzzed data
    if (Size > 100) {
        // Use middle part for another query
        size_t offset = Size / 3;
        size_t length = Size / 3 > 256 ? 256 : Size / 3;
        if (offset + length <= Size) {
            std::string xpath_str2(reinterpret_cast<const char*>(Data + offset), length);
            try {
                pugi::xpath_query query2(xpath_str2.c_str());
                
                // Test the new query
                try { (void)query2.evaluate_string(context); } catch (...) {}
                try { (void)query2.evaluate_boolean(context); } catch (...) {}
                try { (void)query2.evaluate_node_set(context); } catch (...) {}
                try { (void)query2.evaluate_number(context); } catch (...) {}
                try { (void)query2.evaluate_node(context); } catch (...) {}
            } catch (...) {
                // Ignore parsing failures
            }
        }
    }
    
    return 0;
}