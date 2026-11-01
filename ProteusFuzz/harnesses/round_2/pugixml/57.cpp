// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <climits>

static void cleanup_xml_document(pugi::xml_document& doc) {
    // pugixml handles memory automatically, but we ensure document is empty
    doc.reset();
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Load XML from fuzz input
    // We try to parse the input as XML. If it fails, we create a minimal document to test empty states.
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, create a minimal valid document to ensure we can still test the API on empty nodes
        doc.append_child("root");
    }

    // Create a text node object. 
    // Note: xml_text is a lightweight wrapper. We need to get a valid text node from the document 
    // to test the methods effectively, or test the default constructor state (empty node).
    
    // Test 1: Default constructed xml_text (empty/null node)
    pugi::xml_text empty_text;
    
    // Test as_float on empty node
    float f_val = empty_text.as_float(1.5f);
    (void)f_val;
    
    // Test as_int on empty node
    int i_val = empty_text.as_int(42);
    (void)i_val;
    
    // Test as_string on empty node
    const pugi::char_t* s_val = empty_text.as_string("default");
    (void)s_val;
    
    // Test as_ullong on empty node
    unsigned long long ull_val = empty_text.as_ullong(100ULL);
    (void)ull_val;
    
    // Test as_bool on empty node
    bool b_val = empty_text.as_bool(true);
    (void)b_val;

    // Test 2: Get a text node from the document if available
    pugi::xml_node root = doc.child("root");
    if (root) {
        pugi::xml_text text_node = root.text();
        
        // Test set with various data derived from fuzz input
        // We use a subset of the input data as the string to set
        size_t max_len = Size > 1000 ? 1000 : Size;
        if (max_len > 0) {
            // Ensure null termination for safety if we were using C-strings, 
            // but set(size_t size) takes size explicitly.
            text_node.set(reinterpret_cast<const pugi::char_t*>(Data), max_len);
        }
        
        // After setting, test the conversion functions
        float f2 = text_node.as_float(0.0f);
        (void)f2;
        
        int i2 = text_node.as_int(0);
        (void)i2;
        
        const pugi::char_t* s2 = text_node.as_string("fallback");
        (void)s2;
        
        unsigned long long ull2 = text_node.as_ullong(0ULL);
        (void)ull2;
        
        bool b2 = text_node.as_bool(false);
        (void)b2;
    }

    // Cleanup
    cleanup_xml_document(doc);

    return 0;
}