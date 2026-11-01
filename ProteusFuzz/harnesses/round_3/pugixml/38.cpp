// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    bool loaded = false;
    
    // Ensure the input is null-terminated for load_string
    // We create a temporary buffer with a null terminator
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    // Try loading from string first
    if (doc.load_string(buffer.data())) {
        loaded = true;
    }

    // If string parsing fails, try loading from file
    if (!loaded) {
        write_dummy_file(Data, Size);
        if (doc.load_file("./dummy_file")) {
            loaded = true;
        }
    }

    // If still not loaded, create a minimal document
    if (!loaded) {
        doc.append_child("root");
    }

    pugi::xml_node root = doc.first_element_by_path("/");
    if (!root) {
        root = doc.child("root");
    }
    if (!root) {
        root = doc.append_child("root");
    }

    // First prepend_attribute (const char_t*)
    pugi::xml_attribute attr1 = root.prepend_attribute("attr1");
    
    // Second prepend_attribute (string_view_t)
    std::string_view sv = "attr2";
    pugi::xml_attribute attr2 = root.prepend_attribute(sv);
    
    // First child
    pugi::xml_node child1 = root.child("child1");
    
    // Third prepend_attribute (const char_t*)
    // Only call if child1 is valid to avoid undefined behavior on empty nodes
    if (child1) {
        pugi::xml_attribute attr3 = child1.prepend_attribute("attr3");
    }
    
    // Second child
    pugi::xml_node child2 = child1.child("child2");
    
    // Fourth prepend_attribute (string_view_t)
    std::string_view sv2 = "attr4";
    if (child2) {
        pugi::xml_attribute attr4 = child2.prepend_attribute(sv2);
    }
    
    // Third child
    pugi::xml_node child3 = child2.child("child3");
    
    // Fourth child
    pugi::xml_node child4 = child3.child("child4");
    
    // Fifth prepend_attribute (const char_t*)
    if (child4) {
        pugi::xml_attribute attr5 = child4.prepend_attribute("attr5");
    }
    
    // Fifth child
    pugi::xml_node child5 = child4.child("child5");
    
    // Sixth child
    pugi::xml_node child6 = child5.child("child6");
    
    // Sixth prepend_attribute (string_view_t)
    std::string_view sv3 = "attr6";
    if (child6) {
        pugi::xml_attribute attr6 = child6.prepend_attribute(sv3);
    }

    return 0;
}