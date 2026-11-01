// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
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

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.append_child(pugi::node_element).set_name("root");
        return doc;
    }
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    if (!doc.load_string(xml_str.c_str())) {
        doc.append_child(pugi::node_element).set_name("root");
        return doc;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc = create_document_from_input(Data, Size);
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.first_child();
        if (!root) {
            pugi::xml_node new_root = doc.append_child(pugi::node_element);
            new_root.set_name("root");
            root = new_root;
        }
    }

    // Prepare child nodes for removal and access
    root.append_child(pugi::node_element).set_name("target");
    root.append_child(pugi::node_element).set_name("target");
    root.append_child(pugi::node_element).set_name("other");

    // 1. pugi::xml_node::remove_child (overload: const char_t* name)
    root.remove_child("target");

    // 2. pugi::xml_node::remove_child (overload: const char_t* name)
    root.remove_child("target");

    // 3. pugi::xml_node::child
    pugi::xml_node child1 = root.child("other");

    // 4. pugi::xml_node::child
    pugi::xml_node child2 = root.child("target");

    // 5. pugi::xml_node::remove_child (overload: const char_t* name)
    root.remove_child("other");

    // 6. pugi::xml_node::remove_child (overload: const char_t* name)
    root.remove_child("target");

    // 7. pugi::xml_node::child
    pugi::xml_node child3 = root.child("nonexistent");

    // 8. pugi::xml_node::remove_child (overload: const xml_node& n)
    pugi::xml_node dummy_node;
    root.remove_child(dummy_node);

    // 9. pugi::xml_node::child
    pugi::xml_node child4 = root.child("root");

    // 10. pugi::xml_node::remove_child (overload: string_view_t name)
    std::string_view sv_name("target");
    root.remove_child(sv_name);

    // 11. pugi::xml_node::remove_child (overload: const char_t* name)
    root.remove_child("target");

    // 12. pugi::xml_node::remove_child (overload: const char_t* name)
    root.remove_child("other");

    // 13. pugi::xml_node::remove_child (overload: const char_t* name)
    root.remove_child("dummy");

    // 14. pugi::xml_node::remove_child (overload: const char_t* name)
    root.remove_child("dummy");

    // 15. pugi::xml_node::remove_child (overload: const char_t* name)
    root.remove_child("dummy");

    return 0;
}