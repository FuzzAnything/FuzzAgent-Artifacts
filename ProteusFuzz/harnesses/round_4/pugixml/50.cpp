// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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
#include <string>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;
    
    // Convert input to string, ensuring null termination
    std::string input(reinterpret_cast<const char*>(data), size);
    // Ensure valid UTF-8 by replacing invalid sequences if necessary, 
    // but for fuzzing we can just pass raw data and let parser handle errors
    if (!doc->load_string(input.c_str())) {
        delete doc;
        return nullptr;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;
    
    pugi::xml_node root = doc->root();
    if (!root) {
        delete doc;
        return 0;
    }
    
    // Sequence: child -> child -> insert_copy_after -> child -> insert_copy_after -> child -> child -> child -> child -> insert_copy_after -> child -> first_child -> name -> child -> insert_copy_after -> child -> first_child -> child -> last_child -> value -> child -> insert_copy_after -> child -> first_child -> child -> first_child -> value
    
    // 1. child
    pugi::xml_node n1 = root.child("test");
    if (!n1) n1 = root; // fallback to root if child not found
    
    // 2. child
    pugi::xml_node n2 = n1.child("test");
    if (!n2) n2 = n1;
    
    // 3. insert_copy_after
    pugi::xml_node n3 = n2.insert_copy_after(n2, n2);
    if (!n3) n3 = n2;
    
    // 4. child
    pugi::xml_node n4 = n3.child("test");
    if (!n4) n4 = n3;
    
    // 5. insert_copy_after
    pugi::xml_node n5 = n4.insert_copy_after(n4, n4);
    if (!n5) n5 = n4;
    
    // 6. child
    pugi::xml_node n6 = n5.child("test");
    if (!n6) n6 = n5;
    
    // 7. child
    pugi::xml_node n7 = n6.child("test");
    if (!n7) n7 = n6;
    
    // 8. child
    pugi::xml_node n8 = n7.child("test");
    if (!n8) n8 = n7;
    
    // 9. child
    pugi::xml_node n9 = n8.child("test");
    if (!n9) n9 = n8;
    
    // 10. insert_copy_after
    pugi::xml_node n10 = n9.insert_copy_after(n9, n9);
    if (!n10) n10 = n9;
    
    // 11. child
    pugi::xml_node n11 = n10.child("test");
    if (!n11) n11 = n10;
    
    // 12. first_child
    pugi::xml_node n12 = n11.first_child();
    if (!n12) n12 = n11;
    
    // 13. name
    const pugi::char_t* name13 = n12.name();
    (void)name13; // suppress unused warning
    
    // 14. child
    pugi::xml_node n14 = n12.child("test");
    if (!n14) n14 = n12;
    
    // 15. insert_copy_after
    pugi::xml_node n15 = n14.insert_copy_after(n14, n14);
    if (!n15) n15 = n14;
    
    // 16. child
    pugi::xml_node n16 = n15.child("test");
    if (!n16) n16 = n15;
    
    // 17. first_child
    pugi::xml_node n17 = n16.first_child();
    if (!n17) n17 = n16;
    
    // 18. child
    pugi::xml_node n18 = n17.child("test");
    if (!n18) n18 = n17;
    
    // 19. last_child
    pugi::xml_node n19 = n18.last_child();
    if (!n19) n19 = n18;
    
    // 20. value
    const pugi::char_t* value20 = n19.value();
    (void)value20;
    
    // 21. child
    pugi::xml_node n21 = n19.child("test");
    if (!n21) n21 = n19;
    
    // 22. insert_copy_after
    pugi::xml_node n22 = n21.insert_copy_after(n21, n21);
    if (!n22) n22 = n21;
    
    // 23. child
    pugi::xml_node n23 = n22.child("test");
    if (!n23) n23 = n22;
    
    // 24. first_child
    pugi::xml_node n24 = n23.first_child();
    if (!n24) n24 = n23;
    
    // 25. child
    pugi::xml_node n25 = n24.child("test");
    if (!n25) n25 = n24;
    
    // 26. first_child
    pugi::xml_node n26 = n25.first_child();
    if (!n26) n26 = n25;
    
    // 27. value
    const pugi::char_t* value27 = n26.value();
    (void)value27;
    
    delete doc;
    return 0;
}