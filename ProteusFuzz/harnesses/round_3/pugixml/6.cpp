// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
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

static void cleanup_document(pugi::xml_document& doc) {
    doc.reset();
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::append_child
    pugi::xml_node child = root.append_child("child");
    if (!child) {
        cleanup_document(doc);
        return 0;
    }

    // 2. pugi::xml_node::append_attribute
    pugi::xml_attribute attr = child.append_attribute("attr");
    if (!attr) {
        cleanup_document(doc);
        return 0;
    }

    // 3. pugi::xml_attribute::set_value (double overload)
    double val_double = static_cast<double>(Data[0]);
    attr.set_value(val_double, 6);

    // 4. pugi::xml_attribute::as_double
    double res1 = attr.as_double(0.0);
    (void)res1;

    // 5. pugi::xml_attribute::set_value (float overload)
    float val_float = static_cast<float>(Data[1]);
    attr.set_value(val_float, 6);

    // 6. pugi::xml_attribute::as_double
    double res2 = attr.as_double(0.0);
    (void)res2;

    // 7. pugi::xml_attribute::set_value (double overload again)
    double val_double2 = static_cast<double>(Data[2]);
    attr.set_value(val_double2, 6);

    // 8. pugi::xml_attribute::as_double
    double res3 = attr.as_double(0.0);
    (void)res3;

    // 9. pugi::xml_node::text
    pugi::xml_text text1 = child.text();

    // 10. pugi::xml_text::set (double overload)
    double text_val_double = static_cast<double>(Data[3]);
    text1.set(text_val_double, 6);

    // 11. pugi::xml_node::text
    pugi::xml_text text2 = child.text();
    (void)text2;

    // 12. pugi::xml_node::text
    pugi::xml_text text3 = child.text();

    // 13. pugi::xml_text::set (float overload)
    float text_val_float = static_cast<float>(Data[4]);
    text3.set(text_val_float, 6);

    // 14. pugi::xml_node::text
    pugi::xml_text text4 = child.text();
    (void)text4;

    // 15. pugi::xml_node::text
    pugi::xml_text text5 = child.text();

    // 16. pugi::xml_text::set (double overload)
    double text_val_double2 = static_cast<double>(Data[5]);
    text5.set(text_val_double2, 6);

    // 17. pugi::xml_node::text
    pugi::xml_text text6 = child.text();
    (void)text6;

    cleanup_document(doc);
    return 0;
}