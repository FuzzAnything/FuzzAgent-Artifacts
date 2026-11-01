// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include <cstddef>
#include <string_view>

static pugi::xml_document* create_document(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;
    
    // Try to parse the input data as XML
    if (size > 0) {
        // Create a null-terminated string for parsing
        std::string xml_str(reinterpret_cast<const char*>(data), size);
        // Ensure null termination
        xml_str += '\0';
        
        pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
        if (!result) {
            // If parsing fails, create a minimal valid document
            doc->reset();
            pugi::xml_node root = doc->append_child("root");
            if (!root) {
                delete doc;
                return nullptr;
            }
        }
    } else {
        // Empty input, create minimal document
        doc->reset();
        pugi::xml_node root = doc->append_child("root");
        if (!root) {
            delete doc;
            return nullptr;
        }
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    pugi::xml_document* doc = create_document(Data, Size);
    if (!doc) {
        return 0;
    }
    
    try {
        // Step 1: pugi::xml_node::child
        pugi::xml_node root = doc->child("root");
        if (!root) {
            // If no root found, try to use the document itself as root
            root = *doc;
        }
        
        if (!root) {
            delete doc;
            return 0;
        }
        
        // Try to find a child node
        pugi::xml_node child_node = root.child("child");
        
        // Step 2: pugi::xml_node::append_child (first call)
        pugi::xml_node node1 = child_node.empty() ? root : child_node;
        pugi::xml_node appended1 = node1.append_child("element1");
        
        // Step 3: pugi::xml_node::append_child (second call)
        pugi::xml_node appended2 = node1.append_child("element2");
        
        // Step 4: pugi::xml_text::set (unsigned long overload)
        if (appended1) {
            pugi::xml_text text1 = appended1.text();
            if (text1) {
                text1.set(static_cast<unsigned long>(Size));
            }
        }
        
        // Step 5: pugi::xml_node::append_child (third call)
        pugi::xml_node appended3 = node1.append_child("element3");
        
        // Step 6: pugi::xml_node::append_child (fourth call)
        pugi::xml_node appended4 = node1.append_child("element4");
        
        // Step 7: pugi::xml_text::set (long overload)
        if (appended2) {
            pugi::xml_text text2 = appended2.text();
            if (text2) {
                text2.set(static_cast<long>(Size));
            }
        }
        
        // Additional exploration: try setting text on other nodes
        if (appended3) {
            pugi::xml_text text3 = appended3.text();
            if (text3) {
                text3.set(static_cast<unsigned long>(Size % 1000));
            }
        }
        
        if (appended4) {
            pugi::xml_text text4 = appended4.text();
            if (text4) {
                text4.set(static_cast<long>(Size % 1000));
            }
        }
        
        // Try to access child again to explore more states
        pugi::xml_node found_child = root.child("element1");
        if (found_child) {
            pugi::xml_text text_found = found_child.text();
            if (text_found) {
                text_found.set(static_cast<unsigned long>(Size));
            }
        }
        
    } catch (...) {
        // Handle any exceptions
    }
    
    delete doc;
    return 0;
}