// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: write input to dummy file and load document
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // If parsing fails, we can still test with an empty document or skip
    // We proceed to test the API chain on whatever document we have (even if empty)

    // 1. pugi::xml_node::child
    std::string_view child_name_view = "test";
    const char* child_name = child_name_view.data();
    pugi::xml_node node1 = doc.child(child_name);

    // 2. pugi::xml_node::attribute
    std::string_view attr_name_view = "id";
    const char* attr_name = attr_name_view.data();
    pugi::xml_attribute attr1 = node1.attribute(attr_name);

    // 3. pugi::xml_node::child (again)
    pugi::xml_node node2 = node1.child(child_name);

    // 4. pugi::xml_node::attribute (again)
    pugi::xml_attribute attr2 = node2.attribute(attr_name);

    // 5. pugi::xml_attribute::next_attribute
    pugi::xml_attribute attr3 = attr2.next_attribute();

    // 6. pugi::xml_attribute::next_attribute (again)
    pugi::xml_attribute attr4 = attr3.next_attribute();

    // 7. pugi::xml_attribute::previous_attribute
    pugi::xml_attribute attr5 = attr4.previous_attribute();

    // 8. pugi::xml_attribute::previous_attribute (again)
    pugi::xml_attribute attr6 = attr5.previous_attribute();

    // 9. pugi::xml_attribute::next_attribute
    pugi::xml_attribute attr7 = attr6.next_attribute();

    // 10. pugi::xml_attribute::previous_attribute
    pugi::xml_attribute attr8 = attr7.previous_attribute();

    // Cleanup is handled by RAII (xml_document destructor)
    // No exceptions thrown by these functions, return 0
    return 0;
}