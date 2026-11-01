// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5554:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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

static void fuzz_text_set(pugi::xml_text text_node, const uint8_t* data, size_t size) {
    if (size < 2) return;
    double d_val = 0.0;
    float f_val = 0.0f;
    int precision = 0;

    if (size >= 8) {
        d_val = *reinterpret_cast<const double*>(data);
        f_val = *reinterpret_cast<const float*>(data + 4);
    }
    if (size >= 12) {
        precision = *reinterpret_cast<const int*>(data + 8);
        if (precision < 0) precision = 0;
        if (precision > 20) precision = 20;
    }

    text_node.set(d_val, precision);
    text_node.set(f_val, precision);
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, create a minimal valid document
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        if (!root) return 0;
    } else {
        // Ensure we have a root element to work on
        pugi::xml_node root = doc.child("root");
        if (!root) {
            root = doc.append_child("root");
            if (!root) return 0;
        }
    }

    // 1. pugi::xml_node::append_child
    const char* child_name = "child";
    pugi::xml_node child = doc.append_child(child_name);
    if (!child) return 0;

    // 2. pugi::xml_node::append_attribute
    const char* attr_name = "attr";
    pugi::xml_attribute attr = child.append_attribute(attr_name);
    if (!attr) return 0;

    // Prepare values from input if possible
    double d_val = 0.0;
    float f_val = 0.0f;
    int precision = 0;

    if (Size >= 8) {
        d_val = *reinterpret_cast<const double*>(Data);
        f_val = *reinterpret_cast<const float*>(Data + 4);
    }
    if (Size >= 12) {
        precision = *reinterpret_cast<const int*>(Data + 8);
        if (precision < 0) precision = 0;
        if (precision > 20) precision = 20;
    }

    // 3. pugi::xml_attribute::set_value (double)
    attr.set_value(d_val, precision);
    
    // 4. pugi::xml_attribute::as_double
    double res1 = attr.as_double(0.0);
    (void)res1; // Suppress unused warning

    // 5. pugi::xml_attribute::set_value (float)
    attr.set_value(f_val, precision);
    
    // 6. pugi::xml_attribute::as_double
    double res2 = attr.as_double(0.0);
    (void)res2;

    // 7. pugi::xml_attribute::set_value (double again)
    attr.set_value(d_val, precision);
    
    // 8. pugi::xml_attribute::as_double
    double res3 = attr.as_double(0.0);
    (void)res3;

    // 9. pugi::xml_node::text
    pugi::xml_text text1 = child.text();
    
    // 10. pugi::xml_text::set
    fuzz_text_set(text1, Data, Size);

    // 11. pugi::xml_node::text
    pugi::xml_text text2 = child.text();
    (void)text2;

    // 12. pugi::xml_node::text
    pugi::xml_text text3 = child.text();
    
    // 13. pugi::xml_text::set
    fuzz_text_set(text3, Data, Size);

    // 14. pugi::xml_node::text
    pugi::xml_text text4 = child.text();
    (void)text4;

    // 15. pugi::xml_node::text
    pugi::xml_text text5 = child.text();
    
    // 16. pugi::xml_text::set
    fuzz_text_set(text5, Data, Size);

    // 17. pugi::xml_node::text
    pugi::xml_text text6 = child.text();
    (void)text6;

    return 0;
}