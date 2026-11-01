// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
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
#include <string_view>
#include <cstring>
#include <cstdlib>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

// Helper to create a safe null-terminated string from fuzz input
static std::string make_safe_string(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    // Find the first null byte to ensure we don't read past buffer if used as C-string
    // Or just take the whole buffer and ensure null termination by copying to a new buffer
    // Since pugixml expects null-terminated strings for char_t*, we must ensure safety.
    // We will copy up to 'size' bytes into a new string, which adds a null terminator.
    // However, if the input contains nulls, strlen will stop early, which is fine for C-strings.
    // The crash happened because we passed a pointer into the fuzz buffer which might not be null-terminated 
    // at the exact boundary or pugixml reads past it.
    // Safest approach: copy to a std::string which guarantees null termination.
    return std::string(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Parse the input as XML to create a valid document structure
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, create a minimal valid document
        doc.append_child("root");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Create safe string versions of the input to use as attribute names
    std::string safe_str = make_safe_string(Data, Size);
    const char* c_str = safe_str.c_str();
    std::string_view a_str = safe_str;

    // 1. pugi::xml_node::prepend_attribute (overload 1: string_view_t)
    pugi::xml_attribute attr1 = root.prepend_attribute(a_str);

    // 2. pugi::xml_node::prepend_attribute (overload 2: const char_t*)
    // Using the safe C-string from std::string
    pugi::xml_attribute attr2 = root.prepend_attribute(c_str);

    // 3. pugi::xml_node::child
    pugi::xml_node child1 = root.child(c_str);

    // 4. pugi::xml_node::prepend_attribute (overload 1: string_view_t)
    pugi::xml_attribute attr3 = root.prepend_attribute(a_str);

    // 5. pugi::xml_node::child
    pugi::xml_node child2 = root.child(c_str);

    // 6. pugi::xml_node::prepend_attribute (overload 2: const char_t*)
    pugi::xml_attribute attr4 = root.prepend_attribute(c_str);

    // 7. pugi::xml_node::child
    pugi::xml_node child3 = root.child(c_str);

    // 8. pugi::xml_node::child
    pugi::xml_node child4 = root.child(c_str);

    // 9. pugi::xml_node::prepend_attribute (overload 1: string_view_t)
    pugi::xml_attribute attr5 = root.prepend_attribute(a_str);

    // 10. pugi::xml_node::child
    pugi::xml_node child5 = root.child(c_str);

    // 11. pugi::xml_node::child
    pugi::xml_node child6 = root.child(c_str);

    // 12. pugi::xml_node::prepend_attribute (overload 2: const char_t*)
    pugi::xml_attribute attr6 = root.prepend_attribute(c_str);

    // Cleanup is handled by doc destructor
    return 0;
}