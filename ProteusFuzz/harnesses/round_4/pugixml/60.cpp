// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
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
#include <string>

static void fuzz_xml_attribute_methods(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to create a minimal valid document to test attribute methods
        // pugi::xml_document does not have a clear() method, so we just proceed with the empty doc
        // or re-initialize by assigning a new document if needed, but here we just use the current one.
        // We will manually add a root and attribute to ensure we have something to test.
        pugi::xml_node root = doc.append_child("root");
        if (!root) return; // Should not happen usually
        
        pugi::xml_attribute attr = root.append_attribute("test_attr");
        if (attr) {
            // Try to set some data from the input if possible
            if (Size > 0) {
                std::string val(reinterpret_cast<const char*>(Data), Size);
                // Truncate to avoid null bytes in the middle if needed, though set_value handles strings
                // We use a safe substring to avoid potential issues with embedded nulls if treated as C-string
                // but set_value takes const char_t*, so we pass c_str().
                attr.set_value(val.c_str());
            }
        }
    }

    // Iterate through all nodes to find attributes
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        // Test attribute retrieval
        pugi::xml_attribute attr = node.attribute("test_attr");
        if (!attr) {
            // Try other common names or just continue if not found
            attr = node.attribute("id");
            if (!attr) {
                attr = node.attribute("name");
                if (!attr) {
                    continue;
                }
            }
        }

        // 1. Test as_bool
        bool bool_val = attr.as_bool();
        bool bool_val_def = attr.as_bool(true);

        // 2. Test as_ullong
        unsigned long long ull_val = attr.as_ullong();
        unsigned long long ull_val_def = attr.as_ullong(999);

        // 3. Test as_string
        const pugi::char_t* str_val = attr.as_string();
        const pugi::char_t* str_val_def = attr.as_string(PUGIXML_TEXT("default"));

        // 4. Test set_value with unsigned long
        // We use a value derived from the input or a fixed value
        unsigned long test_val = 0;
        if (Size >= sizeof(unsigned long)) {
            std::memcpy(&test_val, Data, sizeof(unsigned long));
        }
        attr.set_value(test_val);

        // 5. Test set_name
        // Prepare a new name from input or fixed string
        const pugi::char_t* new_name = PUGIXML_TEXT("new_attr");
        size_t name_size = 0;
        if (Size > 0) {
            // Use a safe portion of input for name, ensuring no null terminator issues for size
            // We limit the size to avoid huge allocations
            size_t max_name_len = 100;
            size_t actual_len = (Size < max_name_len) ? Size : max_name_len;
            // Ensure we don't include null bytes in the name length if we treat it as string
            // For fuzzing, we just pass the buffer and a size
            new_name = reinterpret_cast<const pugi::char_t*>(Data);
            name_size = actual_len;
        }
        attr.set_name(new_name, name_size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_xml_attribute_methods(Data, Size);

    return 0;
}