// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
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
#include <cstddef>
#include <string>

static std::string make_string_from_input(const uint8_t* data, size_t size) {
    std::string result;
    if (size == 0) {
        return result;
    }
    // Ensure null termination and avoid invalid UTF-8 if possible, though pugixml handles char_t
    size_t max_len = size > 1024 ? 1024 : size;
    result.assign(reinterpret_cast<const char*>(data), max_len);
    // Replace null bytes to ensure valid C-strings for API calls
    for (size_t i = 0; i < result.size(); ++i) {
        if (result[i] == '\0') {
            result[i] = ' ';
        }
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, create a minimal document to test the API
        doc.append_child(pugi::node_document);
    }

    pugi::xml_node root = doc.first_child();
    if (root.empty()) {
        root = doc.append_child("root");
    }

    // Helper to get safe C-string from input
    auto get_cstr = [](const std::string& s) -> const pugi::char_t* {
        return s.c_str();
    };

    std::string s1 = make_string_from_input(Data, Size);
    std::string s2 = make_string_from_input(Data, Size);
    std::string s3 = make_string_from_input(Data, Size);
    std::string s4 = make_string_from_input(Data, Size);
    std::string s5 = make_string_from_input(Data, Size);
    std::string s6 = make_string_from_input(Data, Size);
    std::string s7 = make_string_from_input(Data, Size);
    std::string s8 = make_string_from_input(Data, Size);

    // 1. append_child (type overload)
    pugi::xml_node n1 = root.append_child(pugi::node_element);
    if (!n1.empty()) {
        n1.set_name(get_cstr(s1));
    }

    // 2. append_child (name overload)
    pugi::xml_node n2 = root.append_child(get_cstr(s2));

    // 3. append_child (type overload)
    pugi::xml_node n3 = root.append_child(pugi::node_pcdata);

    // 4. append_child (name overload)
    pugi::xml_node n4 = root.append_child(get_cstr(s3));

    // 5. append_attribute
    pugi::xml_attribute a1 = root.append_attribute(get_cstr(s4));
    if (!a1.empty()) {
        a1.set_value(get_cstr(s5));
    }

    // 6. append_attribute
    pugi::xml_attribute a2 = root.append_attribute(get_cstr(s6));
    if (!a2.empty()) {
        a2.set_value(get_cstr(s7));
    }

    // 7. append_attribute
    pugi::xml_attribute a3 = root.append_attribute(get_cstr(s8));
    if (!a3.empty()) {
        a3.set_value(get_cstr(s1));
    }

    // 8. find_child_by_attribute (2 args)
    pugi::xml_node f1 = root.find_child_by_attribute(get_cstr(s4), get_cstr(s5));

    // 9. find_child_by_attribute (2 args)
    pugi::xml_node f2 = root.find_child_by_attribute(get_cstr(s6), get_cstr(s7));

    // 10. find_child_by_attribute (2 args)
    pugi::xml_node f3 = root.find_child_by_attribute(get_cstr(s8), get_cstr(s1));

    // 11. find_child_by_attribute (2 args)
    pugi::xml_node f4 = root.find_child_by_attribute(get_cstr(s2), get_cstr(s3));

    // 12. find_child_by_attribute (2 args)
    pugi::xml_node f5 = root.find_child_by_attribute(get_cstr(s1), get_cstr(s2));

    // 13. find_child_by_attribute (2 args)
    pugi::xml_node f6 = root.find_child_by_attribute(get_cstr(s3), get_cstr(s4));

    // 14. find_child_by_attribute (2 args)
    pugi::xml_node f7 = root.find_child_by_attribute(get_cstr(s5), get_cstr(s6));

    // 15. find_child_by_attribute (3 args)
    pugi::xml_node f8 = root.find_child_by_attribute(get_cstr(s1), get_cstr(s4), get_cstr(s5));

    // 16. find_child_by_attribute (3 args)
    pugi::xml_node f9 = root.find_child_by_attribute(get_cstr(s2), get_cstr(s6), get_cstr(s7));

    // 17. find_child_by_attribute (3 args)
    pugi::xml_node f10 = root.find_child_by_attribute(get_cstr(s3), get_cstr(s8), get_cstr(s1));

    // 18. find_child_by_attribute (3 args)
    pugi::xml_node f11 = root.find_child_by_attribute(get_cstr(s4), get_cstr(s2), get_cstr(s3));

    // 19. find_child_by_attribute (3 args)
    pugi::xml_node f12 = root.find_child_by_attribute(get_cstr(s5), get_cstr(s1), get_cstr(s2));

    // 20. find_child_by_attribute (3 args)
    pugi::xml_node f13 = root.find_child_by_attribute(get_cstr(s6), get_cstr(s3), get_cstr(s4));

    // 21. find_child_by_attribute (3 args)
    pugi::xml_node f14 = root.find_child_by_attribute(get_cstr(s7), get_cstr(s5), get_cstr(s6));

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}