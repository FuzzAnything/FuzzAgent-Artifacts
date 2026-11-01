// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
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
#include <string>

static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (size == 0) {
        return doc;
    }
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    if (!doc->load_string(xml_str.c_str())) {
        // If parsing fails, return the empty document to allow fuzzing of empty states
        return doc;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->first_child();
    if (root.empty()) {
        // If no root, create a dummy root to allow testing of prepend_copy on non-empty structures
        root = doc->append_child("root");
    }

    // Ensure we have a node to copy from for prepend_copy
    pugi::xml_node copy_source = root;
    if (copy_source.empty()) {
        copy_source = root.append_child("dummy");
    }

    // 1. pugi::xml_node::prepend_copy
    pugi::xml_node node1 = root.prepend_copy(copy_source);

    // 2. pugi::xml_node::child
    pugi::xml_node node2 = node1.child("dummy");
    if (node2.empty()) {
        node2 = node1.child("root");
    }
    if (node2.empty()) {
        node2 = node1;
    }

    // 3. pugi::xml_node::prepend_copy
    pugi::xml_node node3 = node2.prepend_copy(copy_source);

    // 4. pugi::xml_node::prepend_copy
    pugi::xml_node node4 = node3.prepend_copy(copy_source);

    // 5. pugi::xml_node::child
    pugi::xml_node node5 = node4.child("dummy");
    if (node5.empty()) {
        node5 = node4.child("root");
    }
    if (node5.empty()) {
        node5 = node4;
    }

    // 6. pugi::xml_node::prepend_copy
    pugi::xml_node node6 = node5.prepend_copy(copy_source);

    // 7. pugi::xml_node::child
    pugi::xml_node node7 = node6.child("dummy");
    if (node7.empty()) {
        node7 = node6.child("root");
    }
    if (node7.empty()) {
        node7 = node6;
    }

    // 8. pugi::xml_node::first_child
    pugi::xml_node node8 = node7.first_child();
    if (node8.empty()) {
        node8 = node7;
    }

    // 9. pugi::xml_node::value
    const pugi::char_t* val9 = node8.value();
    (void)val9; // Suppress unused variable warning

    // 10. pugi::xml_node::child
    pugi::xml_node node10 = node8.child("dummy");
    if (node10.empty()) {
        node10 = node8.child("root");
    }
    if (node10.empty()) {
        node10 = node8;
    }

    // 11. pugi::xml_node::prepend_copy
    pugi::xml_node node11 = node10.prepend_copy(copy_source);

    // 12. pugi::xml_node::child
    pugi::xml_node node12 = node11.child("dummy");
    if (node12.empty()) {
        node12 = node11.child("root");
    }
    if (node12.empty()) {
        node12 = node11;
    }

    // 13. pugi::xml_node::child
    pugi::xml_node node13 = node12.child("dummy");
    if (node13.empty()) {
        node13 = node12.child("root");
    }
    if (node13.empty()) {
        node13 = node12;
    }

    // 14. pugi::xml_node::name
    const pugi::char_t* name14 = node13.name();
    (void)name14;

    // 15. pugi::xml_node::child
    pugi::xml_node node15 = node13.child("dummy");
    if (node15.empty()) {
        node15 = node13.child("root");
    }
    if (node15.empty()) {
        node15 = node13;
    }

    // 16. pugi::xml_node::child
    pugi::xml_node node16 = node15.child("dummy");
    if (node16.empty()) {
        node16 = node15.child("root");
    }
    if (node16.empty()) {
        node16 = node15;
    }

    // 17. pugi::xml_node::prepend_copy
    pugi::xml_node node17 = node16.prepend_copy(copy_source);

    // 18. pugi::xml_node::child
    pugi::xml_node node18 = node17.child("dummy");
    if (node18.empty()) {
        node18 = node17.child("root");
    }
    if (node18.empty()) {
        node18 = node17;
    }

    // 19. pugi::xml_node::first_child
    pugi::xml_node node19 = node18.first_child();
    if (node19.empty()) {
        node19 = node18;
    }

    // 20. pugi::xml_node::next_sibling
    pugi::xml_node node20 = node19.next_sibling();
    if (node20.empty()) {
        node20 = node19;
    }

    // 21. pugi::xml_node::value
    const pugi::char_t* val21 = node20.value();
    (void)val21;

    delete doc;
    return 0;
}