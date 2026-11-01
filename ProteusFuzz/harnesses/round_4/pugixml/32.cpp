// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstddef>
#include <string>
#include <cstring>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Create a string from the input data, ensuring null termination for safety
    std::string input(reinterpret_cast<const char*>(data), size);
    // Append a null terminator if not present to avoid buffer issues, though pugixml handles sizes
    if (!input.empty() && input.back() != '\0') {
        input += '\0';
    }

    pugi::xml_parse_result result = doc->load_string(input.c_str());
    if (!result) {
        // If parsing fails, we still return the document to test error handling paths
        // or we could try to load an empty document. Let's keep the empty document.
        doc->reset();
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->document_element();
    if (!root) {
        // If no root, try to create one or use document node itself
        root = *doc;
    }

    // Prepare string_view for overloads
    std::string_view dummy_view = "";
    const pugi::char_t* dummy_cstr = nullptr;

    // Sequence of calls as requested
    // 1. pugi::xml_node::remove_child
    root.remove_child(dummy_cstr);

    // 2. pugi::xml_node::remove_child
    root.remove_child(dummy_cstr);

    // 3. pugi::xml_node::child
    pugi::xml_node node1 = root.child(dummy_cstr);

    // 4. pugi::xml_node::child
    pugi::xml_node node2 = root.child(dummy_cstr);

    // 5. pugi::xml_node::remove_child
    root.remove_child(dummy_cstr);

    // 6. pugi::xml_node::remove_child
    root.remove_child(dummy_cstr);

    // 7. pugi::xml_node::child
    pugi::xml_node node3 = root.child(dummy_cstr);

    // 8. pugi::xml_node::remove_child (overload: xml_node&)
    // We pass node1 which might be empty, which is safe
    root.remove_child(node1);

    // 9. pugi::xml_node::child
    pugi::xml_node node4 = root.child(dummy_cstr);

    // 10. pugi::xml_node::remove_child (overload: string_view_t)
    root.remove_child(dummy_view);

    // 11. pugi::xml_node::remove_child
    root.remove_child(dummy_cstr);

    // 12. pugi::xml_node::remove_child
    root.remove_child(dummy_cstr);

    // 13. pugi::xml_node::remove_child
    root.remove_child(dummy_cstr);

    // 14. pugi::xml_node::remove_child
    root.remove_child(dummy_cstr);

    // 15. pugi::xml_node::remove_child
    root.remove_child(dummy_cstr);

    delete doc;
    return 0;
}