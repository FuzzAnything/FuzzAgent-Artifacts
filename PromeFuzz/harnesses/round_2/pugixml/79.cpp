// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
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

static void test_xml_node_apis(pugi::xml_node& node, const uint8_t* data, size_t size) {
    // Test name() function
    const char* node_name = node.name();
    (void)node_name;
    
    // Test value() function
    const char* node_value = node.value();
    (void)node_value;
    
    // Test type() function
    pugi::xml_node_type node_type = node.type();
    (void)node_type;
    
    // Test set_value overloads
    if (size > 0) {
        // Create null-terminated string for set_value(const char*)
        char* null_term_str = new char[size + 1];
        std::memcpy(null_term_str, data, size);
        null_term_str[size] = '\0';
        
        // Test set_value(const char*)
        bool set_value_result1 = node.set_value(null_term_str);
        (void)set_value_result1;
        
        // Test set_value(const char*, size_t)
        bool set_value_result2 = node.set_value(null_term_str, size);
        (void)set_value_result2;
        
        delete[] null_term_str;
    }
    
    // Test set_name function
    if (size > 0) {
        // Create string_view for set_name
        std::string_view str_view(reinterpret_cast<const char*>(data), size);
        
        // Test set_name(const char*, size_t)
        bool set_name_result = node.set_name(str_view.data(), str_view.size());
        (void)set_name_result;
    }
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_ws_pcdata);
    
    if (result) {
        // If parsing succeeded, test APIs on all nodes in the document
        pugi::xml_node root = doc.document_element();
        
        // Test on document node
        test_xml_node_apis(doc, Data, Size);
        
        // Test on root element if exists
        if (root) {
            test_xml_node_apis(root, Data, Size);
            
            // Test on all child nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                test_xml_node_apis(child, Data, Size);
            }
            
            // Test on all attributes (attributes are not nodes, so we skip them)
            // We'll test the attribute values through the attribute objects
            for (pugi::xml_attribute attr = root.first_attribute(); attr; attr = attr.next_attribute()) {
                // Test attribute name and value getters (these are xml_attribute methods, not xml_node)
                const char* attr_name = attr.name();
                const char* attr_value = attr.value();
                (void)attr_name;
                (void)attr_value;
            }
        }
    } else {
        // If parsing failed, create a simple document with different node types
        // and test the APIs on them
        
        // Create document node
        test_xml_node_apis(doc, Data, Size);
        
        // Create element node
        pugi::xml_node elem = doc.append_child(pugi::node_element);
        test_xml_node_apis(elem, Data, Size);
        
        // Create text node (PCDATA)
        pugi::xml_node text = doc.append_child(pugi::node_pcdata);
        test_xml_node_apis(text, Data, Size);
        
        // Create comment node
        pugi::xml_node comment = doc.append_child(pugi::node_comment);
        test_xml_node_apis(comment, Data, Size);
        
        // Create processing instruction node
        pugi::xml_node pi = doc.append_child(pugi::node_pi);
        test_xml_node_apis(pi, Data, Size);
        
        // Create CDATA node
        pugi::xml_node cdata = doc.append_child(pugi::node_cdata);
        test_xml_node_apis(cdata, Data, Size);
        
        // Create doctype node
        pugi::xml_node doctype = doc.append_child(pugi::node_doctype);
        test_xml_node_apis(doctype, Data, Size);
    }
    
    return 0;
}