// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
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
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
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
#include <string>
#include <vector>
#include <cstring>

static void test_as_utf8(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a wide string from input data (interpreting bytes as wchar_t if possible)
    // We need to be careful about alignment and size
    if (size % sizeof(wchar_t) != 0) return;
    
    size_t wchar_count = size / sizeof(wchar_t);
    if (wchar_count == 0) return;
    
    std::wstring wide_str(wchar_count, L'\0');
    // Copy data, treating input as wchar_t sequence
    // This is a simplified approach; in real fuzzing we might construct valid wide strings
    for (size_t i = 0; i < wchar_count; ++i) {
        wide_str[i] = static_cast<wchar_t>(data[i * sizeof(wchar_t)]);
    }
    
    // Test as_utf8
    std::string utf8_result = pugi::as_utf8(wide_str);
    (void)utf8_result; // Suppress unused variable warning
}

static void test_as_wide(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a null-terminated string from input
    std::string narrow_str(reinterpret_cast<const char*>(data), size);
    
    // Test as_wide
    std::wstring wide_result = pugi::as_wide(narrow_str.c_str());
    (void)wide_result; // Suppress unused variable warning
}

static void test_attribute_methods(const uint8_t* data, size_t size) {
    if (size < 10) return;
    
    // Create a simple XML document with attributes
    std::string xml_str = "<root attr1=\"123\" attr2=\"45.67\" attr3=\"hello\" attr4=\"9223372036854775807\" attr5=\"invalid\"></root>";
    
    pugi::xml_document doc;
    if (!doc.load_string(xml_str.c_str())) {
        return;
    }
    
    pugi::xml_node root = doc.child("root");
    if (!root) return;
    
    // Get various attributes
    pugi::xml_attribute attr1 = root.attribute("attr1");
    pugi::xml_attribute attr2 = root.attribute("attr2");
    pugi::xml_attribute attr3 = root.attribute("attr3");
    pugi::xml_attribute attr4 = root.attribute("attr4");
    pugi::xml_attribute attr5 = root.attribute("attr5");
    pugi::xml_attribute missing = root.attribute("missing");
    
    // Test as_float with different defaults
    float f1 = attr1.as_float(0.0f);
    float f2 = attr2.as_float(-1.0f);
    float f3 = attr3.as_float(999.0f);
    float f4 = missing.as_float(42.0f);
    (void)f1; (void)f2; (void)f3; (void)f4;
    
    // Test as_double with different defaults
    double d1 = attr1.as_double(0.0);
    double d2 = attr2.as_double(-1.0);
    double d3 = attr3.as_double(999.0);
    double d4 = missing.as_double(42.0);
    (void)d1; (void)d2; (void)d3; (void)d4;
    
    // Test as_llong with different defaults
    long long ll1 = attr1.as_llong(0);
    long long ll2 = attr4.as_llong(-1);
    long long ll3 = attr3.as_llong(999);
    long long ll4 = missing.as_llong(42);
    (void)ll1; (void)ll2; (void)ll3; (void)ll4;
    
    // Test as_string with different defaults
    const pugi::char_t* s1 = attr1.as_string();
    const pugi::char_t* s2 = attr2.as_string("default");
    const pugi::char_t* s3 = missing.as_string("missing_default");
    (void)s1; (void)s2; (void)s3;
    
    // Test with fuzzed attribute values by creating dynamic XML
    if (size >= 20) {
        // Create a custom XML with fuzzed content
        std::string fuzzed_xml = "<root ";
        for (size_t i = 0; i < std::min(size - 10, 50UL); ++i) {
            fuzzed_xml += "attr";
            fuzzed_xml += static_cast<char>('0' + (data[i] % 10));
            fuzzed_xml += "=\"";
            // Add some fuzzed values
            for (size_t j = 0; j < 10 && i + j < size; ++j) {
                char c = data[i + j];
                if (c >= 32 && c < 127 && c != '"' && c != '>') {
                    fuzzed_xml += c;
                }
            }
            fuzzed_xml += "\" ";
        }
        fuzzed_xml += "></root>";
        
        pugi::xml_document fuzzed_doc;
        if (fuzzed_doc.load_string(fuzzed_xml.c_str())) {
            pugi::xml_node fuzzed_root = fuzzed_doc.child("root");
            if (fuzzed_root) {
                pugi::xml_attribute fuzzed_attr = fuzzed_root.attribute("attr0");
                if (fuzzed_attr) {
                    float f = fuzzed_attr.as_float(0.0f);
                    double d = fuzzed_attr.as_double(0.0);
                    long long ll = fuzzed_attr.as_llong(0);
                    const pugi::char_t* s = fuzzed_attr.as_string("");
                    (void)f; (void)d; (void)ll; (void)s;
                }
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test as_utf8
    test_as_utf8(Data, Size);
    
    // Test as_wide
    test_as_wide(Data, Size);
    
    // Test attribute methods
    test_attribute_methods(Data, Size);
    
    return 0;
}