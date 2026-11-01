// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
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

static void fuzzer_initialize() {
    // No specific global initialization required for pugixml in this context
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzzer_initialize();

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    if (!root) {
        return 0;
    }

    // Step 1: pugi::xml_node::child
    // We create a child first to ensure child() can find something, 
    // though the fuzzing logic will also test the case where it returns empty.
    pugi::xml_node existing_child = root.append_child("target");
    pugi::xml_node found_node = root.child("target");

    // Step 2: pugi::xml_node::append_child (1st call)
    // Use a name derived from input if possible, or a safe default
    std::string_view raw_name(reinterpret_cast<const char*>(Data), Size);
    // Ensure null-termination for safety if we were to pass it directly, 
    // but append_child takes const char_t*. We'll use a simple safe name or 
    // a sanitized version. For fuzzing, let's just use a constant to ensure 
    // the API call happens, as the name itself is less critical than the call.
    pugi::xml_node node1 = found_node ? found_node.append_child("child1") : root.append_child("child1");

    // Step 3: pugi::xml_node::append_child (2nd call)
    pugi::xml_node node2 = node1 ? node1.append_child("child2") : root.append_child("child2");

    // Step 4: pugi::xml_text::set (unsigned long long)
    if (node2) {
        pugi::xml_text text_node = node2.text();
        // Set a value derived from input to vary state
        unsigned long long val1 = 0;
        if (Size >= sizeof(unsigned long long)) {
            val1 = *reinterpret_cast<const unsigned long long*>(Data);
        }
        text_node.set(val1);
    }

    // Step 5: pugi::xml_node::append_child (3rd call)
    pugi::xml_node node3 = node2 ? node2.append_child("child3") : root.append_child("child3");

    // Step 6: pugi::xml_node::append_child (4th call)
    pugi::xml_node node4 = node3 ? node3.append_child("child4") : root.append_child("child4");

    // Step 7: pugi::xml_text::set (long long)
    if (node4) {
        pugi::xml_text text_node2 = node4.text();
        long long val2 = 0;
        if (Size >= sizeof(long long)) {
            val2 = *reinterpret_cast<const long long*>(Data);
        }
        text_node2.set(val2);
    }

    // Cleanup is handled by pugi::xml_document destructor automatically
    return 0;
}