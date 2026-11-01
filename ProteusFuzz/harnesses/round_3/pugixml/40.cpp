// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstddef>
#include <string>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;

    // Parse input as XML to create a valid document structure
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to create a minimal document manually
        if (!doc.append_child("root")) return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root exists, create one
        root = doc.append_child("root");
        if (!root) return 0;
    }

    // Step 1: pugi::xml_node::prepend_child (overload 1: const char_t*)
    pugi::xml_node node1 = root.prepend_child("child1");
    if (node1) {
        // Step 2: pugi::xml_node::child
        pugi::xml_node found1 = root.child("child1");
        if (found1) {
            // Step 3: pugi::xml_node::prepend_child (overload 2: string_view_t)
            std::string_view sv_name = "child2";
            pugi::xml_node node2 = found1.prepend_child(sv_name);
            if (node2) {
                // Step 4: pugi::xml_node::child
                pugi::xml_node found2 = found1.child("child2");
                if (found2) {
                    // Step 5: pugi::xml_node::child
                    pugi::xml_node found3 = found2.child("child3");
                    if (found3) {
                        // Step 6: pugi::xml_node::prepend_child (overload 1)
                        pugi::xml_node node3 = found3.prepend_child("child4");
                        if (node3) {
                            // Step 7: pugi::xml_node::child
                            pugi::xml_node found4 = found3.child("child4");
                            if (found4) {
                                // Step 8: pugi::xml_node::prepend_child (overload 1)
                                pugi::xml_node node4 = found4.prepend_child("child5");
                                if (node4) {
                                    // Step 9: pugi::xml_node::prepend_child (overload 2)
                                    std::string_view sv_name2 = "child6";
                                    pugi::xml_node node5 = node4.prepend_child(sv_name2);
                                    (void)node5; // Suppress unused variable warning
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Cleanup is automatic via RAII (xml_document destructor)
    return 0;
}