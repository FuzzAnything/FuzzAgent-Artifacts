// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <cstddef>
#include <string>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Try to parse the input as XML
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    
    // If parsing fails, create a minimal valid XML structure
    if (!result) {
        doc->reset();
        pugi::xml_node root = doc->append_child("root");
        if (!root) {
            delete doc;
            return nullptr;
        }
        root.append_child("child");
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    try {
        pugi::xml_node root = doc->first_child();
        if (!root) {
            // If no children, create one
            root = doc->append_child("root");
        }

        // 1. pugi::xml_node::child
        std::string child_name = "test";
        pugi::xml_node child_node = root.child(child_name.c_str());
        
        if (child_node) {
            // 2. pugi::xml_node::text
            pugi::xml_text text_node = child_node.text();
            
            // 3. pugi::xml_text::set
            std::string text_value = "initial";
            text_node.set(text_value.c_str(), text_value.size());
            
            // 4. pugi::xml_node::first_child
            pugi::xml_node first_child = child_node.first_child();
            
            if (first_child) {
                // 5. pugi::xml_text::set (on first child's text)
                pugi::xml_text first_text = first_child.text();
                first_text.set("first_child_text", 16);
            }
            
            // 6. pugi::xml_node::first_child (again)
            first_child = child_node.first_child();
            
            // 7. pugi::xml_node::last_child
            pugi::xml_node last_child = child_node.last_child();
            
            if (last_child) {
                // 8. pugi::xml_text::set (on last child's text)
                pugi::xml_text last_text = last_child.text();
                last_text.set("last_child_text", 15);
            }
            
            // 9. pugi::xml_node::first_child (again)
            first_child = child_node.first_child();
            
            // 10. pugi::xml_node::last_child (again)
            last_child = child_node.last_child();
            
            if (last_child) {
                // 11. pugi::xml_text::set (on last child's text again)
                pugi::xml_text last_text2 = last_child.text();
                last_text2.set("updated_last", 12);
            }
            
            // 12. pugi::xml_node::first_child (final)
            first_child = child_node.first_child();
        }
    } catch (...) {
        // Handle any exceptions gracefully
    }

    delete doc;
    return 0;
}