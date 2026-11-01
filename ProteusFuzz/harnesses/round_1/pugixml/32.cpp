// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

static pugi::xml_document* create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Convert fuzz data to a string, ensuring null termination
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Ensure the string is null-terminated for pugi
    xml_str += '\0';

    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    if (!result) {
        // Even if parsing fails, we might still have a document structure to test with
        // or we can create a minimal valid document
        delete doc;
        doc = new pugi::xml_document();
        doc->append_child("root");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_fuzz_data(Data, Size);
    if (!doc) return 0;

    try {
        // Get the root node to work with
        pugi::xml_node root = doc->first_child();
        if (!root) {
            // If no root, try to create one or use the document itself if it's an element
            root = *doc;
            if (root.type() != pugi::node_element) {
                root = doc->append_child("root");
            }
        }

        // 1. pugi::xml_node::child
        // We use a string_view as hinted, but child expects const char_t*. 
        // We'll construct a temporary string from fuzz data if possible, or use a fixed name.
        std::string_view a_str(reinterpret_cast<const char*>(Data), Size);
        // Ensure null termination for the string_view content if we pass it as C-string
        // However, child takes const char_t*. We can't pass string_view directly.
        // We will use a safe subset or a fixed name if the data is not safe.
        const char* child_name = nullptr;
        if (Size > 0 && Data[Size-1] == '\0') {
            child_name = reinterpret_cast<const char*>(Data);
        } else {
            // Fallback to a safe name
            child_name = "test_node";
        }
        
        pugi::xml_node child_node = root.child(child_name);

        // 2. pugi::xml_node::append_attribute (First call)
        // Use a name derived from fuzz data or a safe default
        const char* attr_name_1 = (Size > 5 && Data[Size-1] == '\0') ? reinterpret_cast<const char*>(Data) : "attr1";
        pugi::xml_attribute attr1 = child_node.append_attribute(attr_name_1);

        // 3. pugi::xml_node::append_attribute (Second call)
        const char* attr_name_2 = (Size > 10 && Data[Size-1] == '\0') ? reinterpret_cast<const char*>(Data + 5) : "attr2";
        pugi::xml_attribute attr2 = child_node.append_attribute(attr_name_2);

        // 4. pugi::xml_attribute::set_value (Overload 1: unsigned long long)
        // We need a valid attribute. If attr1 is empty, try attr2.
        pugi::xml_attribute target_attr_1 = attr1;
        if (!target_attr_1) target_attr_1 = attr2;
        
        if (target_attr_1) {
            // Generate a number from fuzz data
            unsigned long long val1 = 0;
            if (Size >= sizeof(unsigned long long)) {
                val1 = *reinterpret_cast<const unsigned long long*>(Data);
            } else if (Size > 0) {
                val1 = Data[0];
            }
            target_attr_1.set_value(val1);
        }

        // 5. pugi::xml_node::append_attribute (Third call)
        const char* attr_name_3 = "attr3";
        pugi::xml_attribute attr3 = child_node.append_attribute(attr_name_3);

        // 6. pugi::xml_node::append_attribute (Fourth call)
        const char* attr_name_4 = "attr4";
        pugi::xml_attribute attr4 = child_node.append_attribute(attr_name_4);

        // 7. pugi::xml_attribute::set_value (Overload 2: long long)
        pugi::xml_attribute target_attr_2 = attr3;
        if (!target_attr_2) target_attr_2 = attr4;

        if (target_attr_2) {
            // Generate a signed number from fuzz data
            long long val2 = 0;
            if (Size >= sizeof(long long)) {
                val2 = *reinterpret_cast<const long long*>(Data);
            } else if (Size > 0) {
                val2 = static_cast<signed char>(Data[0]);
            }
            target_attr_2.set_value(val2);
        }

    } catch (...) {
        // Catch any unexpected exceptions
    }

    delete doc;
    return 0;
}