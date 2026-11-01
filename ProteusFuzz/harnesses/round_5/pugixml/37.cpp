// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (size == 0) {
        return doc;
    }
    
    // Ensure null termination for string interpretation
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    
    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    // We don't fail on parse errors, as fuzzing should explore error states too
    (void)result;
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->first_child();
    
    // 2. Invoke target functions in specified order
    
    // pugi::xml_node::first_child
    pugi::xml_node child = root.first_child();
    
    // Prepare hint for attribute lookup
    pugi::xml_attribute hint;
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr1 = child.attribute("test");
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr2 = child.attribute("id");
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr3 = child.attribute("class");
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr4 = child.attribute("style");
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr5 = child.attribute("data");
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr6 = child.attribute("value");
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr7 = child.attribute("name");
    
    // pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
    // Using std::string_view as suggested for string_view_t
    std::string_view sv = std::string_view("test");
    pugi::xml_attribute attr8 = child.attribute(sv, hint);
    
    // pugi::xml_attribute::set_name
    // Try to rename an attribute if it exists
    if (attr1) {
        std::string_view new_name = std::string_view("renamed");
        attr1.set_name(new_name);
    }
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr9 = child.attribute("renamed");
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr10 = child.attribute("test");
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr11 = child.attribute("id");
    
    // pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr12 = child.attribute("class");

    // 3. Cleanup
    delete doc;
    
    return 0;
}