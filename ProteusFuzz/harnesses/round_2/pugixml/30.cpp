// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    if (!doc.load_string("<root/>")) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::append_child (overload 1: type)
    pugi::xml_node node1 = root.append_child(pugi::node_element);
    if (!node1) {
        return 0;
    }

    // 2. pugi::xml_node::append_child (overload 2: name)
    pugi::xml_node node2 = root.append_child("child2");
    if (!node2) {
        return 0;
    }

    // 3. pugi::xml_node::child
    pugi::xml_node child1 = root.child("child2");

    // 4. pugi::xml_node::append_buffer
    // Prepare a safe buffer from fuzz input or a default string if empty
    const char* buffer_content = "<inner/>";
    size_t buffer_size = std::strlen(buffer_content);
    
    // Optionally mix fuzz data if it looks like XML, but keep it safe
    if (Size > 10 && Data[0] == '<') {
        buffer_content = reinterpret_cast<const char*>(Data);
        buffer_size = Size;
    }

    pugi::xml_parse_result res1 = child1.append_buffer(buffer_content, buffer_size);
    // Check result status if needed, but continue for fuzzing

    // 5. pugi::xml_node::child
    pugi::xml_node child2 = child1.child("inner");

    // 6. pugi::xml_node::child
    pugi::xml_node child3 = root.child("child2");

    // 7. pugi::xml_node::remove_children
    bool removed = root.remove_children();
    // removed is bool, ignore for now

    // 8. pugi::xml_node::child
    pugi::xml_node child4 = root.child("child2");

    // 9. pugi::xml_node::append_buffer
    // Use the original buffer again or a different one
    pugi::xml_parse_result res2 = child4.append_buffer(buffer_content, buffer_size);

    // 10. pugi::xml_node::child
    pugi::xml_node child5 = child4.child("inner");

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}