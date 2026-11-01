// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    pugi::xml_document doc;
    write_dummy_file(Data, Size);

    // Load document from dummy file
    if (!doc.load_file("./dummy_file")) {
        // If load fails, try parsing as string to ensure some structure exists for testing
        std::string input(reinterpret_cast<const char*>(Data), Size);
        if (!doc.load_string(input.c_str())) {
            // Create a minimal valid document if parsing fails completely
            doc.append_child("root");
        }
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root, try first child of document
        root = doc.first_child();
    }
    if (!root) {
        // If still no valid node, return early
        return 0;
    }

    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child("test");
    if (!child_node) {
        // Create a child if it doesn't exist to allow further operations
        child_node = root.append_child("test");
    }

    // 2. pugi::xml_node::text
    pugi::xml_text text_proxy = child_node.text();

    // 3. pugi::xml_text::set
    if (Size > 0) {
        text_proxy.set(reinterpret_cast<const char*>(Data), Size);
    }

    // 4. pugi::xml_node::first_child
    pugi::xml_node first = child_node.first_child();

    // 5. pugi::xml_text::set (on first child's text if exists)
    if (first) {
        pugi::xml_text first_text = first.text();
        if (Size > 0) {
            first_text.set(reinterpret_cast<const char*>(Data), Size);
        }
    }

    // 6. pugi::xml_node::first_child (again on child_node)
    pugi::xml_node first_again = child_node.first_child();

    // 7. pugi::xml_node::last_child
    pugi::xml_node last = child_node.last_child();

    // 8. pugi::xml_text::set (on last child's text if exists)
    if (last) {
        pugi::xml_text last_text = last.text();
        if (Size > 0) {
            last_text.set(reinterpret_cast<const char*>(Data), Size);
        }
    }

    // 9. pugi::xml_node::first_child (on last if exists)
    if (last) {
        pugi::xml_node last_first = last.first_child();
        // 10. pugi::xml_node::last_child (on last if exists)
        pugi::xml_node last_last = last.last_child();
        
        // 11. pugi::xml_text::set (on last_last if exists)
        if (last_last) {
            pugi::xml_text last_last_text = last_last.text();
            if (Size > 0) {
                last_last_text.set(reinterpret_cast<const char*>(Data), Size);
            }
        }
        
        // 12. pugi::xml_node::first_child (on last_last if exists)
        if (last_last) {
            pugi::xml_node last_last_first = last_last.first_child();
            (void)last_last_first; // Suppress unused warning
        }
    }

    // Cleanup is automatic via RAII for pugi::xml_document
    return 0;
}