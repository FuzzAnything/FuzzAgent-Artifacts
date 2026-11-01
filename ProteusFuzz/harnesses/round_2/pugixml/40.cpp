// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
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
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Ensure the input is null-terminated for safe parsing by load_string
    // We create a buffer with an extra byte for the null terminator
    std::vector<char> buffer(Size + 1);
    if (Size > 0) {
        std::memcpy(buffer.data(), Data, Size);
    }
    buffer[Size] = '\0';

    // Try to load the document. If it fails, we create a minimal valid document.
    if (!doc.load_string(buffer.data())) {
        doc.append_child("root");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }

    // Ensure we have an element node to work with
    if (root.type() != pugi::node_element) {
        return 0;
    }

    // Prepare string_view for overloads
    std::string_view a_str = "fuzz_attr";
    const char* c_str = "fuzz_attr";

    // 1. pugi::xml_node::prepend_attribute (string_view_t)
    pugi::xml_attribute attr1 = root.prepend_attribute(a_str);

    // 2. pugi::xml_node::prepend_attribute (const char_t*)
    pugi::xml_attribute attr2 = root.prepend_attribute(c_str);

    // 3. pugi::xml_node::child
    pugi::xml_node child1 = root.child("child1");

    // 4. pugi::xml_node::prepend_attribute (string_view_t) on child if exists, else root
    pugi::xml_node target4 = child1 ? child1 : root;
    pugi::xml_attribute attr3 = target4.prepend_attribute(a_str);

    // 5. pugi::xml_node::child
    pugi::xml_node child2 = target4.child("child2");

    // 6. pugi::xml_node::prepend_attribute (const char_t*) on child if exists, else target4
    pugi::xml_node target6 = child2 ? child2 : target4;
    pugi::xml_attribute attr4 = target6.prepend_attribute(c_str);

    // 7. pugi::xml_node::child
    pugi::xml_node child3 = target6.child("child3");

    // 8. pugi::xml_node::child
    pugi::xml_node child4 = child3 ? child3.child("child4") : target6.child("child4");

    // 9. pugi::xml_node::prepend_attribute (string_view_t) on child if exists, else target6
    pugi::xml_node target9 = child4 ? child4 : target6;
    pugi::xml_attribute attr5 = target9.prepend_attribute(a_str);

    // 10. pugi::xml_node::child
    pugi::xml_node child5 = target9.child("child5");

    // 11. pugi::xml_node::child
    pugi::xml_node child6 = child5 ? child5.child("child6") : target9.child("child6");

    // 12. pugi::xml_node::prepend_attribute (const char_t*) on child if exists, else target9
    pugi::xml_node target12 = child6 ? child6 : target9;
    pugi::xml_attribute attr6 = target12.prepend_attribute(c_str);

    return 0;
}