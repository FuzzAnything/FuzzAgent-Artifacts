// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5554:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
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
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    (void)data;
    (void)size;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    // xml_document does not have an allocate() method in standard pugixml.
    // It is ready to use upon construction.

    // 1. pugi::xml_node::append_child
    const char* child_name = "child";
    
    pugi::xml_node root = doc;
    pugi::xml_node child = root.append_child(child_name);
    if (!child) {
        return 0;
    }

    // 2. pugi::xml_node::append_attribute
    pugi::xml_attribute attr = child.append_attribute("test_attr");
    if (!attr) {
        return 0;
    }

    // Helper to get a double from fuzz data safely
    auto get_double_from_data = [&](size_t offset) -> double {
        if (offset + 8 > Size) return 0.0;
        double val;
        uint8_t buf[8];
        for (int i = 0; i < 8; ++i) buf[i] = Data[offset + i];
        return *reinterpret_cast<double*>(buf);
    };

    // 3. pugi::xml_attribute::set_value (double overload)
    double val1 = get_double_from_data(0);
    int precision1 = (Size > 8) ? static_cast<int>(Data[8] % 100) : 6;
    attr.set_value(val1, precision1);

    // 4. pugi::xml_attribute::as_double
    double res1 = attr.as_double(0.0);
    (void)res1;

    // 5. pugi::xml_attribute::set_value (float overload)
    float val2 = static_cast<float>(get_double_from_data(8));
    int precision2 = (Size > 16) ? static_cast<int>(Data[16] % 100) : 6;
    attr.set_value(val2, precision2);

    // 6. pugi::xml_attribute::as_double
    double res2 = attr.as_double(0.0);
    (void)res2;

    // 7. pugi::xml_attribute::set_value (double overload again)
    double val3 = get_double_from_data(16);
    int precision3 = (Size > 24) ? static_cast<int>(Data[24] % 100) : 6;
    attr.set_value(val3, precision3);

    // 8. pugi::xml_attribute::as_double
    double res3 = attr.as_double(0.0);
    (void)res3;

    // 9. pugi::xml_node::text
    pugi::xml_text text1 = child.text();

    // 10. pugi::xml_text::set (float overload)
    float text_val1 = static_cast<float>(get_double_from_data(24));
    int text_prec1 = (Size > 32) ? static_cast<int>(Data[32] % 100) : 6;
    text1.set(text_val1, text_prec1);

    // 11. pugi::xml_node::text
    pugi::xml_text text2 = child.text();

    // 12. pugi::xml_node::text
    pugi::xml_text text3 = child.text();

    // 13. pugi::xml_text::set (double overload)
    double text_val2 = get_double_from_data(32);
    int text_prec2 = (Size > 40) ? static_cast<int>(Data[40] % 100) : 6;
    text3.set(text_val2, text_prec2);

    // 14. pugi::xml_node::text
    pugi::xml_text text4 = child.text();

    // 15. pugi::xml_node::text
    pugi::xml_text text5 = child.text();

    // 16. pugi::xml_text::set (float overload again)
    float text_val3 = static_cast<float>(get_double_from_data(40));
    int text_prec3 = (Size > 48) ? static_cast<int>(Data[48] % 100) : 6;
    text5.set(text_val3, text_prec3);

    // 17. pugi::xml_node::text
    pugi::xml_text text6 = child.text();

    // Cleanup is handled by xml_document destructor automatically.
    return 0;
}