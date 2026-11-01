// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    static pugi::xml_document doc;
    doc.reset();
    
    if (size == 0) {
        return nullptr;
    }
    
    // Ensure the input is null-terminated for parsing
    std::string str(reinterpret_cast<const char*>(data), size);
    // Add a null terminator if not present
    if (size > 0 && str.back() != '\0') {
        str += '\0';
    }
    
    pugi::xml_parse_result result = doc.load_string(str.c_str());
    if (!result) {
        return nullptr;
    }
    
    return &doc;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) {
        return 0;
    }
    
    pugi::xml_node root = *doc;
    
    // Sequence 1: child -> child -> insert_copy_before -> child -> insert_copy_before -> child
    pugi::xml_node n1 = root.child("root");
    if (!n1) n1 = root;
    
    pugi::xml_node n2 = n1.child("child1");
    if (!n2) n2 = n1;
    
    pugi::xml_node n3 = n2.child("target");
    if (!n3) n3 = n2;
    
    pugi::xml_node n4 = n2.insert_copy_before(n3, n2);
    
    pugi::xml_node n5 = n4.child("copy1");
    if (!n5) n5 = n4;
    
    pugi::xml_node n6 = n5.insert_copy_before(n3, n5);
    
    pugi::xml_node n7 = n6.child("copy2");
    if (!n7) n7 = n6;
    
    // Sequence 2: child -> child -> child -> child -> insert_copy_before -> child
    pugi::xml_node n8 = n7.child("deep1");
    if (!n8) n8 = n7;
    
    pugi::xml_node n9 = n8.child("deep2");
    if (!n9) n9 = n8;
    
    pugi::xml_node n10 = n9.child("deep3");
    if (!n10) n10 = n9;
    
    pugi::xml_node n11 = n10.child("deep4");
    if (!n11) n11 = n10;
    
    pugi::xml_node n12 = n11.insert_copy_before(n3, n11);
    
    pugi::xml_node n13 = n12.child("copy3");
    if (!n13) n13 = n12;
    
    // Sequence 3: first_child -> name -> child -> insert_copy_before -> child
    pugi::xml_node n14 = n13.first_child();
    
    const pugi::char_t* name1 = n14.name();
    (void)name1; // Suppress unused warning
    
    pugi::xml_node n15 = n14.child("next");
    if (!n15) n15 = n14;
    
    pugi::xml_node n16 = n15.insert_copy_before(n3, n15);
    
    pugi::xml_node n17 = n16.child("copy4");
    if (!n17) n17 = n16;
    
    // Sequence 4: first_child -> child -> last_child -> name -> child -> insert_copy_before -> child
    pugi::xml_node n18 = n17.first_child();
    
    pugi::xml_node n19 = n18.child("inner");
    if (!n19) n19 = n18;
    
    pugi::xml_node n20 = n19.last_child();
    
    const pugi::char_t* name2 = n20.name();
    (void)name2; // Suppress unused warning
    
    pugi::xml_node n21 = n20.child("final");
    if (!n21) n21 = n20;
    
    pugi::xml_node n22 = n21.insert_copy_before(n3, n21);
    
    pugi::xml_node n23 = n22.child("copy5");
    if (!n23) n23 = n22;
    
    // Sequence 5: first_child -> next_sibling -> child -> first_child -> value
    pugi::xml_node n24 = n23.first_child();
    
    pugi::xml_node n25 = n24.next_sibling();
    
    pugi::xml_node n26 = n25.child("sibling_child");
    if (!n26) n26 = n25;
    
    pugi::xml_node n27 = n26.first_child();
    
    const pugi::char_t* value = n27.value();
    (void)value; // Suppress unused warning
    
    return 0;
}