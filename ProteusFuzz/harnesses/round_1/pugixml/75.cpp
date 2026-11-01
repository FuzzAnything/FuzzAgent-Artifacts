// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a document and parse input data
    pugi::xml_document doc;
    
    // Ensure the input is valid UTF-8 or at least null-terminated for parsing
    // We wrap the input in a dummy root tag to ensure valid XML structure
    // If Size is too small to hold a minimal tag, skip
    if (Size < 4) {
        return 0;
    }

    // Construct a safe buffer for XML parsing
    // We prepend "<root>" and append "</root>" to the fuzzed data
    // We need to handle the case where the data contains null bytes, which is fine for pugixml
    // but we must ensure the buffer is large enough.
    size_t header_len = 6; // "<root>"
    size_t footer_len = 7; // "</root>"
    size_t total_len = header_len + Size + footer_len;
    
    // Limit size to prevent allocation issues
    if (total_len > 1024 * 1024) {
        return 0;
    }

    char* buffer = new char[total_len + 1];
    if (!buffer) {
        return 0;
    }

    memcpy(buffer, "<root>", header_len);
    memcpy(buffer + header_len, Data, Size);
    memcpy(buffer + header_len + Size, "</root>", footer_len);
    buffer[total_len] = '\0';

    pugi::xml_parse_result result = doc.load_string(buffer);
    delete[] buffer;

    if (!result) {
        // If parsing fails, we can still try to test with an empty document or skip
        // However, to test the API robustness, we proceed even if parsing failed partially
        // but we need a valid node to attach attributes to.
        // If the root is missing, we create one.
        if (!doc.root()) {
            doc.append_child("root");
        }
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Create a child node to attach attributes to
    pugi::xml_node child = root.append_child("test_node");
    if (!child) {
        return 0;
    }

    // Extract potential attribute values from the fuzzed data
    // We will try to set attributes using substrings of the input data
    // to trigger various parsing paths in as_float, as_uint, etc.
    
    // Helper lambda to safely set an attribute
    auto set_attr = [&](const char* name, const char* value) {
        if (child.attribute(name)) {
            child.attribute(name).set_value(value);
        } else {
            child.append_attribute(name).set_value(value);
        }
    };

    // Generate some test attributes from the input data
    // We use the input data as attribute values to test parsing logic
    // We ensure null termination for the attribute values
    char attr_val[256];
    size_t val_len = (Size < 255) ? Size : 255;
    memcpy(attr_val, Data, val_len);
    attr_val[val_len] = '\0';

    // Set attributes with the fuzzed data
    set_attr("float_attr", attr_val);
    set_attr("uint_attr", attr_val);
    set_attr("llong_attr", attr_val);
    set_attr("bool_attr", attr_val);
    set_attr("ullong_attr", attr_val);
    set_attr("int_attr", attr_val);

    // Also test with empty string and specific edge cases
    child.append_attribute("empty_float").set_value("");
    child.append_attribute("empty_uint").set_value("");
    child.append_attribute("empty_llong").set_value("");
    child.append_attribute("empty_bool").set_value("");
    child.append_attribute("empty_ullong").set_value("");
    child.append_attribute("empty_int").set_value("");

    // Retrieve attributes
    pugi::xml_attribute float_attr = child.attribute("float_attr");
    pugi::xml_attribute uint_attr = child.attribute("uint_attr");
    pugi::xml_attribute llong_attr = child.attribute("llong_attr");
    pugi::xml_attribute bool_attr = child.attribute("bool_attr");
    pugi::xml_attribute ullong_attr = child.attribute("ullong_attr");
    pugi::xml_attribute int_attr = child.attribute("int_attr");

    // Fuzz the API functions with various default values
    // We use the first byte of input to seed default values for diversity
    int seed = 0;
    if (Size > 0) seed = Data[0];
    if (Size > 1) seed += Data[1];
    
    // Test as_float
    float f1 = float_attr.as_float();
    float f2 = float_attr.as_float(static_cast<float>(seed));
    float f3 = float_attr.as_float(3.14f);
    float f4 = float_attr.as_float(-1.5f);
    (void)f1; (void)f2; (void)f3; (void)f4;

    // Test as_uint
    unsigned int u1 = uint_attr.as_uint();
    unsigned int u2 = uint_attr.as_uint(static_cast<unsigned int>(seed));
    unsigned int u3 = uint_attr.as_uint(0xFFFFFFFFu);
    (void)u1; (void)u2; (void)u3;

    // Test as_llong
    long long ll1 = llong_attr.as_llong();
    long long ll2 = llong_attr.as_llong(static_cast<long long>(seed));
    long long ll3 = llong_attr.as_llong(0x7FFFFFFFFFFFFFFFLL);
    long long ll4 = llong_attr.as_llong(-0x7FFFFFFFFFFFFFFFLL);
    (void)ll1; (void)ll2; (void)ll3; (void)ll4;

    // Test as_bool
    bool b1 = bool_attr.as_bool();
    bool b2 = bool_attr.as_bool(seed % 2 != 0);
    bool b3 = bool_attr.as_bool(true);
    bool b4 = bool_attr.as_bool(false);
    (void)b1; (void)b2; (void)b3; (void)b4;

    // Test as_ullong
    unsigned long long ull1 = ullong_attr.as_ullong();
    unsigned long long ull2 = ullong_attr.as_ullong(static_cast<unsigned long long>(seed));
    unsigned long long ull3 = ullong_attr.as_ullong(0xFFFFFFFFFFFFFFFFULL);
    (void)ull1; (void)ull2; (void)ull3;

    // Test as_int
    int i1 = int_attr.as_int();
    int i2 = int_attr.as_int(seed);
    int i3 = int_attr.as_int(2147483647);
    int i4 = int_attr.as_int(-2147483648);
    (void)i1; (void)i2; (void)i3; (void)i4;

    // Test with null attributes (attributes that don't exist)
    pugi::xml_attribute null_attr = child.attribute("non_existent_attr");
    
    float nf1 = null_attr.as_float();
    float nf2 = null_attr.as_float(999.0f);
    unsigned int nu1 = null_attr.as_uint();
    unsigned int nu2 = null_attr.as_uint(123);
    long long nll1 = null_attr.as_llong();
    long long nll2 = null_attr.as_llong(-123);
    bool nb1 = null_attr.as_bool();
    bool nb2 = null_attr.as_bool(true);
    unsigned long long null1 = null_attr.as_ullong();
    unsigned long long null2 = null_attr.as_ullong(456);
    int ni1 = null_attr.as_int();
    int ni2 = null_attr.as_int(789);

    (void)nf1; (void)nf2;
    (void)nu1; (void)nu2;
    (void)nll1; (void)nll2;
    (void)nb1; (void)nb2;
    (void)null1; (void)null2;
    (void)ni1; (void)ni2;

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}