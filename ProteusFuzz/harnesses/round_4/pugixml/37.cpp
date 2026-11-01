// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
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

static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;
    
    // Ensure null termination for string conversion
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Add null terminator if not present
    if (!xml_str.empty() && xml_str.back() != '\0') {
        xml_str += '\0';
    }
    
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    if (!result) {
        delete doc;
        return nullptr;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) return 0;

    try {
        pugi::xml_node root = doc->first_child();
        
        // 1. pugi::xml_node::first_child
        pugi::xml_node child = root.first_child();
        
        if (child) {
            // Prepare hint for attribute lookup
            pugi::xml_attribute hint;
            
            // 2. pugi::xml_node::attribute (const char_t* name)
            pugi::xml_attribute attr1 = child.attribute("id");
            
            // 3. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
            std::string_view sv1("class");
            pugi::xml_attribute attr2 = child.attribute(sv1, hint);
            
            // 4. pugi::xml_node::attribute (const char_t* name)
            pugi::xml_attribute attr3 = child.attribute("name");
            
            // 5. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
            std::string_view sv2("value");
            pugi::xml_attribute attr4 = child.attribute(sv2, hint);
            
            // 6. pugi::xml_node::attribute (const char_t* name)
            pugi::xml_attribute attr5 = child.attribute("type");
            
            // 7. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
            std::string_view sv3("data");
            pugi::xml_attribute attr6 = child.attribute(sv3, hint);
            
            // 8. pugi::xml_node::attribute (const char_t* name)
            pugi::xml_attribute attr7 = child.attribute("ref");
            
            // 9. pugi::xml_attribute::set_name
            if (attr7) {
                std::string_view new_name("new_ref");
                attr7.set_name(new_name);
            }
            
            // 10. pugi::xml_node::attribute (const char_t* name)
            pugi::xml_attribute attr8 = child.attribute("new_ref");
            
            // 11. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
            std::string_view sv4("test");
            pugi::xml_attribute attr9 = child.attribute(sv4, hint);
            
            // 12. pugi::xml_node::attribute (const char_t* name)
            pugi::xml_attribute attr10 = child.attribute("foo");
            
            // 13. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
            std::string_view sv5("bar");
            pugi::xml_attribute attr11 = child.attribute(sv5, hint);
        }
    } catch (...) {
        // Ignore exceptions during fuzzing
    }

    delete doc;
    return 0;
}