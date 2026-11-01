// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
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
#include <cstring>
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for safe string operations
    // We create a buffer with a null terminator to prevent buffer overflows
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    if (!result) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc;
    }

    // Create attributes to use as references
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    pugi::xml_attribute attr2 = root.append_attribute("attr2");

    // If we couldn't create attributes (e.g., root is not an element), skip attribute tests
    if (!attr1 || !attr2) {
        // Still try to run child traversal tests on the document or root
        pugi::xml_node child1 = root.first_child();
        pugi::xml_node child2 = child1.first_child();
        pugi::xml_node child3 = child2.first_child();
        pugi::xml_node child4 = child3.first_child();
        (void)child1; (void)child2; (void)child3; (void)child4;
        return 0;
    }

    // 1. insert_attribute_after
    pugi::xml_attribute newAttrAfter = root.insert_attribute_after("new_after", attr1);

    // 2. insert_attribute_before
    pugi::xml_attribute newAttrBefore = root.insert_attribute_before("new_before", attr2);

    // 3. first_child
    pugi::xml_node child1 = root.first_child();

    // 4. first_child
    pugi::xml_node child2 = child1.first_child();

    // 5. first_child
    pugi::xml_node child3 = child2.first_child();

    // 6. first_child
    pugi::xml_node child4 = child3.first_child();

    // 7. append_copy
    pugi::xml_attribute copiedAttrAppend = root.append_copy(attr1);

    // 8. prepend_copy
    pugi::xml_attribute copiedAttrPrepend = root.prepend_copy(attr2);

    // 9. insert_copy_after
    pugi::xml_attribute copiedAttrAfter = root.insert_copy_after(attr1, attr2);

    // 10. insert_copy_before
    pugi::xml_attribute copiedAttrBefore = root.insert_copy_before(attr2, attr1);

    // Suppress unused variable warnings
    (void)newAttrAfter;
    (void)newAttrBefore;
    (void)child1;
    (void)child2;
    (void)child3;
    (void)child4;
    (void)copiedAttrAppend;
    (void)copiedAttrPrepend;
    (void)copiedAttrAfter;
    (void)copiedAttrBefore;

    return 0;
}