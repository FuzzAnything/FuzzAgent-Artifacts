// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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

static pugi::xml_node create_test_node(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    // Try to parse the input as XML first
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    if (result && doc.first_child()) {
        // Return the first child if parsing succeeded
        return doc.first_child();
    } else {
        // Otherwise create a dummy element node
        doc.reset();
        return doc.append_child(pugi::node_element);
    }
}

static void test_node_functions(pugi::xml_node& node, const uint8_t* data, size_t size) {
    // Test name() function
    const char* node_name = node.name();
    (void)node_name; // Use the result to avoid unused variable warning
    
    // Test value() function
    const char* node_value = node.value();
    (void)node_value;
    
    // Test type() function
    pugi::xml_node_type node_type = node.type();
    (void)node_type;
    
    // Test set_value overloads if we have enough data
    if (size > 0) {
        // For set_value(const char_t* rhs, size_t size)
        // Use the input data directly as the value
        bool set_result1 = node.set_value(reinterpret_cast<const char*>(data), 
                                          size > 1024 ? 1024 : size); // Limit size
        (void)set_result1;
        
        // For set_value(const char_t* rhs)
        // Need null-terminated string, so create a temporary buffer
        if (size > 0) {
            // Create a buffer with null terminator
            size_t copy_size = size > 256 ? 256 : size;
            char* temp_buf = new char[copy_size + 1];
            std::memcpy(temp_buf, data, copy_size);
            temp_buf[copy_size] = '\0';
            
            bool set_result2 = node.set_value(temp_buf);
            (void)set_result2;
            
            delete[] temp_buf;
        }
    }
    
    // Test set_name if we have enough data
    if (size > 0) {
        // Use a substring of the input data for the name
        size_t name_size = size > 128 ? 128 : size;
        bool name_result = node.set_name(reinterpret_cast<const char*>(data), name_size);
        (void)name_result;
    }
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* data, size_t size) {
    static pugi::xml_document doc;
    
    // Reset document for each iteration
    doc.reset();
    
    // Create a test node using the fuzzer input
    pugi::xml_node test_node = create_test_node(doc, data, size);
    
    // Test all the target functions on this node
    test_node_functions(test_node, data, size);
    
    // Also test on child nodes if they exist
    for (pugi::xml_node child = test_node.first_child(); child; child = child.next_sibling()) {
        test_node_functions(child, data, size);
    }
    
    // Test on different node types by creating them
    if (size > 0) {
        // Create a comment node
        pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
        test_node_functions(comment_node, data, size);
        
        // Create a processing instruction node
        pugi::xml_node pi_node = doc.append_child(pugi::node_pi);
        test_node_functions(pi_node, data, size);
        
        // Create a CDATA node
        pugi::xml_node cdata_node = doc.append_child(pugi::node_cdata);
        test_node_functions(cdata_node, data, size);
        
        // Create a PCDATA node
        pugi::xml_node pcdata_node = doc.append_child(pugi::node_pcdata);
        test_node_functions(pcdata_node, data, size);
    }
    
    return 0;
}