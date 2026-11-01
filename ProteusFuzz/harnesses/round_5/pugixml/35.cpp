// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::child
    // Use a safe string from the input data, ensuring it's null-terminated or using a substring
    std::string child_name_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for safety if passed as C-string, though pugixml often handles non-null-terminated via size or string_view
    // Here we pass the string directly which works with the string_view_t overload or C-string if we manage it.
    // To be safe with the char_t* overload, we ensure a null terminator or use the string_view overload if available.
    // The error suggests char_t is char. Let's use the string_view overload implicitly or explicitly cast.
    // Since the error showed 'string_view_t' candidate, we can pass a std::string_view.
    std::string_view sv(child_name_str);
    pugi::xml_node child_node = root.child(sv);

    // If child_node is empty, we can still proceed with appending to root or the child if it exists
    pugi::xml_node target = child_node ? child_node : root;

    // 2. pugi::xml_node::append_child (first call)
    pugi::xml_node node1 = target.append_child("test1");
    if (!node1) {
        return 0;
    }

    // 3. pugi::xml_node::append_child (second call)
    pugi::xml_node node2 = target.append_child("test2");
    if (!node2) {
        return 0;
    }

    // 4. pugi::xml_text::set (long long)
    pugi::xml_text text1 = node1.text();
    long long val_ll = static_cast<long long>(Data[0]);
    text1.set(val_ll);

    // 4b. pugi::xml_text::set (unsigned long long)
    pugi::xml_text text2 = node2.text();
    unsigned long long val_ull = static_cast<unsigned long long>(Data[1]);
    text2.set(val_ull);

    // 5. pugi::xml_node::append_child (third call)
    pugi::xml_node node3 = target.append_child("test3");
    if (!node3) {
        return 0;
    }

    // 6. pugi::xml_node::append_child (fourth call)
    pugi::xml_node node4 = target.append_child("test4");
    if (!node4) {
        return 0;
    }

    // 7. pugi::xml_text::set (long long)
    pugi::xml_text text3 = node3.text();
    long long val_ll2 = static_cast<long long>(Data[0] ^ Data[1]);
    text3.set(val_ll2);

    // 7b. pugi::xml_text::set (unsigned long long)
    pugi::xml_text text4 = node4.text();
    unsigned long long val_ull2 = static_cast<unsigned long long>(Data[0] | Data[1]);
    text4.set(val_ull2);

    return 0;
}