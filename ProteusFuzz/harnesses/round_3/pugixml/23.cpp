// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <string>

static std::string generate_xml_document(const uint8_t* data, size_t size) {
    std::string result;
    result.reserve(size + 200);
    result += "<root>";
    
    // Fuzz data as content, escaping special characters
    for (size_t i = 0; i < size; ++i) {
        uint8_t byte = data[i];
        if (byte == '<') {
            result += "&lt;";
        } else if (byte == '>') {
            result += "&gt;";
        } else if (byte == '&') {
            result += "&amp;";
        } else if (byte == '"') {
            result += "&quot;";
        } else if (byte == '\'') {
            result += "&apos;";
        } else if (byte >= 32 && byte < 127) {
            result += static_cast<char>(byte);
        } else {
            // Insert random tag names based on data to create structure
            if (i % 10 == 0) {
                result += "<tag";
                result += static_cast<char>(byte & 0x3F);
                result += " attr=\"";
                result += static_cast<char>(byte);
                result += "\">";
            } else if (i % 10 == 5) {
                result += "</tag>";
            }
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    std::string xml_str = generate_xml_document(Data, Size);
    
    if (!doc.load_string(xml_str.c_str())) {
        // If parsing fails, try a minimal valid XML to test API behavior on valid but simple structures
        if (!doc.load_string("<root><a><b/></a></root>")) {
            return 0;
        }
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Sequence: child -> child -> insert_copy_before -> child -> insert_copy_before -> child -> child -> child -> child -> insert_copy_before -> child -> first_child -> name -> child -> insert_copy_before -> child -> first_child -> child -> last_child -> name -> child -> insert_copy_before -> child -> first_child -> next_sibling -> child -> first_child -> value

    // 1. child
    pugi::xml_node n1 = root.child("a");
    // 2. child
    pugi::xml_node n2 = n1.child("b");
    
    // 3. insert_copy_before (proto, node)
    // We need a valid proto and a valid node to insert before.
    // If n2 is empty, we might not be able to insert before it safely in some contexts, 
    // but the API handles empty nodes by returning empty.
    // Let's use root as proto if n2 is empty, or n2 itself.
    pugi::xml_node proto1 = n2.empty() ? root : n2;
    pugi::xml_node target1 = n2.empty() ? root : n2;
    pugi::xml_node res1 = target1.insert_copy_before(proto1, target1);

    // 4. child
    pugi::xml_node n3 = res1.child("a");
    // 5. insert_copy_before
    pugi::xml_node proto2 = n3.empty() ? root : n3;
    pugi::xml_node target2 = n3.empty() ? root : n3;
    pugi::xml_node res2 = target2.insert_copy_before(proto2, target2);

    // 6. child
    pugi::xml_node n4 = res2.child("b");
    // 7. child
    pugi::xml_node n5 = n4.child("c");
    // 8. child
    pugi::xml_node n6 = n5.child("d");
    // 9. child
    pugi::xml_node n7 = n6.child("e");
    
    // 10. insert_copy_before
    pugi::xml_node proto3 = n7.empty() ? root : n7;
    pugi::xml_node target3 = n7.empty() ? root : n7;
    pugi::xml_node res3 = target3.insert_copy_before(proto3, target3);

    // 11. child
    pugi::xml_node n8 = res3.child("f");
    
    // 12. first_child
    pugi::xml_node n9 = n8.first_child();
    
    // 13. name
    const pugi::char_t* name1 = n9.name();
    (void)name1; // Suppress unused warning

    // 14. child
    pugi::xml_node n10 = n9.child("g");
    
    // 15. insert_copy_before
    pugi::xml_node proto4 = n10.empty() ? root : n10;
    pugi::xml_node target4 = n10.empty() ? root : n10;
    pugi::xml_node res4 = target4.insert_copy_before(proto4, target4);

    // 16. child
    pugi::xml_node n11 = res4.child("h");
    
    // 17. first_child
    pugi::xml_node n12 = n11.first_child();
    
    // 18. child
    pugi::xml_node n13 = n12.child("i");
    
    // 19. last_child
    pugi::xml_node n14 = n13.last_child();
    
    // 20. name
    const pugi::char_t* name2 = n14.name();
    (void)name2;

    // 21. child
    pugi::xml_node n15 = n14.child("j");
    
    // 22. insert_copy_before
    pugi::xml_node proto5 = n15.empty() ? root : n15;
    pugi::xml_node target5 = n15.empty() ? root : n15;
    pugi::xml_node res5 = target5.insert_copy_before(proto5, target5);

    // 23. child
    pugi::xml_node n16 = res5.child("k");
    
    // 24. first_child
    pugi::xml_node n17 = n16.first_child();
    
    // 25. next_sibling
    pugi::xml_node n18 = n17.next_sibling();
    
    // 26. child
    pugi::xml_node n19 = n18.child("l");
    
    // 27. first_child
    pugi::xml_node n20 = n19.first_child();
    
    // 28. value
    const pugi::char_t* val = n20.value();
    (void)val;

    return 0;
}