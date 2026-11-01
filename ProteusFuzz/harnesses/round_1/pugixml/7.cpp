// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    // This function is a placeholder if file operations were needed.
    // The task specifies using "./dummy_file" if a file is required, 
    // but the target APIs operate on in-memory structures.
    // We do not actually write to a file here as the fuzzing logic is in-memory.
    (void)data;
    (void)size;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;

    // 1. pugi::xml_node::append_child
    // We need a valid node to append to. The document root is a valid node.
    pugi::xml_node root = doc;
    
    // Ensure we have a valid name string from input or use a default to avoid null pointer issues
    const char* child_name = "child";
    if (Size > 0) {
        // Use a safe substring or just the first byte if it's not null
        // To be safe and simple, we just use a static valid name.
        // Fuzzing the name string itself is possible but requires careful null-termination.
        // For this driver, we focus on the API flow.
        child_name = "child";
    }
    
    pugi::xml_node child_node = root.append_child(child_name);
    if (!child_node) {
        return 0;
    }

    // 2. pugi::xml_node::append_attribute
    const char* attr_name = "attr";
    pugi::xml_attribute attr = child_node.append_attribute(attr_name);
    if (!attr) {
        return 0;
    }

    // 3. pugi::xml_attribute::set_value (double overload)
    double val_double = 0.0;
    if (Size > 0) {
        val_double = static_cast<double>(Data[0]);
    }
    attr.set_value(val_double, 6);

    // 4. pugi::xml_attribute::as_double
    double result_double = attr.as_double(0.0);
    (void)result_double;

    // 5. pugi::xml_attribute::set_value (float overload)
    float val_float = 0.0f;
    if (Size > 1) {
        val_float = static_cast<float>(Data[1]);
    }
    attr.set_value(val_float, 6);

    // 6. pugi::xml_attribute::as_double
    result_double = attr.as_double(0.0);
    (void)result_double;

    // 7. pugi::xml_attribute::set_value (double overload again)
    if (Size > 2) {
        val_double = static_cast<double>(Data[2]);
    }
    attr.set_value(val_double, 6);

    // 8. pugi::xml_attribute::as_double
    result_double = attr.as_double(0.0);
    (void)result_double;

    // 9. pugi::xml_node::text
    pugi::xml_text text_proxy = child_node.text();

    // 10. pugi::xml_text::set (double overload)
    if (Size > 3) {
        val_double = static_cast<double>(Data[3]);
    }
    text_proxy.set(val_double, 6);

    // 11. pugi::xml_node::text
    text_proxy = child_node.text();

    // 12. pugi::xml_node::text
    text_proxy = child_node.text();

    // 13. pugi::xml_text::set (float overload)
    if (Size > 4) {
        val_float = static_cast<float>(Data[4]);
    }
    text_proxy.set(val_float, 6);

    // 14. pugi::xml_node::text
    text_proxy = child_node.text();

    // 15. pugi::xml_node::text
    text_proxy = child_node.text();

    // 16. pugi::xml_text::set (double overload)
    if (Size > 5) {
        val_double = static_cast<double>(Data[5]);
    }
    text_proxy.set(val_double, 6);

    // 17. pugi::xml_node::text
    text_proxy = child_node.text();

    // Cleanup is handled automatically by pugi::xml_document destructor
    return 0;
}