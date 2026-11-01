// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include <algorithm>
#include <pugixml.hpp>

static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Ensure the input is null-terminated before passing to load_string
    // load_string expects a C-string (null-terminated)
    std::string safe_xml(reinterpret_cast<const char*>(data), size);
    
    // Try to parse as UTF-8
    pugi::xml_parse_result result = doc->load_string(safe_xml.c_str());
    if (!result) {
        // If parsing fails, create a minimal valid document to allow API testing
        doc->reset();
        doc->append_child("root");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) return 0;

    // Ensure we have a root element to work with
    pugi::xml_node root = doc->first_child();
    if (!root) {
        root = doc->append_child("root");
    }

    // 1. pugi::xml_node::child
    // Try to get a child. We use a name derived from input or a default.
    // Limit string length to avoid huge allocations or invalid pointers if data is not null-terminated
    // We construct a temporary string to ensure null-termination for the API
    size_t safe_len = (Size > 20) ? 20 : Size;
    std::string safe_child_name(reinterpret_cast<const char*>(Data), safe_len);
    if (safe_child_name.empty()) safe_child_name = "child";
    
    pugi::xml_node child_node = root.child(safe_child_name.c_str());
    
    // If child doesn't exist, create one to ensure we have a valid element for attributes
    if (!child_node) {
        child_node = root.append_child(safe_child_name.c_str());
    }

    // 2. pugi::xml_node::append_attribute (1st call)
    std::string attr1_name = "attr1";
    if (Size > 0) {
        size_t len1 = (Size > 10) ? 10 : Size;
        attr1_name = std::string(reinterpret_cast<const char*>(Data), len1);
    }
    pugi::xml_attribute attr1 = child_node.append_attribute(attr1_name.c_str());

    // 3. pugi::xml_node::append_attribute (2nd call)
    std::string attr2_name = "attr2";
    if (Size > 10) {
        size_t len2 = (Size - 10 > 10) ? 10 : (Size - 10);
        attr2_name = std::string(reinterpret_cast<const char*>(Data + 10), len2);
    }
    pugi::xml_attribute attr2 = child_node.append_attribute(attr2_name.c_str());

    // 4. pugi::xml_attribute::set_value (long overload)
    if (attr1) {
        long val_long = 0;
        if (Size >= sizeof(long)) {
            val_long = *reinterpret_cast<const long*>(Data);
        }
        attr1.set_value(val_long);
    }

    // 5. pugi::xml_node::append_attribute (3rd call)
    std::string attr3_name = "attr3";
    if (Size > 20) {
        size_t len3 = (Size - 20 > 10) ? 10 : (Size - 20);
        attr3_name = std::string(reinterpret_cast<const char*>(Data + 20), len3);
    }
    pugi::xml_attribute attr3 = child_node.append_attribute(attr3_name.c_str());

    // 6. pugi::xml_node::append_attribute (4th call)
    std::string attr4_name = "attr4";
    if (Size > 30) {
        size_t len4 = (Size - 30 > 10) ? 10 : (Size - 30);
        attr4_name = std::string(reinterpret_cast<const char*>(Data + 30), len4);
    }
    pugi::xml_attribute attr4 = child_node.append_attribute(attr4_name.c_str());

    // 7. pugi::xml_attribute::set_value (unsigned long overload)
    if (attr4) {
        unsigned long val_ulong = 0;
        if (Size >= sizeof(unsigned long)) {
            val_ulong = *reinterpret_cast<const unsigned long*>(Data);
        }
        attr4.set_value(val_ulong);
    }

    delete doc;
    return 0;
}