// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    pugi::xml_document doc;
    if (!doc.load_string("<root/>")) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // Helper lambda to extract a string from fuzz data
    auto get_string = [&](size_t &offset, size_t max_len) -> const char* {
        if (offset >= Size) return "";
        size_t len = 0;
        while (offset + len < Size && len < max_len && Data[offset + len] != '\0') {
            len++;
        }
        if (offset + len >= Size) return "";
        // Ensure we don't return a pointer into the middle of the buffer without null termination
        // For safety in fuzzing, we treat the segment as a C-string if null-terminated, else empty
        if (Data[offset + len] == '\0') {
            return reinterpret_cast<const char*>(Data + offset);
        }
        return "";
    };

    size_t offset = 0;
    
    // Sequence 1
    {
        const char* name1 = get_string(offset, 100);
        pugi::xml_attribute attr1 = root.append_attribute(name1);
        if (attr1) {
            const char* val1 = get_string(offset, 100);
            attr1.set_value(val1);
        }
    }
    // Sequence 2
    {
        const char* name2 = get_string(offset, 100);
        pugi::xml_attribute attr2 = root.append_attribute(name2);
        if (attr2) {
            const char* val2 = get_string(offset, 100);
            attr2.set_value(val2);
        }
    }
    // Sequence 3
    {
        const char* name3 = get_string(offset, 100);
        pugi::xml_attribute attr3 = root.append_attribute(name3);
        if (attr3) {
            const char* val3 = get_string(offset, 100);
            attr3.set_value(val3);
        }
    }
    // Sequence 4
    {
        const char* name4 = get_string(offset, 100);
        pugi::xml_attribute attr4 = root.append_attribute(name4);
        if (attr4) {
            const char* val4 = get_string(offset, 100);
            attr4.set_value(val4);
        }
    }
    // Sequence 5
    {
        const char* name5 = get_string(offset, 100);
        pugi::xml_attribute attr5 = root.append_attribute(name5);
        if (attr5) {
            const char* val5 = get_string(offset, 100);
            attr5.set_value(val5);
        }
    }
    // Sequence 6
    {
        const char* name6 = get_string(offset, 100);
        pugi::xml_attribute attr6 = root.append_attribute(name6);
        if (attr6) {
            const char* val6 = get_string(offset, 100);
            attr6.set_value(val6);
        }
    }

    // Additional exploration: Try string_view_t overload for append_attribute if available
    // Note: string_view_t is typically std::string_view in C++17 or a wrapper.
    // We attempt to construct a string_view from the data if possible.
    if (Size > 0) {
        std::string_view sv(reinterpret_cast<const char*>(Data), Size);
        pugi::xml_attribute attr_sv = root.append_attribute(sv);
        if (attr_sv) {
            // Try various set_value overloads on this attribute
            // 1. string_view_t
            attr_sv.set_value(sv);
            // 2. const char_t*
            attr_sv.set_value("test_string");
            // 3. float
            attr_sv.set_value(3.14f);
            // 4. double
            attr_sv.set_value(2.718281828);
            // 5. bool
            attr_sv.set_value(true);
        }
    }

    return 0;
}