// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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

static void test_empty_functions(pugi::xml_document& doc) {
    // Test xml_node::empty
    pugi::xml_node empty_node;
    (void)empty_node.empty();
    
    pugi::xml_node doc_node = doc.document_element();
    (void)doc_node.empty();
    
    // Test xml_attribute::empty
    pugi::xml_attribute empty_attr;
    (void)empty_attr.empty();
    
    pugi::xml_attribute node_attr = doc_node.first_attribute();
    (void)node_attr.empty();
    
    // Test xml_text::empty
    pugi::xml_text empty_text;
    (void)empty_text.empty();
    
    pugi::xml_text node_text = doc_node.text();
    (void)node_text.empty();
    
    // Test xml_node::remove_attributes
    (void)empty_node.remove_attributes();
    (void)doc_node.remove_attributes();
    
    // Test xpath_node_set::empty
    pugi::xpath_node_set empty_set;
    (void)empty_set.empty();
    
    try {
        pugi::xpath_node_set non_empty_set = doc.select_nodes("//*");
        (void)non_empty_set.empty();
    } catch (...) {
        // Ignore XPath exceptions
    }
    
    // Test xml_object_range::empty
    pugi::xml_node_iterator empty_it1, empty_it2;
    pugi::xml_object_range<pugi::xml_node_iterator> empty_range(empty_it1, empty_it2);
    (void)empty_range.empty();
    
    pugi::xml_object_range<pugi::xml_node_iterator> children_range = doc_node.children();
    (void)children_range.empty();
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_trim_pcdata);
    
    // Test all empty functions regardless of parse result
    test_empty_functions(doc);
    
    return 0;
}