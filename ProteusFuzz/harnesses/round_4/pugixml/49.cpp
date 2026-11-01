// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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
#include <string>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    if (size == 0) {
        return nullptr;
    }
    pugi::xml_document* doc = new pugi::xml_document();
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    if (!result) {
        delete doc;
        return nullptr;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->document_element();
    if (!root) {
        root = *doc;
    }

    // Create a prototype node if needed for prepend_copy
    pugi::xml_node proto = root;
    if (!proto) {
        proto = doc->append_child("prototype");
        // Fix: Use text() to get the text node, then set_value on it
        pugi::xml_node text_node = proto.append_child(pugi::node_pcdata);
        text_node.set_value("test");
    }

    // 1. pugi::xml_node::prepend_copy
    pugi::xml_node n1 = root.prepend_copy(proto);

    // 2. pugi::xml_node::child
    pugi::xml_node n2 = root.child("prototype");

    // 3. pugi::xml_node::prepend_copy
    pugi::xml_node n3 = n2.prepend_copy(proto);

    // 4. pugi::xml_node::prepend_copy
    pugi::xml_node n4 = n3.prepend_copy(proto);

    // 5. pugi::xml_node::child
    pugi::xml_node n5 = n4.child("prototype");

    // 6. pugi::xml_node::prepend_copy
    pugi::xml_node n6 = n5.prepend_copy(proto);

    // 7. pugi::xml_node::child
    pugi::xml_node n7 = n6.child("prototype");

    // 8. pugi::xml_node::first_child
    pugi::xml_node n8 = n7.first_child();

    // 9. pugi::xml_node::value
    const pugi::char_t* v9 = n8.value();
    (void)v9;

    // 10. pugi::xml_node::child
    pugi::xml_node n10 = n8.child("prototype");

    // 11. pugi::xml_node::prepend_copy
    pugi::xml_node n11 = n10.prepend_copy(proto);

    // 12. pugi::xml_node::child
    pugi::xml_node n12 = n11.child("prototype");

    // 13. pugi::xml_node::child
    pugi::xml_node n13 = n12.child("prototype");

    // 14. pugi::xml_node::name
    const pugi::char_t* v14 = n13.name();
    (void)v14;

    // 15. pugi::xml_node::child
    pugi::xml_node n15 = n13.child("prototype");

    // 16. pugi::xml_node::child
    pugi::xml_node n16 = n15.child("prototype");

    // 17. pugi::xml_node::prepend_copy
    pugi::xml_node n17 = n16.prepend_copy(proto);

    // 18. pugi::xml_node::child
    pugi::xml_node n18 = n17.child("prototype");

    // 19. pugi::xml_node::first_child
    pugi::xml_node n19 = n18.first_child();

    // 20. pugi::xml_node::next_sibling
    pugi::xml_node n20 = n19.next_sibling();

    // 21. pugi::xml_node::value
    const pugi::char_t* v21 = n20.value();
    (void)v21;

    delete doc;
    return 0;
}