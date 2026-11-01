// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <pugixml.hpp>

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t offset) {
    if (offset + 1 > size) return std::string_view();
    size_t len = data[offset];
    if (offset + 1 + len > size) len = size - offset - 1;
    return std::string_view(reinterpret_cast<const char*>(data + offset + 1), len);
}

static const char* get_cstr(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "";
    size_t len = 0;
    while (offset + len < size && data[offset + len] != 0) {
        len++;
    }
    if (len == 0) return "";
    static char buf[1024];
    size_t copy_len = len > 1023 ? 1023 : len;
    std::memcpy(buf, data + offset, copy_len);
    buf[copy_len] = '\0';
    return buf;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        if (!doc.load_string("<root/>")) return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        if (!doc.append_child("root")) return 0;
        root = doc.root();
    }

    std::string attr_name1 = "attr1";
    std::string attr_name2 = "attr2";
    std::string attr_name3 = "attr3";
    
    if (Size > 2) {
        size_t len1 = std::min(Size/2, static_cast<size_t>(10));
        size_t len2 = std::min(Size/2, static_cast<size_t>(10));
        size_t len3 = std::min(Size/4, static_cast<size_t>(10));
        
        attr_name1 = std::string(reinterpret_cast<const char*>(Data), len1);
        attr_name2 = std::string(reinterpret_cast<const char*>(Data + Size/2), len2);
        attr_name3 = std::string(reinterpret_cast<const char*>(Data + Size/4), len3);
    }

    root.append_attribute(attr_name1.c_str());
    root.append_attribute(attr_name2.c_str());
    root.append_attribute(attr_name3.c_str());

    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");

    // 1. pugi::xml_node::remove_attribute (const char_t* name)
    root.remove_attribute(get_cstr(Data, Size, 0));

    // 2. pugi::xml_node::remove_attribute (const char_t* name)
    root.remove_attribute(get_cstr(Data, Size, 1));

    // 3. pugi::xml_node::child (const char_t* name)
    pugi::xml_node c1 = root.child(get_cstr(Data, Size, 2));

    // 4. pugi::xml_node::child (const char_t* name)
    pugi::xml_node c2 = root.child(get_cstr(Data, Size, 3));

    // 5. pugi::xml_node::remove_attribute (const char_t* name)
    if (c1) c1.remove_attribute(get_cstr(Data, Size, 4));
    else root.remove_attribute(get_cstr(Data, Size, 4));

    // 6. pugi::xml_node::remove_attribute (const char_t* name)
    if (c2) c2.remove_attribute(get_cstr(Data, Size, 5));
    else root.remove_attribute(get_cstr(Data, Size, 5));

    // 7. pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute a1 = root.attribute(get_cstr(Data, Size, 6));

    // 8. pugi::xml_node::remove_attribute (const xml_attribute& a)
    if (a1) {
        root.remove_attribute(a1);
    } else {
        root.remove_attribute(attr_name1.c_str());
    }

    // 9. pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute a2 = root.attribute(get_cstr(Data, Size, 7));

    // 10. pugi::xml_node::remove_attribute (const xml_attribute& a)
    if (a2) {
        root.remove_attribute(a2);
    } else {
        root.remove_attribute(attr_name2.c_str());
    }

    // 11. pugi::xml_node::remove_attribute (string_view_t name)
    std::string_view sv1 = get_string_view(Data, Size, 8);
    root.remove_attribute(sv1);

    // 12. pugi::xml_node::remove_attribute (string_view_t name)
    std::string_view sv2 = get_string_view(Data, Size, 9);
    root.remove_attribute(sv2);

    // 13. pugi::xml_node::remove_attribute (string_view_t name)
    std::string_view sv3 = get_string_view(Data, Size, 10);
    root.remove_attribute(sv3);

    return 0;
}