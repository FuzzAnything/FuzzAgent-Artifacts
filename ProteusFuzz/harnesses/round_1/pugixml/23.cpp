// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <fstream>
#include <vector>

static bool parse_xml_from_fuzz_data(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    
    // Ensure the data is null-terminated for C-string parsing
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';
    
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    if (!parse_xml_from_fuzz_data(Data, Size, doc)) {
        // If parsing fails, try to create a minimal valid XML structure to test API behavior
        doc.append_child("root");
    }

    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        // If no root, try to create one or use the document itself
        root = doc;
        if (root.empty()) {
            root = doc.append_child("root");
        }
    }

    // Sequence of API calls as specified
    // 1. pugi::xml_node::child
    pugi::xml_node n1 = root.child("child1");
    if (n1.empty()) {
        n1 = root.append_child("child1");
    }

    // 2. pugi::xml_node::child
    pugi::xml_node n2 = n1.child("child2");
    if (n2.empty()) {
        n2 = n1.append_child("child2");
    }

    // 3. pugi::xml_node::insert_copy_before
    // Need a prototype and a target node
    pugi::xml_node proto = n2;
    pugi::xml_node target = n1.child("child3");
    if (target.empty()) {
        target = n1.append_child("child3");
    }
    pugi::xml_node n3 = n1.insert_copy_before(proto, target);

    // 4. pugi::xml_node::child
    pugi::xml_node n4 = n3.child("child4");
    if (n4.empty()) {
        n4 = n3.append_child("child4");
    }

    // 5. pugi::xml_node::insert_copy_before
    pugi::xml_node n5 = n3.insert_copy_before(n4, n4);

    // 6. pugi::xml_node::child
    pugi::xml_node n6 = n5.child("child6");
    if (n6.empty()) {
        n6 = n5.append_child("child6");
    }

    // 7. pugi::xml_node::child
    pugi::xml_node n7 = n6.child("child7");
    if (n7.empty()) {
        n7 = n6.append_child("child7");
    }

    // 8. pugi::xml_node::child
    pugi::xml_node n8 = n7.child("child8");
    if (n8.empty()) {
        n8 = n7.append_child("child8");
    }

    // 9. pugi::xml_node::child
    pugi::xml_node n9 = n8.child("child9");
    if (n9.empty()) {
        n9 = n8.append_child("child9");
    }

    // 10. pugi::xml_node::insert_copy_before
    pugi::xml_node n10 = n8.insert_copy_before(n9, n9);

    // 11. pugi::xml_node::child
    pugi::xml_node n11 = n10.child("child11");
    if (n11.empty()) {
        n11 = n10.append_child("child11");
    }

    // 12. pugi::xml_node::first_child
    pugi::xml_node n12 = n11.first_child();

    // 13. pugi::xml_node::name
    const pugi::char_t* name13 = n12.name();
    (void)name13; // Suppress unused variable warning

    // 14. pugi::xml_node::child
    pugi::xml_node n14 = n12.child("child14");
    if (n14.empty()) {
        n14 = n12.append_child("child14");
    }

    // 15. pugi::xml_node::insert_copy_before
    pugi::xml_node n15 = n12.insert_copy_before(n14, n14);

    // 16. pugi::xml_node::child
    pugi::xml_node n16 = n15.child("child16");
    if (n16.empty()) {
        n16 = n15.append_child("child16");
    }

    // 17. pugi::xml_node::first_child
    pugi::xml_node n17 = n16.first_child();

    // 18. pugi::xml_node::child
    pugi::xml_node n18 = n17.child("child18");
    if (n18.empty()) {
        n18 = n17.append_child("child18");
    }

    // 19. pugi::xml_node::last_child
    pugi::xml_node n19 = n18.last_child();

    // 20. pugi::xml_node::name
    const pugi::char_t* name20 = n19.name();
    (void)name20;

    // 21. pugi::xml_node::child
    pugi::xml_node n21 = n19.child("child21");
    if (n21.empty()) {
        n21 = n19.append_child("child21");
    }

    // 22. pugi::xml_node::insert_copy_before
    pugi::xml_node n22 = n19.insert_copy_before(n21, n21);

    // 23. pugi::xml_node::child
    pugi::xml_node n23 = n22.child("child23");
    if (n23.empty()) {
        n23 = n22.append_child("child23");
    }

    // 24. pugi::xml_node::first_child
    pugi::xml_node n24 = n23.first_child();

    // 25. pugi::xml_node::next_sibling
    pugi::xml_node n25 = n24.next_sibling();

    // 26. pugi::xml_node::child
    pugi::xml_node n26 = n25.child("child26");
    if (n26.empty()) {
        n26 = n25.append_child("child26");
    }

    // 27. pugi::xml_node::first_child
    pugi::xml_node n27 = n26.first_child();

    // 28. pugi::xml_node::value
    const pugi::char_t* value28 = n27.value();
    (void)value28;

    return 0;
}