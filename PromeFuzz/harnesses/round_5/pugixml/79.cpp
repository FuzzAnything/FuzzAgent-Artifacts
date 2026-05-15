// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
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
#include <cstring>

static pugi::xml_node create_test_node(pugi::xml_document& doc, bool with_text) {
    pugi::xml_node node = doc.append_child("test_node");
    if (with_text) {
        node.append_child(pugi::node_pcdata).set_value("123");
    }
    return node;
}

static pugi::xml_text get_test_text(pugi::xml_document& doc, bool with_text) {
    pugi::xml_node node = create_test_node(doc, with_text);
    return node.text();
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Initialize pugixml document
    pugi::xml_document doc;
    
    // Test xml_text::set overloads
    {
        pugi::xml_text text1 = get_test_text(doc, false);
        pugi::xml_text text2 = get_test_text(doc, true);
        
        // Use fuzzer data to determine which set overload to call and with what value
        if (Size >= 1) {
            uint8_t choice = Data[0] % 3;
            switch (choice) {
                case 0: {
                    bool val = (Data[0] & 0x01) != 0;
                    text1.set(val);
                    text2.set(val);
                    break;
                }
                case 1: {
                    int val = 0;
                    if (Size >= sizeof(int) + 1) {
                        memcpy(&val, Data + 1, sizeof(int));
                    } else if (Size >= 2) {
                        val = static_cast<int>(Data[1]);
                    }
                    text1.set(val);
                    text2.set(val);
                    break;
                }
                default:
                    // Do nothing for this iteration
                    break;
            }
        }
    }
    
    // Test xml_text::empty
    {
        pugi::xml_text text1 = get_test_text(doc, false);
        pugi::xml_text text2 = get_test_text(doc, true);
        
        bool empty1 = text1.empty();
        bool empty2 = text2.empty();
        (void)empty1;
        (void)empty2;
    }
    
    // Test xml_text::as_int
    {
        pugi::xml_text text1 = get_test_text(doc, false);
        pugi::xml_text text2 = get_test_text(doc, true);
        
        int default_val = 0;
        if (Size >= 1) {
            default_val = static_cast<int>(Data[0]);
        }
        
        int val1 = text1.as_int(default_val);
        int val2 = text2.as_int(default_val);
        int val3 = text1.as_int();
        int val4 = text2.as_int();
        (void)val1;
        (void)val2;
        (void)val3;
        (void)val4;
    }
    
    // Test xml_node::empty
    {
        pugi::xml_node empty_node;
        pugi::xml_node non_empty_node = create_test_node(doc, true);
        
        bool empty1 = empty_node.empty();
        bool empty2 = non_empty_node.empty();
        (void)empty1;
        (void)empty2;
    }
    
    // Test xml_text::as_bool
    {
        pugi::xml_text text1 = get_test_text(doc, false);
        pugi::xml_text text2 = get_test_text(doc, true);
        
        bool default_val = false;
        if (Size >= 1) {
            default_val = (Data[0] & 0x01) != 0;
        }
        
        bool val1 = text1.as_bool(default_val);
        bool val2 = text2.as_bool(default_val);
        bool val3 = text1.as_bool();
        bool val4 = text2.as_bool();
        (void)val1;
        (void)val2;
        (void)val3;
        (void)val4;
    }
    
    // Cleanup happens automatically when objects go out of scope
    return 0;
}