// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
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

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;
    
    // Try to parse as XML
    if (size > 0) {
        // Ensure the data is null-terminated for load_string
        // We create a temporary buffer with a null terminator
        std::string temp_str(reinterpret_cast<const char*>(data), size);
        // Add a null terminator explicitly
        temp_str.push_back('\0');
        
        pugi::xml_parse_result result = doc->load_string(temp_str.c_str());
        if (!result) {
            // If parsing fails, create a simple valid XML with the data as text
            // to ensure we have a text node to test
            pugi::xml_node root = doc->append_child("root");
            if (root) {
                // Create a text node with the raw data (handling null bytes carefully)
                std::string safe_data(reinterpret_cast<const char*>(data), size);
                // Replace null bytes to ensure valid string for XML
                for (size_t i = 0; i < safe_data.size(); ++i) {
                    if (safe_data[i] == '\0') safe_data[i] = ' ';
                }
                root.text().set(safe_data.c_str());
            }
        }
    } else {
        // Empty input: create a document with an empty text node
        pugi::xml_node root = doc->append_child("root");
        if (root) {
            root.text().set("");
        }
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;
    
    // Get the text node from the root
    pugi::xml_node root = doc->first_child();
    if (!root) {
        delete doc;
        return 0;
    }
    
    pugi::xml_text text_node = root.text();
    
    // Invoke the target functions in the specified order
    // 1. as_string
    const pugi::char_t* str_val = text_node.as_string(PUGIXML_TEXT("default"));
    (void)str_val; // Suppress unused variable warning
    
    // 2. as_int
    int int_val = text_node.as_int(0);
    (void)int_val;
    
    // 3. as_uint
    unsigned int uint_val = text_node.as_uint(0);
    (void)uint_val;
    
    // 4. as_double
    double double_val = text_node.as_double(0.0);
    (void)double_val;
    
    // 5. as_float
    float float_val = text_node.as_float(0.0f);
    (void)float_val;
    
    // 6. as_bool
    bool bool_val = text_node.as_bool(false);
    (void)bool_val;
    
    // 7. as_llong
    long long llong_val = text_node.as_llong(0);
    (void)llong_val;
    
    // 8. as_ullong
    unsigned long long ullong_val = text_node.as_ullong(0);
    (void)ullong_val;
    
    // Cleanup
    delete doc;
    return 0;
}