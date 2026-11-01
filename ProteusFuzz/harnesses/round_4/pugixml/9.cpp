// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string_view>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    if (size > 0) {
        // Try to parse the input as XML if it looks valid, otherwise create a minimal structure
        std::string_view input(reinterpret_cast<const char*>(data), size);
        if (input.size() > 0 && input[0] == '<') {
            // Attempt to load from string. 
            // FIX: load_string expects a null-terminated string. 
            // Since fuzz input is binary and not guaranteed to be null-terminated, 
            // we must use load_buffer which accepts size explicitly.
            pugi::xml_parse_result result = doc->load_buffer(data, size, pugi::parse_default, pugi::encoding_utf8);
            if (!result) {
                // If parsing fails, we still have a valid document object, just empty or with errors
                // We will proceed to build structure manually to test the API
            }
        }
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    try {
        // Ensure we have a root element to work with
        pugi::xml_node root = doc->document_element();
        if (root.empty()) {
            root = doc->append_child("root");
            if (root.empty()) {
                delete doc;
                return 0;
            }
        }

        // 1. pugi::xml_node::append_child
        pugi::xml_node child = root.append_child("child");
        if (child.empty()) {
            delete doc;
            return 0;
        }

        // 2. pugi::xml_node::append_attribute
        pugi::xml_attribute attr = child.append_attribute("attr");
        if (attr.empty()) {
            delete doc;
            return 0;
        }

        // 3. pugi::xml_attribute::set_value (double overload)
        double val_d = 0.0;
        if (Size > 0) val_d = static_cast<double>(Data[0]) / 10.0;
        attr.set_value(val_d, 6);

        // 4. pugi::xml_attribute::as_double
        double res_d = attr.as_double(0.0);
        (void)res_d; // Use result to avoid unused warning

        // 5. pugi::xml_attribute::set_value (float overload)
        float val_f = 0.0f;
        if (Size > 1) val_f = static_cast<float>(Data[1]) / 10.0f;
        attr.set_value(val_f, 6);

        // 6. pugi::xml_attribute::as_double
        res_d = attr.as_double(0.0);
        (void)res_d;

        // 7. pugi::xml_attribute::set_value (double overload again)
        if (Size > 2) val_d = static_cast<double>(Data[2]) / 10.0;
        attr.set_value(val_d, 6);

        // 8. pugi::xml_attribute::as_double
        res_d = attr.as_double(0.0);
        (void)res_d;

        // 9. pugi::xml_node::text
        pugi::xml_text text1 = child.text();

        // 10. pugi::xml_text::set (float overload)
        if (Size > 3) val_f = static_cast<float>(Data[3]) / 10.0f;
        text1.set(val_f, 6);

        // 11. pugi::xml_node::text
        pugi::xml_text text2 = child.text();

        // 12. pugi::xml_node::text
        pugi::xml_text text3 = child.text();

        // 13. pugi::xml_text::set (double overload)
        if (Size > 4) val_d = static_cast<double>(Data[4]) / 10.0;
        text3.set(val_d, 6);

        // 14. pugi::xml_node::text
        pugi::xml_text text4 = child.text();

        // 15. pugi::xml_node::text
        pugi::xml_text text5 = child.text();

        // 16. pugi::xml_text::set (float overload)
        if (Size > 5) val_f = static_cast<float>(Data[5]) / 10.0f;
        text5.set(val_f, 6);

        // 17. pugi::xml_node::text
        pugi::xml_text text6 = child.text();

        // Cleanup
        delete doc;
    } catch (...) {
        if (doc) delete doc;
        return 0;
    }

    return 0;
}