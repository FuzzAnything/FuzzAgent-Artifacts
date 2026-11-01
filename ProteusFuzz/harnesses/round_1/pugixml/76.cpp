// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
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
#include <string>

static void fuzzer_test_as_llong(pugi::xml_text text_node, long long default_val) {
    (void)text_node.as_llong(default_val);
}

static void fuzzer_test_as_ullong(pugi::xml_text text_node, unsigned long long default_val) {
    (void)text_node.as_ullong(default_val);
}

static void fuzzer_test_as_int(pugi::xml_text text_node, int default_val) {
    (void)text_node.as_int(default_val);
}

static void fuzzer_test_as_bool(pugi::xml_text text_node, bool default_val) {
    (void)text_node.as_bool(default_val);
}

static void fuzzer_test_set_bool(pugi::xml_text text_node, bool value) {
    (void)text_node.set(value);
}

static void fuzzer_test_empty(pugi::xml_text text_node) {
    (void)text_node.empty();
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Create a valid XML document structure to ensure text nodes can be retrieved
    pugi::xml_node root = doc.append_child("root");
    pugi::xml_node child = root.append_child("child");
    pugi::xml_node text_node = child.append_child("text");
    
    // Use input data to set text content if possible, or use a dummy string
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for safety, though set handles it
    text_node.text().set(input_str.c_str());

    pugi::xml_text text_obj = text_node.text();

    // Test empty() on valid node
    fuzzer_test_empty(text_obj);

    // Test as_llong with various defaults derived from input
    long long def_llong = 0;
    if (Size >= sizeof(long long)) {
        std::memcpy(&def_llong, Data, sizeof(long long));
    }
    fuzzer_test_as_llong(text_obj, def_llong);

    // Test as_ullong with various defaults derived from input
    unsigned long long def_ullong = 0;
    if (Size >= sizeof(unsigned long long)) {
        std::memcpy(&def_ullong, Data, sizeof(unsigned long long));
    }
    fuzzer_test_as_ullong(text_obj, def_ullong);

    // Test as_int with various defaults derived from input
    int def_int = 0;
    if (Size >= sizeof(int)) {
        std::memcpy(&def_int, Data, sizeof(int));
    }
    fuzzer_test_as_int(text_obj, def_int);

    // Test as_bool with various defaults derived from input
    bool def_bool = false;
    if (Size >= sizeof(bool)) {
        def_bool = Data[0] != 0;
    }
    fuzzer_test_as_bool(text_obj, def_bool);

    // Test set(bool) with various values derived from input
    bool set_val = (Size > 0 && Data[0] != 0);
    fuzzer_test_set_bool(text_obj, set_val);

    // Test with an empty (default constructed) xml_text object to cover null paths
    pugi::xml_text empty_text;
    fuzzer_test_empty(empty_text);
    fuzzer_test_as_llong(empty_text, def_llong);
    fuzzer_test_as_ullong(empty_text, def_ullong);
    fuzzer_test_as_int(empty_text, def_int);
    fuzzer_test_as_bool(empty_text, def_bool);
    fuzzer_test_set_bool(empty_text, set_val);

    return 0;
}