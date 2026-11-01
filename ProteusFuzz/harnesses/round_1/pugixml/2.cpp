// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
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
#include <cstring>

static const char* safe_cstr(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    // Ensure null-termination by copying to a static buffer if needed
    // For fuzzing, we assume the input might not be null-terminated
    static char buffer[65536];
    size_t len = (size < sizeof(buffer) - 1) ? size : sizeof(buffer) - 1;
    std::memcpy(buffer, data, len);
    buffer[len] = '\0';
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // 1. pugi::xml_document::load_string
    const char* xml_str = safe_cstr(Data, Size);
    pugi::xml_parse_result result = doc.load_string(xml_str);
    if (!result) return 0; // Fail silently if parsing fails completely

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root, try to create one or use document node
        root = doc;
    }

    // 2. pugi::xml_node::child (first call)
    pugi::xml_node child1 = root.child("child1");
    if (!child1) {
        // Try to create a child if not found to proceed with the chain
        // We cannot easily create a child with a specific name without append_child
        // But the task says invoke child(). If it returns empty, we continue with empty node
        // Operations on empty nodes usually return false or empty, which is safe for fuzzing
    }

    // 3. pugi::xml_node::child (second call)
    pugi::xml_node child2 = child1.child("child2");

    // 4. pugi::xml_node::set_name
    // Use a safe string for the new name
    const char* new_name = "new_name";
    child2.set_name(new_name);

    // 5. pugi::xml_node::attribute
    pugi::xml_attribute attr1 = child2.attribute("attr1");

    // 6. pugi::xml_attribute::set_name
    const char* new_attr_name = "new_attr_name";
    attr1.set_name(new_attr_name);

    // 7. pugi::xml_node::attribute (second call)
    pugi::xml_attribute attr2 = child2.attribute("attr2");

    // 8. pugi::xml_attribute::set_value
    const char* attr_value = "test_value";
    attr2.set_value(attr_value);

    // 9. pugi::xml_node::child (third call)
    pugi::xml_node child3 = child2.child("child3");

    // 10. pugi::xml_node::text
    pugi::xml_text text_node = child3.text();

    // 11. pugi::xml_text::set
    const char* text_content = "text_content";
    text_node.set(text_content);

    // 12. pugi::xml_node::append_copy
    pugi::xml_node copied1 = child2.append_copy(child3);

    // 13. pugi::xml_node::child (fourth call)
    pugi::xml_node child4 = copied1.child("child4");

    // 14. pugi::xml_node::append_copy (second call)
    pugi::xml_node copied2 = child4.append_copy(child2);

    return 0;
}