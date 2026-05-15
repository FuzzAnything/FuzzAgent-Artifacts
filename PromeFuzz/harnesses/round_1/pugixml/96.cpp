// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <stdint.h>
#include <stddef.h>
#include <string>

static pugi::xml_document create_test_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size > 0) {
        std::string xml_content(reinterpret_cast<const char*>(data), size);
        doc.load_string(xml_content.c_str());
    } else {
        doc.append_child(pugi::node_element);
    }
    return doc;
}

static void test_node_functions(pugi::xml_node node) {
    // Test parent()
    pugi::xml_node parent = node.parent();
    (void)parent;
    
    // Test first_child()
    pugi::xml_node first_child = node.first_child();
    (void)first_child;
    
    // Test offset_debug()
    ptrdiff_t offset = node.offset_debug();
    (void)offset;
    
    // Test type()
    pugi::xml_node_type node_type = node.type();
    (void)node_type;
    
    // Test append_child()
    pugi::xml_node appended = node.append_child();
    (void)appended;
    
    // Test last_child()
    pugi::xml_node last_child = node.last_child();
    (void)last_child;
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* Data, size_t Size) {
    // Create test document from fuzzer input
    pugi::xml_document doc = create_test_document(Data, Size);
    
    // Test functions on document node
    test_node_functions(doc);
    
    // Test functions on root element if it exists
    pugi::xml_node root = doc.document_element();
    if (root) {
        test_node_functions(root);
        
        // Test functions on first child of root
        pugi::xml_node first_child = root.first_child();
        if (first_child) {
            test_node_functions(first_child);
        }
        
        // Test functions on last child of root
        pugi::xml_node last_child = root.last_child();
        if (last_child) {
            test_node_functions(last_child);
        }
    }
    
    // Test functions on empty node
    pugi::xml_node empty_node;
    test_node_functions(empty_node);
    
    // Test with file if size is non-zero
    if (Size > 0) {
        FILE* file = fopen("./dummy_file", "wb");
        if (file) {
            fwrite(Data, 1, Size, file);
            fclose(file);
            
            pugi::xml_document file_doc;
            file_doc.load_file("./dummy_file");
            
            pugi::xml_node file_root = file_doc.document_element();
            if (file_root) {
                test_node_functions(file_root);
            }
        }
    }
    
    return 0;
}