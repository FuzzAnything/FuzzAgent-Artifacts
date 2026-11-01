// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
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
#include <string_view>

static pugi::xml_document* create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    if (size == 0) {
        return nullptr;
    }
    pugi::xml_document* doc = new pugi::xml_document();
    // Attempt to parse the fuzz data as XML
    // We use memory parsing to avoid file I/O issues
    // Ensure null termination for safety if treating as string
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    if (doc->load_string(xml_str.c_str()) == pugi::status_ok) {
        return doc;
    }
    // If parsing fails, try to create a minimal valid document to allow traversal testing
    // This ensures the fuzzer can still exercise the API even with invalid XML input
    delete doc;
    doc = new pugi::xml_document();
    doc->append_child("root");
    doc->child("root").append_child("child");
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    pugi::xml_document* doc = create_document_from_fuzz_data(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->child("root");
    if (!root) {
        // Fallback if root doesn't exist in generated fallback doc
        root = doc->first_child();
    }

    // Ensure we have a valid node to work with
    if (!root) {
        delete doc;
        return 0;
    }

    // 1. pugi::xml_node::prepend_copy
    // We need a prototype node. Use root's first child or root itself if no children.
    pugi::xml_node proto = root.first_child();
    if (!proto) {
        proto = root; // Fallback to root
    }
    pugi::xml_node n1 = root.prepend_copy(proto);

    // 2. pugi::xml_node::child
    pugi::xml_node n2 = n1.child("child");
    if (!n2) n2 = n1.child("root"); // Try alternative name if first fails

    // 3. pugi::xml_node::prepend_copy
    pugi::xml_node n3 = n2.prepend_copy(proto);

    // 4. pugi::xml_node::prepend_copy
    pugi::xml_node n4 = n3.prepend_copy(proto);

    // 5. pugi::xml_node::child
    pugi::xml_node n5 = n4.child("child");
    if (!n5) n5 = n4.child("root");

    // 6. pugi::xml_node::prepend_copy
    pugi::xml_node n6 = n5.prepend_copy(proto);

    // 7. pugi::xml_node::child
    pugi::xml_node n7 = n6.child("child");
    if (!n7) n7 = n6.child("root");

    // 8. pugi::xml_node::first_child
    pugi::xml_node n8 = n7.first_child();

    // 9. pugi::xml_node::value
    const pugi::char_t* val9 = n8.value();
    (void)val9; // Suppress unused variable warning

    // 10. pugi::xml_node::child
    pugi::xml_node n10 = n8.child("child");
    if (!n10) n10 = n8.child("root");

    // 11. pugi::xml_node::prepend_copy
    pugi::xml_node n11 = n10.prepend_copy(proto);

    // 12. pugi::xml_node::child
    pugi::xml_node n12 = n11.child("child");
    if (!n12) n12 = n11.child("root");

    // 13. pugi::xml_node::child
    pugi::xml_node n13 = n12.child("child");
    if (!n13) n13 = n12.child("root");

    // 14. pugi::xml_node::name
    const pugi::char_t* name14 = n13.name();
    (void)name14;

    // 15. pugi::xml_node::child
    pugi::xml_node n15 = n13.child("child");
    if (!n15) n15 = n13.child("root");

    // 16. pugi::xml_node::child
    pugi::xml_node n16 = n15.child("child");
    if (!n16) n16 = n15.child("root");

    // 17. pugi::xml_node::prepend_copy
    pugi::xml_node n17 = n16.prepend_copy(proto);

    // 18. pugi::xml_node::child
    pugi::xml_node n18 = n17.child("child");
    if (!n18) n18 = n17.child("root");

    // 19. pugi::xml_node::first_child
    pugi::xml_node n19 = n18.first_child();

    // 20. pugi::xml_node::next_sibling
    pugi::xml_node n20 = n19.next_sibling();

    // 21. pugi::xml_node::value
    const pugi::char_t* val21 = n20.value();
    (void)val21;

    delete doc;
    return 0;
}