// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
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

// Helper to safely get a node from the document based on input data
static pugi::xml_node get_node_from_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size < 2) return pugi::xml_node();
    
    // Try to parse the input as an XML document
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    if (!result) return pugi::xml_node();

    // Return the first child element if it exists, otherwise return document node
    pugi::xml_node root = doc.first_child();
    return root.empty() ? doc : root;
}

// Helper to create a simple document for testing append_buffer
static pugi::xml_document create_base_document() {
    pugi::xml_document doc;
    doc.append_child(pugi::node_element).set_name("root");
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test 1: pugi::xml_node::child_value (both overloads)
    {
        pugi::xml_document doc;
        // Try parsing the input as XML
        doc.load_buffer(Data, Size);
        
        pugi::xml_node node = get_node_from_document(doc, Data, Size);
        
        // Call child_value() without arguments
        const pugi::char_t* val1 = node.child_value();
        (void)val1; // Suppress unused variable warning
        
        // Call child_value(name) with a name derived from input
        if (Size > 0) {
            // Create a safe name from input (ensure null termination)
            char safe_name[32];
            size_t name_len = (Size < 31) ? Size : 31;
            std::memcpy(safe_name, Data, name_len);
            safe_name[name_len] = '\0';
            
            // Convert to wide char if necessary (pugixml handles this internally)
            const pugi::char_t* name = reinterpret_cast<const pugi::char_t*>(safe_name);
            const pugi::char_t* val2 = node.child_value(name);
            (void)val2;
        }
    }

    // Test 2: pugi::xml_node::print (both overloads)
    {
        pugi::xml_document doc;
        doc.load_buffer(Data, Size);
        
        pugi::xml_node node = get_node_from_document(doc, Data, Size);
        
        // Overload 1: Print to std::basic_ostream
        std::ostringstream oss;
        node.print(oss, "\t", pugi::format_default, pugi::encoding_auto, 0);
        
        // Overload 2: Print to xml_writer_file
        // Create a temporary file for writing
        FILE* f = fopen("./dummy_file", "wb");
        if (f) {
            pugi::xml_writer_file writer(f);
            node.print(writer, "\t", pugi::format_default, pugi::encoding_auto, 0);
            fclose(f);
        }
        
        // Overload 3: Print to xml_writer_stream (alternative stream writer)
        std::ostringstream oss2;
        pugi::xml_writer_stream writer2(oss2);
        node.print(writer2, "\t", pugi::format_default, pugi::encoding_auto, 0);
    }

    // Test 3: pugi::xml_node::append_buffer
    {
        pugi::xml_document doc = create_base_document();
        pugi::xml_node root = doc.child("root");
        
        // Append buffer with various options
        // Default options
        pugi::xml_parse_result res1 = root.append_buffer(Data, Size);
        (void)res1;
        
        // With parse_embed_pcdata
        pugi::xml_parse_result res2 = root.append_buffer(Data, Size, pugi::parse_embed_pcdata);
        (void)res2;
        
        // With different encodings
        pugi::xml_parse_result res3 = root.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_utf8);
        (void)res3;
        
        pugi::xml_parse_result res4 = root.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_latin1);
        (void)res4;
    }

    // Test 4: pugi::xml_node::name
    {
        pugi::xml_document doc;
        doc.load_buffer(Data, Size);
        
        pugi::xml_node node = get_node_from_document(doc, Data, Size);
        
        // Get node name
        const pugi::char_t* name = node.name();
        (void)name;
        
        // Also test on an empty node
        pugi::xml_node empty_node;
        const pugi::char_t* empty_name = empty_node.name();
        (void)empty_name;
    }

    return 0;
}