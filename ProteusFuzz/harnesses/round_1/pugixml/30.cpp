// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
    
    // Ensure the input is null-terminated for load_string
    // We create a temporary buffer with a null terminator if not present
    std::vector<char> safe_buffer(Size + 1);
    std::memcpy(safe_buffer.data(), Data, Size);
    safe_buffer[Size] = '\0';

    // Attempt to load from file if it exists, otherwise parse from buffer
    if (!doc.load_file("./dummy_file")) {
        if (!doc.load_string(safe_buffer.data())) {
            doc.reset();
        }
    }

    pugi::xml_node root = doc.root();
    if (root.empty()) {
        root = doc.append_child("root");
    }

    // 1. pugi::xml_node::append_child (overload 1: const char_t* name)
    pugi::xml_node child1 = root.append_child("child1");

    // 2. pugi::xml_node::append_child (overload 2: xml_node_type type)
    pugi::xml_node child2 = root.append_child(pugi::node_element);

    // 3. pugi::xml_node::child
    pugi::xml_node found1 = root.child("child1");

    // 4. pugi::xml_node::append_buffer
    // Ensure we don't read past the buffer if Size is large
    size_t bufSize = Size;
    if (bufSize > 1024) bufSize = 1024;
    
    if (bufSize > 0) {
        const void* buffer = reinterpret_cast<const void*>(Data);
        // Check if found1 is valid before appending
        if (!found1.empty()) {
            found1.append_buffer(buffer, bufSize, pugi::parse_default, pugi::encoding_auto);
        }
    }

    // 5. pugi::xml_node::child
    pugi::xml_node found2 = root.child("child2");

    // 6. pugi::xml_node::child
    pugi::xml_node found3 = found2.child("nonexistent");

    // 7. pugi::xml_node::remove_children
    bool removed = root.remove_children();

    // 8. pugi::xml_node::child
    pugi::xml_node found4 = root.child("child1");

    // 9. pugi::xml_node::append_buffer
    if (bufSize > 0) {
        const void* buffer = reinterpret_cast<const void*>(Data);
        if (!found4.empty()) {
            found4.append_buffer(buffer, bufSize, pugi::parse_default, pugi::encoding_auto);
        }
    }

    // 10. pugi::xml_node::child
    pugi::xml_node found5 = found4.child("test");

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}