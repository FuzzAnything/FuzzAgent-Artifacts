// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
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
#include <cstring>

static pugi::xml_document* create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    if (size == 0) {
        return nullptr;
    }
    // Ensure the data is null-terminated for parsing
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Append a null terminator if not present
    if (xml_str.back() != '\0') {
        xml_str += '\0';
    }
    
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    if (!result) {
        delete doc;
        return nullptr;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document* doc = create_document_from_fuzz_data(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->root();
    if (!root) {
        delete doc;
        return 0;
    }

    // 1. pugi::xml_node::child
    // We try to find a child. If it doesn't exist, we get an empty node, which is safe.
    // We use a simple string from the input if possible, or a default.
    const char* child_name = reinterpret_cast<const char*>(Data);
    pugi::xml_node target_node = root.child(child_name);
    
    // If child not found, try to create a child manually to ensure we have a valid node to work on
    if (!target_node) {
        target_node = root.append_child("test_child");
        if (!target_node) {
            delete doc;
            return 0;
        }
    }

    // 2. pugi::xml_node::append_attribute
    pugi::xml_attribute attr1 = target_node.append_attribute("attr1");
    if (!attr1) {
        delete doc;
        return 0;
    }

    // 3. pugi::xml_attribute::set_value (3 overloads)
    // Overload 1: const char_t*
    const char* val_str = reinterpret_cast<const char*>(Data + 1);
    if (Size > 1) {
        // Ensure null termination for safety in case input isn't
        std::string safe_val(val_str, Size - 1);
        safe_val += '\0';
        attr1.set_value(safe_val.c_str());
    } else {
        attr1.set_value("default");
    }

    // Overload 2: int
    int val_int = 0;
    if (Size >= 4) {
        val_int = *reinterpret_cast<const int*>(Data);
    }
    attr1.set_value(val_int);

    // Overload 3: unsigned int
    unsigned int val_uint = 0;
    if (Size >= 4) {
        val_uint = *reinterpret_cast<const unsigned int*>(Data);
    }
    attr1.set_value(val_uint);

    // 4. pugi::xml_node::append_attribute
    pugi::xml_attribute attr2 = target_node.append_attribute("attr2");
    if (!attr2) {
        delete doc;
        return 0;
    }

    // 5. pugi::xml_node::append_attribute
    pugi::xml_attribute attr3 = target_node.append_attribute("attr3");
    if (!attr3) {
        delete doc;
        return 0;
    }

    // 6. pugi::xml_attribute::set_value (3 overloads)
    // Overload 1: const char_t*
    if (Size > 5) {
        std::string safe_val2(reinterpret_cast<const char*>(Data + 5), Size - 5);
        safe_val2 += '\0';
        attr2.set_value(safe_val2.c_str());
    } else {
        attr2.set_value("default2");
    }
    
    // Overload 2: int
    int val_int2 = 100;
    if (Size >= 4) {
        val_int2 = *reinterpret_cast<const int*>(Data + 4);
    }
    attr2.set_value(val_int2);

    // Overload 3: unsigned int
    unsigned int val_uint2 = 200;
    if (Size >= 4) {
        val_uint2 = *reinterpret_cast<const unsigned int*>(Data + 4);
    }
    attr2.set_value(val_uint2);

    // 7. pugi::xml_node::append_attribute
    pugi::xml_attribute attr4 = target_node.append_attribute("attr4");
    if (!attr4) {
        delete doc;
        return 0;
    }

    // 8. pugi::xml_node::append_attribute
    pugi::xml_attribute attr5 = target_node.append_attribute("attr5");
    if (!attr5) {
        delete doc;
        return 0;
    }

    // 9. pugi::xml_attribute::set_value (3 overloads)
    // Overload 1: const char_t*
    if (Size > 10) {
        std::string safe_val3(reinterpret_cast<const char*>(Data + 10), Size - 10);
        safe_val3 += '\0';
        attr3.set_value(safe_val3.c_str());
    } else {
        attr3.set_value("default3");
    }

    // Overload 2: int
    int val_int3 = 300;
    if (Size >= 4) {
        val_int3 = *reinterpret_cast<const int*>(Data + 8);
    }
    attr3.set_value(val_int3);

    // Overload 3: unsigned int
    unsigned int val_uint3 = 400;
    if (Size >= 4) {
        val_uint3 = *reinterpret_cast<const unsigned int*>(Data + 8);
    }
    attr3.set_value(val_uint3);

    delete doc;
    return 0;
}