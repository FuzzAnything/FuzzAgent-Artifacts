// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to create a minimal valid document to test traversal logic
        doc.append_child("root");
    }

    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        // If no root child found, try to get any child or use document root
        root = doc.first_child();
        if (root.empty()) {
            root = doc;
        }
    }

    // 1. pugi::xml_node::child
    pugi::xml_node n1 = root.child("test");
    // 2. pugi::xml_node::child
    pugi::xml_node n2 = n1.child("test");
    // 3. pugi::xml_node::insert_copy_after
    // Note: insert_copy_after is for attributes in the context of the provided description logic, 
    // but the signature provided is xml_node insert_copy_after(const xml_node& proto, const xml_node& node).
    // The description says "insert a deep copy of proto attribute after attr". 
    // However, the signature provided in the prompt is: xml_node insert_copy_after(const xml_node& proto, const xml_node& node);
    // This signature matches the method for inserting a child node copy after another child node.
    // Let's assume the prompt's signature is the one to use (inserting child node copy).
    // We need valid nodes. If n2 is empty, we might need to create nodes or handle empty cases.
    if (!n2.empty() && !n1.empty()) {
        // Try to insert a copy of n2 after n1 (if n1 is a child of the same parent)
        // This is a bit tricky without knowing the exact parent structure. 
        // Let's try to insert n2 after n1 if they are siblings, or just call it to trigger logic.
        // If n1 is empty, this might fail gracefully.
        n2 = n1.insert_copy_after(n2, n1);
    }
    
    // 4. pugi::xml_node::child
    pugi::xml_node n3 = root.child("test");
    // 5. pugi::xml_node::insert_copy_after
    if (!n3.empty()) {
        n3 = root.insert_copy_after(n3, root.first_child());
    }
    // 6. pugi::xml_node::child
    pugi::xml_node n4 = root.child("test");
    // 7. pugi::xml_node::child
    pugi::xml_node n5 = n4.child("test");
    // 8. pugi::xml_node::child
    pugi::xml_node n6 = n5.child("test");
    // 9. pugi::xml_node::child
    pugi::xml_node n7 = n6.child("test");
    // 10. pugi::xml_node::insert_copy_after
    if (!n7.empty() && !n6.empty()) {
        n7 = n6.insert_copy_after(n7, n6);
    }
    // 11. pugi::xml_node::child
    pugi::xml_node n8 = root.child("test");
    // 12. pugi::xml_node::first_child
    pugi::xml_node n9 = n8.first_child();
    // 13. pugi::xml_node::name
    const pugi::char_t* name_val = n9.name();
    (void)name_val;
    // 14. pugi::xml_node::child
    pugi::xml_node n10 = n9.child("test");
    // 15. pugi::xml_node::insert_copy_after
    if (!n10.empty()) {
        n10 = n9.insert_copy_after(n10, n9.first_child());
    }
    // 16. pugi::xml_node::child
    pugi::xml_node n11 = n10.child("test");
    // 17. pugi::xml_node::first_child
    pugi::xml_node n12 = n11.first_child();
    // 18. pugi::xml_node::child
    pugi::xml_node n13 = n12.child("test");
    // 19. pugi::xml_node::last_child
    pugi::xml_node n14 = n13.last_child();
    // 20. pugi::xml_node::value
    const pugi::char_t* value_val = n14.value();
    (void)value_val;
    // 21. pugi::xml_node::child
    pugi::xml_node n15 = n14.child("test");
    // 22. pugi::xml_node::insert_copy_after
    if (!n15.empty()) {
        n15 = n14.insert_copy_after(n15, n14.first_child());
    }
    // 23. pugi::xml_node::child
    pugi::xml_node n16 = n15.child("test");
    // 24. pugi::xml_node::first_child
    pugi::xml_node n17 = n16.first_child();
    // 25. pugi::xml_node::child
    pugi::xml_node n18 = n17.child("test");
    // 26. pugi::xml_node::first_child
    pugi::xml_node n19 = n18.first_child();
    // 27. pugi::xml_node::value
    const pugi::char_t* value_val2 = n19.value();
    (void)value_val2;

    return 0;
}