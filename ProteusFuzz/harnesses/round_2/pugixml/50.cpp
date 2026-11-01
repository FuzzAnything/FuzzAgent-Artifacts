// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
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
#include <vector>

static std::string generate_xml_from_fuzz_data(const uint8_t* data, size_t size) {
    std::string result;
    result.reserve(size + 200);
    result += "<root>";
    for (size_t i = 0; i < size; ++i) {
        uint8_t c = data[i];
        if (c == '<' || c == '>' || c == '&' || c == '"' || c == '\'') {
            result += "\\x";
            char hex[4];
            sprintf(hex, "%02x", c);
            result += hex;
        } else if (c >= 32 && c < 127) {
            result += static_cast<char>(c);
        } else {
            result += "\\x";
            char hex[4];
            sprintf(hex, "%02x", c);
            result += hex;
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string xml_str = generate_xml_from_fuzz_data(Data, Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    if (!result) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Prepare name for child() calls
    std::string_view name_view = std::string_view("test");
    const char* name_ptr = name_view.data();

    // Invoke child() 8 times as per sequence
    pugi::xml_node n1 = root.child(name_ptr);
    pugi::xml_node n2 = n1.child(name_ptr);
    pugi::xml_node n3 = n2.child(name_ptr);
    pugi::xml_node n4 = n3.child(name_ptr);
    pugi::xml_node n5 = n4.child(name_ptr);
    pugi::xml_node n6 = n5.child(name_ptr);
    pugi::xml_node n7 = n6.child(name_ptr);
    pugi::xml_node n8 = n7.child(name_ptr);

    // Invoke children() 4 times
    pugi::xml_object_range<pugi::xml_named_node_iterator> r1 = root.children(name_ptr);
    pugi::xml_object_range<pugi::xml_named_node_iterator> r2 = n1.children(name_ptr);
    pugi::xml_object_range<pugi::xml_named_node_iterator> r3 = n2.children(name_ptr);
    pugi::xml_object_range<pugi::xml_named_node_iterator> r4 = n3.children(name_ptr);

    // Invoke end() 1 time
    auto it1 = r1.end();

    // Invoke first_child() 1 time
    pugi::xml_node fc1 = root.first_child();

    // Invoke end() 2 times
    auto it2 = r2.end();
    auto it3 = r3.end();

    // Invoke first_child() 1 time
    pugi::xml_node fc2 = n1.first_child();

    // Invoke last_child() 1 time
    pugi::xml_node lc1 = n2.last_child();

    // Invoke end() 6 times
    auto it4 = r4.end();
    auto it5 = r1.end();
    auto it6 = r2.end();
    auto it7 = r3.end();
    auto it8 = r4.end();
    auto it9 = r1.end();

    // Invoke first_child() 1 time
    pugi::xml_node fc3 = n3.first_child();

    // Invoke end() 2 times
    auto it10 = r2.end();
    auto it11 = r3.end();

    // Invoke begin() 1 time
    auto it12 = r4.begin();

    // Invoke end() 2 times
    auto it13 = r1.end();
    auto it14 = r2.end();

    // Invoke begin() 1 time
    auto it15 = r3.begin();

    (void)it1; (void)it2; (void)it3; (void)it4; (void)it5; (void)it6; (void)it7; (void)it8; (void)it9;
    (void)it10; (void)it11; (void)it12; (void)it13; (void)it14; (void)it15;
    (void)n1; (void)n2; (void)n3; (void)n4; (void)n5; (void)n6; (void)n7; (void)n8;
    (void)r1; (void)r2; (void)r3; (void)r4;
    (void)fc1; (void)fc2; (void)fc3; (void)lc1;

    return 0;
}