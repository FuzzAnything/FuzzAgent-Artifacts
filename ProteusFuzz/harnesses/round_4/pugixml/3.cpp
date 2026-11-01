// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_node::attributes_begin at pugixml.cpp:5628:54 in pugixml.hpp
// pugi::xml_node::attributes_end at pugixml.cpp:5633:54 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
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
#include <cstdio>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: write data to a dummy file
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    if (!doc.load_file("./dummy_file")) {
        // If parsing fails, we still want to test the API on an empty/invalid document
        // to ensure robustness against malformed input.
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc; // Fallback to document node if root is empty
    }

    // 1. pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_node_iterator> children_range = root.children();

    // 2. pugi::xml_node::begin
    pugi::xml_node_iterator begin_iter = root.begin();

    // 3. pugi::xml_node::end
    pugi::xml_node_iterator end_iter = root.end();

    // 4. pugi::xml_node::children (second invocation)
    pugi::xml_object_range<pugi::xml_node_iterator> children_range2 = root.children();

    // 5. pugi::xml_object_range::begin
    pugi::xml_node_iterator range_begin = children_range2.begin();

    // 6. pugi::xml_object_range::end
    pugi::xml_node_iterator range_end = children_range2.end();

    // 7. pugi::xml_node::child
    // Use a string_view_t compatible type (const char_t*)
    // We try to find a child named "test" or use a pointer from input if safe
    const pugi::char_t* name_ptr = "test";
    if (Size > 10) {
        // Create a safe buffer to hold a name from input if possible, 
        // but for simplicity and safety, we stick to a static name or null-terminated slice.
        // Here we just use the static name to avoid complex buffer management in fuzzing.
        name_ptr = "test"; 
    }
    pugi::xml_node child_node = root.child(name_ptr);

    // 8. pugi::xml_node::attributes
    pugi::xml_object_range<pugi::xml_attribute_iterator> attrs_range = root.attributes();

    // 9. pugi::xml_node::attributes_begin
    pugi::xml_attribute_iterator attrs_begin = root.attributes_begin();

    // 10. pugi::xml_node::attributes_end
    pugi::xml_attribute_iterator attrs_end = root.attributes_end();

    // 11. pugi::xml_node::attributes (second invocation)
    pugi::xml_object_range<pugi::xml_attribute_iterator> attrs_range2 = root.attributes();

    // 12. pugi::xml_object_range::begin (for attributes)
    pugi::xml_attribute_iterator attrs_range_begin = attrs_range2.begin();

    // 13. pugi::xml_object_range::end (for attributes)
    pugi::xml_attribute_iterator attrs_range_end = attrs_range2.end();

    // Cleanup is handled by RAII (xml_document destructor)
    // No explicit cleanup needed for iterators or ranges as they are value types or lightweight wrappers.

    return 0;
}