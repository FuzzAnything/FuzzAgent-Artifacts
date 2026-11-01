// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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

static void test_as_float(pugi::xml_attribute attr) {
    (void)attr.as_float();
    (void)attr.as_float(1.0f);
    (void)attr.as_float(-1.0f);
    (void)attr.as_float(3.14159f);
}

static void test_as_uint(pugi::xml_attribute attr) {
    (void)attr.as_uint();
    (void)attr.as_uint(1u);
    (void)attr.as_uint(0xFFFFFFFFu);
}

static void test_as_llong(pugi::xml_attribute attr) {
    (void)attr.as_llong();
    (void)attr.as_llong(1LL);
    (void)attr.as_llong(-1LL);
    (void)attr.as_llong(0x7FFFFFFFFFFFFFFFLL);
}

static void test_as_bool(pugi::xml_attribute attr) {
    (void)attr.as_bool();
    (void)attr.as_bool(true);
    (void)attr.as_bool(false);
}

static void test_as_ullong(pugi::xml_attribute attr) {
    (void)attr.as_ullong();
    (void)attr.as_ullong(1ULL);
    (void)attr.as_ullong(0xFFFFFFFFFFFFFFFFULL);
}

static void test_as_int(pugi::xml_attribute attr) {
    (void)attr.as_int();
    (void)attr.as_int(1);
    (void)attr.as_int(-1);
    (void)attr.as_int(0x7FFFFFFF);
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for string operations
    // We create a buffer with a null terminator to safely pass to load_string
    // or to construct XML strings.
    // Since load_string expects a null-terminated string, we must ensure termination.
    // If the input data contains a null byte, strlen would stop early, which is fine for load_string
    // as long as the buffer we pass is valid. However, the fuzzer input might not be null-terminated.
    // We will create a temporary buffer with a null terminator.
    
    // Max size check to avoid huge allocations
    if (Size > 1024 * 1024) {
        return 0;
    }

    // Create a null-terminated copy for parsing
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    // Test with a default constructed (empty) attribute
    pugi::xml_attribute empty_attr;
    test_as_float(empty_attr);
    test_as_uint(empty_attr);
    test_as_llong(empty_attr);
    test_as_bool(empty_attr);
    test_as_ullong(empty_attr);
    test_as_int(empty_attr);

    // If parsing was successful, iterate through attributes
    if (result.status == pugi::status_ok) {
        for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
            for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
                test_as_float(attr);
                test_as_uint(attr);
                test_as_llong(attr);
                test_as_bool(attr);
                test_as_ullong(attr);
                test_as_int(attr);
            }
        }
    }

    // Also try to create a minimal valid XML if the input is not valid XML
    // to ensure we test non-empty attributes with various values derived from input
    if (Size > 0) {
        pugi::xml_document doc2;
        // Construct a simple XML string using the input data as attribute values
        // We escape basic characters to ensure validity
        char buffer_safe[1024];
        size_t len = (Size < 1020) ? Size : 1020;
        // Simple sanitization to avoid breaking XML structure
        for (size_t i = 0; i < len; ++i) {
            char c = Data[i];
            if (c == '<' || c == '>' || c == '&' || c == '"' || c == '\'') {
                c = 'X'; 
            }
            buffer_safe[i] = c;
        }
        buffer_safe[len] = '\0';
        
        char xml_template[2048];
        snprintf(xml_template, sizeof(xml_template), 
            "<root attr_float=\"%s\" attr_uint=\"%s\" attr_llong=\"%s\" "
            "attr_bool=\"%s\" attr_ullong=\"%s\" attr_int=\"%s\"/>",
            buffer_safe, buffer_safe, buffer_safe, buffer_safe, buffer_safe, buffer_safe);

        if (doc2.load_string(xml_template)) {
            pugi::xml_node root = doc2.child("root");
            if (root) {
                pugi::xml_attribute attr_float = root.attribute("attr_float");
                pugi::xml_attribute attr_uint = root.attribute("attr_uint");
                pugi::xml_attribute attr_llong = root.attribute("attr_llong");
                pugi::xml_attribute attr_bool = root.attribute("attr_bool");
                pugi::xml_attribute attr_ullong = root.attribute("attr_ullong");
                pugi::xml_attribute attr_int = root.attribute("attr_int");

                test_as_float(attr_float);
                test_as_uint(attr_uint);
                test_as_llong(attr_llong);
                test_as_bool(attr_bool);
                test_as_ullong(attr_ullong);
                test_as_int(attr_int);
            }
        }
    }

    return 0;
}