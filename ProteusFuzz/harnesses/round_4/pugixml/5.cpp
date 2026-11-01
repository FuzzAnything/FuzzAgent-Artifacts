// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

static bool safe_load_string(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) return false;
    
    // Ensure null termination
    std::string str(reinterpret_cast<const char*>(data), size);
    str.push_back('\0');
    
    doc.load_string(str.c_str());
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // 1. pugi::xml_document::load_string
    safe_load_string(doc, Data, Size);
    
    // Get root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root, try to use first child of document
        root = doc.first_child();
    }
    if (!root) return 0;

    // 2. pugi::xml_node::child
    pugi::xml_node child1 = root.child("child1");
    if (!child1) {
        // Try alternative names if first fails
        child1 = root.child("item");
        if (!child1) {
            child1 = root.first_child();
        }
    }
    if (!child1) return 0;

    // 3. pugi::xml_node::child
    pugi::xml_node child2 = child1.child("child2");
    if (!child2) {
        child2 = child1.child("item");
        if (!child2) {
            child2 = child1.first_child();
        }
    }
    if (!child2) return 0;

    // 4. pugi::xml_node::set_name
    const char* new_name = "renamed_node";
    child2.set_name(new_name);

    // 5. pugi::xml_node::attribute
    pugi::xml_attribute attr1 = child2.attribute("attr1");
    if (!attr1) {
        attr1 = child2.attribute("id");
        if (!attr1) {
            attr1 = child2.first_attribute();
        }
    }
    if (!attr1) return 0;

    // 6. pugi::xml_attribute::set_name
    const char* new_attr_name = "new_attr_name";
    attr1.set_name(new_attr_name);

    // 7. pugi::xml_node::attribute
    pugi::xml_attribute attr2 = child2.attribute("attr2");
    if (!attr2) {
        attr2 = child2.attribute("value");
        if (!attr2) {
            attr2 = child2.attribute("id");
            if (!attr2) {
                attr2 = child2.first_attribute();
            }
        }
    }
    if (!attr2) return 0;

    // 8. pugi::xml_attribute::set_value
    const char* new_value = "new_value";
    attr2.set_value(new_value);

    // 9. pugi::xml_node::child
    pugi::xml_node child3 = child2.child("child3");
    if (!child3) {
        child3 = child2.child("item");
        if (!child3) {
            child3 = child2.first_child();
        }
    }
    if (!child3) return 0;

    // 10. pugi::xml_node::text
    pugi::xml_text text_obj = child3.text();
    if (!text_obj) return 0;

    // 11. pugi::xml_text::set
    const char* text_content = "text_content";
    text_obj.set(text_content);

    // 12. pugi::xml_node::append_copy
    pugi::xml_node copied_node1 = child3.append_copy(child2);
    if (!copied_node1) return 0;

    // 13. pugi::xml_node::child
    pugi::xml_node child4 = copied_node1.child("child4");
    if (!child4) {
        child4 = copied_node1.child("item");
        if (!child4) {
            child4 = copied_node1.first_child();
        }
    }
    if (!child4) return 0;

    // 14. pugi::xml_node::append_copy
    pugi::xml_node copied_node2 = child4.append_copy(child3);
    (void)copied_node2; // Suppress unused warning

    return 0;
}