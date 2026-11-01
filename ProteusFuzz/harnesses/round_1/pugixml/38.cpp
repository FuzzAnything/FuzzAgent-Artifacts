// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
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
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    if (!doc.load_file("./dummy_file")) {
        // If loading fails, try to parse from buffer directly to ensure coverage
        // We need to cast to char_t* which might be wchar_t or char
        // For fuzzing, we assume char_t is char or handle both if needed, 
        // but standard fuzzer input is uint8_t. 
        // We will attempt to load as string if file load fails or just proceed with empty doc.
        // To maximize coverage, let's try to load as string if possible.
        // Note: pugixml load_string expects char_t*. 
        // We will skip complex char_t conversion for simplicity and rely on file load or empty doc.
        // An empty doc is a valid state to test.
    }

    pugi::xml_node root = doc.first_child();
    if (!root) {
        root = doc; // Fallback to document itself if no first child
    }

    // 1. pugi::xml_node::first_child
    pugi::xml_node child = root.first_child();

    // Prepare hint for attribute overloads
    pugi::xml_attribute hint_attr;

    // 2. pugi::xml_node::attribute (const char_t* name)
    // We use a simple string literal. Fuzzing the name string is not directly possible 
    // with fixed literals, but we can vary the node context.
    pugi::xml_attribute attr1 = child.attribute("id");

    // 3. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
    // Using std::string_view as per hint.
    std::string_view a_str = "test_attr";
    pugi::xml_attribute attr2 = child.attribute(a_str, hint_attr);

    // 4. pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr3 = child.attribute("name");

    // 5. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
    std::string_view a_str2 = "value";
    pugi::xml_attribute attr4 = child.attribute(a_str2, hint_attr);

    // 6. pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr5 = child.attribute("class");

    // 7. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
    std::string_view a_str3 = "style";
    pugi::xml_attribute attr6 = child.attribute(a_str3, hint_attr);

    // 8. pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr7 = child.attribute("data");

    // 9. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
    std::string_view a_str4 = "info";
    pugi::xml_attribute attr8 = child.attribute(a_str4, hint_attr);

    // 10. pugi::xml_attribute::set_name
    // Try to set name on the first attribute found. 
    // If attr1 is empty, set_name returns false safely.
    std::string_view new_name = "new_name";
    attr1.set_name(new_name);

    // 11. pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr9 = child.attribute("updated");

    // 12. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
    std::string_view a_str5 = "check";
    pugi::xml_attribute attr10 = child.attribute(a_str5, hint_attr);

    // 13. pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr11 = child.attribute("final");

    // 14. pugi::xml_node::attribute (string_view_t name, xml_attribute& hint)
    std::string_view a_str6 = "end";
    pugi::xml_attribute attr12 = child.attribute(a_str6, hint_attr);

    return 0;
}