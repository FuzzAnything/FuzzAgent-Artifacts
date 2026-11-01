// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: load XML from fuzzer input
    pugi::xml_document doc;
    
    // Ensure the input is null-terminated for parsing
    std::string input(reinterpret_cast<const char*>(Data), Size);
    // Append a null terminator if not present
    if (input.back() != '\0') {
        input += '\0';
    }

    // Write to dummy file as per requirement
    write_dummy_file(Data, Size);

    // Parse the XML
    if (!doc.load_string(input.c_str())) {
        // If parsing fails, try to load from file or create a minimal valid structure
        if (!doc.load_file("./dummy_file")) {
            // Create a minimal valid XML document to ensure we have a root
            doc.append_child("root");
        }
    }

    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        // If no root found, try to get any child or create one
        root = doc.first_child();
        if (root.empty()) {
            root = doc.append_child("root");
        }
    }

    // 1. pugi::xml_node::child
    // We use a fixed name "test" or try to derive from input if possible, 
    // but for safety and simplicity in fuzzing, we use a static name or empty string.
    // The hint suggests using string_view_t, but the signature takes char_t*.
    // We'll use a simple C-string.
    pugi::xml_node child_node = root.child("test");
    
    // If child_node is empty, we might need to create it to proceed with text operations
    if (child_node.empty()) {
        child_node = root.append_child("test");
    }

    // 2. pugi::xml_node::text
    pugi::xml_text text_proxy = child_node.text();

    // 3. pugi::xml_text::set
    // We use the fuzzer input as the text content. 
    // Ensure it's null-terminated for the char_t* parameter if size is large.
    // The set function takes (const char_t* rhs, size_t size).
    // We cast Data to char_t*. Note: pugixml char_t is usually char or wchar_t.
    // Assuming default char mode.
    text_proxy.set(reinterpret_cast<const pugi::char_t*>(Data), Size);

    // 4. pugi::xml_node::first_child
    pugi::xml_node first_child = child_node.first_child();

    // 5. pugi::xml_text::set
    // If first_child is empty, text() returns empty proxy, set returns false safely.
    pugi::xml_text text_proxy_2 = first_child.text();
    text_proxy_2.set(reinterpret_cast<const pugi::char_t*>(Data), Size);

    // 6. pugi::xml_node::first_child
    pugi::xml_node first_child_2 = first_child.first_child();

    // 7. pugi::xml_node::last_child
    pugi::xml_node last_child = first_child.last_child();

    // 8. pugi::xml_text::set
    pugi::xml_text text_proxy_3 = last_child.text();
    text_proxy_3.set(reinterpret_cast<const pugi::char_t*>(Data), Size);

    // 9. pugi::xml_node::first_child
    pugi::xml_node first_child_3 = last_child.first_child();

    // 10. pugi::xml_node::last_child
    pugi::xml_node last_child_2 = last_child.last_child();

    // 11. pugi::xml_text::set
    pugi::xml_text text_proxy_4 = last_child_2.text();
    text_proxy_4.set(reinterpret_cast<const pugi::char_t*>(Data), Size);

    // 12. pugi::xml_node::first_child
    pugi::xml_node first_child_4 = last_child_2.first_child();

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}