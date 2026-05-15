// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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

static pugi::xml_document create_dummy_document() {
    pugi::xml_document doc;
    // Create a simple XML structure for testing
    auto root = doc.append_child("root");
    root.append_child("child1").append_attribute("attr1").set_value("value1");
    root.append_child("child2").text().set("text2");
    root.append_child("child3").append_child("grandchild");
    root.append_attribute("rootattr").set_value("rootvalue");
    return doc;
}

static pugi::xml_document parse_xml_from_data(const uint8_t *data, size_t size) {
    pugi::xml_document doc;
    // Try to parse the fuzzer input as XML
    doc.load_buffer(data, size, pugi::parse_default | pugi::parse_fragment);
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create XML documents for testing
    pugi::xml_document dummy_doc = create_dummy_document();
    pugi::xml_document parsed_doc = parse_xml_from_data(Data, Size);
    
    // Create context nodes
    pugi::xpath_node dummy_node(dummy_doc.root());
    pugi::xpath_node parsed_node(parsed_doc.root());
    
    // Create XPath query from fuzzer input
    std::string xpath_str(reinterpret_cast<const char*>(Data), Size);
    
    try {
        pugi::xpath_query query(xpath_str.c_str());
        
        if (query) {
            // Test evaluate_string
            static_cast<void>(query.evaluate_string(dummy_node));
            static_cast<void>(query.evaluate_string(parsed_node));
            
            // Test evaluate_boolean
            static_cast<void>(query.evaluate_boolean(dummy_node));
            static_cast<void>(query.evaluate_boolean(parsed_node));
            
            // Test evaluate_number
            static_cast<void>(query.evaluate_number(dummy_node));
            static_cast<void>(query.evaluate_number(parsed_node));
            
            // Test evaluate_node
            static_cast<void>(query.evaluate_node(dummy_node));
            static_cast<void>(query.evaluate_node(parsed_node));
            
            // Test select_nodes
            static_cast<void>(dummy_doc.root().select_nodes(query));
            static_cast<void>(parsed_doc.root().select_nodes(query));
            
            // Test evaluate_node_set
            static_cast<void>(query.evaluate_node_set(dummy_node));
            static_cast<void>(query.evaluate_node_set(parsed_node));
        }
    } catch (const std::bad_alloc&) {
        // Memory allocation error - ignore in fuzzing
    } catch (...) {
        // Catch any other exception (including xpath_exception if defined)
    }
    
    // Test with empty/default constructed objects
    pugi::xpath_query empty_query;
    pugi::xml_node empty_node;
    pugi::xpath_node empty_xpath_node;
    
    if (empty_query) {
        static_cast<void>(empty_query.evaluate_string(empty_xpath_node));
        static_cast<void>(empty_query.evaluate_boolean(empty_xpath_node));
        static_cast<void>(empty_query.evaluate_number(empty_xpath_node));
        static_cast<void>(empty_query.evaluate_node(empty_xpath_node));
        static_cast<void>(empty_node.select_nodes(empty_query));
        static_cast<void>(empty_query.evaluate_node_set(empty_xpath_node));
    }
    
    // Test with different node types (element, attribute)
    if (!dummy_doc.empty()) {
        auto root = dummy_doc.root();
        if (root.first_attribute()) {
            // Correct constructor order: attribute first, then parent node
            pugi::xpath_node attr_node(root.first_attribute(), root);
            pugi::xpath_query attr_query(xpath_str.c_str());
            if (attr_query) {
                static_cast<void>(attr_query.evaluate_string(attr_node));
                static_cast<void>(attr_query.evaluate_boolean(attr_node));
                static_cast<void>(attr_query.evaluate_number(attr_node));
                static_cast<void>(attr_query.evaluate_node(attr_node));
                static_cast<void>(attr_query.evaluate_node_set(attr_node));
            }
        }
    }
    
    return 0;
}