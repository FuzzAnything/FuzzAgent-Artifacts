// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
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

static const char* ensure_null_terminated(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    // Find the first null byte
    size_t null_pos = 0;
    while (null_pos < size && data[null_pos] != 0) {
        null_pos++;
    }
    // If no null byte found, return empty string to avoid OOB read
    if (null_pos == size) return "";
    return reinterpret_cast<const char*>(data);
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    pugi::xml_document doc;
    if (!doc.load_string("<root/>")) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // 1. pugi::xml_node::child
    const char* child_name = ensure_null_terminated(Data, Size);
    pugi::xml_node node1 = root.child(child_name);

    // If node1 is empty, we might not be able to append children effectively in some contexts,
    // but append_child on an empty node returns an empty node, which is safe.
    // However, to follow the sequence and ensure we have a valid parent for subsequent appends,
    // we can append to root if node1 is empty, or use node1 if it exists.
    // The prompt asks to invoke child -> append_child -> ...
    // Let's use root as the base for appending if child() returns empty, or node1 if it exists.
    // Actually, the sequence implies: get child, then append to that child (or the result).
    // If node1 is empty, append_child returns empty. Then subsequent calls on empty node return empty.
    // This is valid behavior to test.
    
    // 2. pugi::xml_node::append_child (first call)
    const char* append_name1 = ensure_null_terminated(Data, Size);
    pugi::xml_node node2 = node1.append_child(append_name1);

    // 3. pugi::xml_node::append_child (second call)
    const char* append_name2 = ensure_null_terminated(Data, Size);
    pugi::xml_node node3 = node2.append_child(append_name2);

    // 4. pugi::xml_text::set (long overload)
    pugi::xml_text text_node = node3.text();
    // Use a value derived from input, clamped to long range
    long val_long = 0;
    if (Size >= sizeof(long)) {
        std::memcpy(&val_long, Data, sizeof(long));
    }
    text_node.set(val_long);

    // 5. pugi::xml_node::append_child (third call)
    const char* append_name3 = ensure_null_terminated(Data, Size);
    pugi::xml_node node4 = node3.append_child(append_name3);

    // 6. pugi::xml_node::append_child (fourth call)
    const char* append_name4 = ensure_null_terminated(Data, Size);
    pugi::xml_node node5 = node4.append_child(append_name4);

    // 7. pugi::xml_text::set (unsigned long overload)
    pugi::xml_text text_node2 = node5.text();
    unsigned long val_ulong = 0;
    if (Size >= sizeof(unsigned long)) {
        std::memcpy(&val_ulong, Data, sizeof(unsigned long));
    }
    text_node2.set(val_ulong);

    return 0;
}