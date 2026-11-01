// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <cstdint>
#include <cstring>
#include <string>

static bool parse_xml_from_fuzz_data(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) return false;
    
    // Ensure null termination for string parsing
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    
    // Try to parse as UTF-8
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    
    // Initialize document with fuzz data
    if (!parse_xml_from_fuzz_data(Data, Size, doc)) {
        // If parsing fails, try with a minimal valid XML
        doc.load_string("<root><child>test</child></root>");
    }
    
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.first_child();
    }
    
    // If no root found, create one
    if (!root) {
        root = doc.append_child("root");
        root.append_child("child").text().set("test");
    }
    
    // Step 1: pugi::xml_node::prepend_copy
    pugi::xml_node proto1 = root.child("child");
    if (!proto1) {
        proto1 = root.append_child("child");
        proto1.text().set("test");
    }
    pugi::xml_node node1 = root.prepend_copy(proto1);
    
    // Step 2: pugi::xml_node::child
    pugi::xml_node node2 = node1.child("child");
    
    // Step 3: pugi::xml_node::prepend_copy
    if (node2) {
        pugi::xml_node node3 = node1.prepend_copy(node2);
        
        // Step 4: pugi::xml_node::prepend_copy
        pugi::xml_node node4 = node1.prepend_copy(node3);
        
        // Step 5: pugi::xml_node::child
        pugi::xml_node node5 = node4.child("child");
        
        // Step 6: pugi::xml_node::prepend_copy
        if (node5) {
            pugi::xml_node node6 = node4.prepend_copy(node5);
            
            // Step 7: pugi::xml_node::child
            pugi::xml_node node7 = node6.child("child");
            
            // Step 8: pugi::xml_node::first_child
            pugi::xml_node node8 = node7.first_child();
            
            // Step 9: pugi::xml_node::value
            const pugi::char_t* val9 = node8.value();
            (void)val9; // Use value to avoid unused warning
            
            // Step 10: pugi::xml_node::child
            pugi::xml_node node10 = node8.child("child");
            
            // Step 11: pugi::xml_node::prepend_copy
            if (node10) {
                pugi::xml_node node11 = node8.prepend_copy(node10);
                
                // Step 12: pugi::xml_node::child
                pugi::xml_node node12 = node11.child("child");
                
                // Step 13: pugi::xml_node::child
                pugi::xml_node node13 = node12.child("child");
                
                // Step 14: pugi::xml_node::name
                const pugi::char_t* name14 = node13.name();
                (void)name14; // Use value to avoid unused warning
                
                // Step 15: pugi::xml_node::child
                pugi::xml_node node15 = node13.child("child");
                
                // Step 16: pugi::xml_node::child
                pugi::xml_node node16 = node15.child("child");
                
                // Step 17: pugi::xml_node::prepend_copy
                if (node16) {
                    pugi::xml_node node17 = node15.prepend_copy(node16);
                    
                    // Step 18: pugi::xml_node::child
                    pugi::xml_node node18 = node17.child("child");
                    
                    // Step 19: pugi::xml_node::first_child
                    pugi::xml_node node19 = node18.first_child();
                    
                    // Step 20: pugi::xml_node::next_sibling
                    pugi::xml_node node20 = node19.next_sibling();
                    
                    // Step 21: pugi::xml_node::value
                    const pugi::char_t* val21 = node20.value();
                    (void)val21; // Use value to avoid unused warning
                }
            }
        }
    }
    
    return 0;
}