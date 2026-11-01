// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
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

static std::string ensure_null_terminated(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    
    size_t null_pos = 0;
    while (null_pos < size && data[null_pos] != '\0') {
        null_pos++;
    }
    
    if (null_pos == size) {
        return "";
    }
    
    return std::string(reinterpret_cast<const char*>(data), null_pos);
}

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    if (size == 0) {
        return doc;
    }
    
    std::string content(reinterpret_cast<const char*>(data), size);
    
    std::string null_terminated = content;
    if (!null_terminated.empty() && null_terminated.back() != '\0') {
        null_terminated += '\0';
    }
    
    pugi::xml_parse_result result = doc.load_string(null_terminated.c_str());
    
    if (result.status != pugi::status_ok) {
        doc.reset();
        pugi::xml_node root = doc.append_child(pugi::node_element);
        if (root) {
            root.append_child(pugi::node_pcdata); // Just ensure a valid structure
        }
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    pugi::xml_document doc = create_document_from_input(Data, Size);
    
    if (!doc) {
        return 0;
    }
    
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child(pugi::node_element);
        if (!root) {
            return 0;
        }
        // Set name using append_child with name string, as name() is const getter
        // We create a child and set its name, or just use the element created
        // To set name on the root element itself, we rely on the fact that append_child(node_element) 
        // creates an element, but we need to name it. 
        // Actually, append_child(const char_t* name) creates and names it.
        // Let's re-create root properly if needed.
        root = doc.append_child("root");
        if (!root) {
            return 0;
        }
    }
    
    // Test 1: append_child with different node types
    pugi::xml_node_type types[] = {
        pugi::node_element,
        pugi::node_pcdata,
        pugi::node_cdata,
        pugi::node_comment,
        pugi::node_pi,
        pugi::node_declaration,
        pugi::node_doctype
    };
    
    for (int i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
        pugi::xml_node child = root.append_child(types[i]);
        if (child) {
            pugi::xml_node_type node_type = child.type();
            (void)node_type;
            
            child.remove_attributes();
            child.remove_children();
        }
    }
    
    // Test 2: append_child with name (element only)
    std::string name_str = ensure_null_terminated(Data, Size);
    if (!name_str.empty()) {
        pugi::xml_node named_child = root.append_child(name_str.c_str());
        if (named_child) {
            pugi::xml_node_type node_type = named_child.type();
            (void)node_type;
            named_child.remove_attributes();
            named_child.remove_children();
        }
    }
    
    // Test 3: append_buffer with input data
    if (Size > 0) {
        pugi::xml_parse_result buffer_result = root.append_buffer(Data, Size);
        (void)buffer_result;
        
        if (buffer_result.status == pugi::status_ok) {
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                pugi::xml_node_type node_type = child.type();
                (void)node_type;
                child.remove_attributes();
                child.remove_children();
            }
        }
    }
    
    // Test 4: print to various outputs
    std::ostringstream oss;
    pugi::xml_writer_stream writer_stream(oss);
    root.print(writer_stream);
    
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        pugi::xml_writer_file writer_file(file);
        root.print(writer_file);
        fclose(file);
    }
    
    // Test 5: Test on document node itself
    pugi::xml_node_type doc_type = doc.type();
    (void)doc_type;
    doc.remove_attributes();
    doc.remove_children();
    
    // Test 6: Test on null node
    pugi::xml_node null_node;
    pugi::xml_node_type null_type = null_node.type();
    (void)null_type;
    null_node.remove_attributes();
    null_node.remove_children();
    
    return 0;
}