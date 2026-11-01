// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
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
#include <string>

static bool try_parse_xml(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) return false;
    // Ensure the input is null-terminated for parsing
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    if (xml_str.empty()) return false;
    
    // Try to parse as UTF-8
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    return result.status == pugi::status_ok;
}

static void fuzz_attribute_methods(pugi::xml_attribute attr, const uint8_t* data, size_t size) {
    if (!attr) return;

    // Test as_float
    float default_float = 0.0f;
    if (size > 0) default_float = static_cast<float>(data[0]);
    attr.as_float(default_float);

    // Test as_uint
    unsigned int default_uint = 0;
    if (size > 1) default_uint = static_cast<unsigned int>(data[1]);
    attr.as_uint(default_uint);

    // Test set_value with float
    float set_val = 0.0f;
    if (size > 2) set_val = static_cast<float>(data[2]);
    attr.set_value(set_val);

    // Test as_bool
    bool default_bool = false;
    if (size > 3) default_bool = (data[3] != 0);
    attr.as_bool(default_bool);

    // Test as_ullong
    unsigned long long default_ullong = 0;
    if (size > 4) default_ullong = static_cast<unsigned long long>(data[4]);
    attr.as_ullong(default_ullong);

    // Test as_int
    int default_int = 0;
    if (size > 5) default_int = static_cast<int>(data[5]);
    attr.as_int(default_int);
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Attempt to parse the input as XML
    if (!try_parse_xml(Data, Size, doc)) {
        // If parsing fails, try to create a minimal valid XML structure with the data as attribute content
        // This ensures we still test the attribute functions even with malformed XML input
        doc.append_child("root");
        pugi::xml_node root = doc.child("root");
        if (!root) return 0;
        
        pugi::xml_node node = root.append_child("test");
        if (!node) return 0;
        
        pugi::xml_attribute attr = node.append_attribute("data");
        if (!attr) return 0;
        
        // Set attribute value from input data (as string)
        std::string val(reinterpret_cast<const char*>(Data), Size);
        // Escape special characters for XML attribute value
        std::string escaped;
        for (char c : val) {
            if (c == '&') escaped += "&amp;";
            else if (c == '<') escaped += "&lt;";
            else if (c == '>') escaped += "&gt;";
            else if (c == '"') escaped += "&quot;";
            else if (c == '\'') escaped += "&apos;";
            else escaped += c;
        }
        attr.set_value(escaped.c_str());
        
        fuzz_attribute_methods(attr, Data, Size);
        return 0;
    }

    // Traverse all attributes in the parsed document
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            fuzz_attribute_methods(attr, Data, Size);
        }
        // Also check nested nodes
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
                fuzz_attribute_methods(attr, Data, Size);
            }
        }
    }

    return 0;
}