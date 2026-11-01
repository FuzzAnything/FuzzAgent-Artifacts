// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
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
#include <string_view>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Create a root element to start building the tree
    pugi::xml_node root = doc->append_child("root");
    if (!root) {
        delete doc;
        return nullptr;
    }

    // Try to parse input as XML if it looks like valid XML, otherwise ignore
    if (size > 0) {
        // Simple check: if input starts with '<', try parsing
        if (data[0] == '<') {
            std::string xml_str(reinterpret_cast<const char*>(data), size);
            if (doc->load_string(xml_str.c_str()) == pugi::status_ok) {
                return doc;
            }
        }
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    try {
        pugi::xml_node root = doc->child("root");
        if (!root) {
            // If no root exists, create one
            root = doc->append_child("root");
            if (!root) {
                delete doc;
                return 0;
            }
        }

        // Step 1: pugi::xml_node::child
        std::string_view child_name_view = "child1";
        pugi::xml_node child_node = root.child(child_name_view.data());

        // Step 2: pugi::xml_node::append_child
        std::string_view append_name1_view = "node1";
        pugi::xml_node node1 = root.append_child(append_name1_view.data());

        // Step 3: pugi::xml_node::append_child
        std::string_view append_name2_view = "node2";
        pugi::xml_node node2 = root.append_child(append_name2_view.data());

        // Step 4: pugi::xml_text::set (unsigned long long)
        if (node1) {
            pugi::xml_text text1 = node1.text();
            unsigned long long val1 = 0;
            if (Size >= sizeof(unsigned long long)) {
                val1 = *reinterpret_cast<const unsigned long long*>(Data);
            }
            text1.set(val1);
        }

        // Step 5: pugi::xml_node::append_child
        std::string_view append_name3_view = "node3";
        pugi::xml_node node3 = root.append_child(append_name3_view.data());

        // Step 6: pugi::xml_node::append_child
        std::string_view append_name4_view = "node4";
        pugi::xml_node node4 = root.append_child(append_name4_view.data());

        // Step 7: pugi::xml_text::set (long long)
        if (node2) {
            pugi::xml_text text2 = node2.text();
            long long val2 = 0;
            // Safe reading: ensure we have enough data before reading
            if (Size >= sizeof(long long)) {
                // Read from a safe offset (start of buffer if size is large enough)
                val2 = *reinterpret_cast<const long long*>(Data);
            }
            text2.set(val2);
        }

        // Additional exploration: try setting text on node3 and node4 if they exist
        if (node3) {
            pugi::xml_text text3 = node3.text();
            unsigned long long val3 = 1234567890ULL;
            text3.set(val3);
        }
        
        if (node4) {
            pugi::xml_text text4 = node4.text();
            long long val4 = -987654321LL;
            text4.set(val4);
        }

        // Try to access child_node if it exists
        if (child_node) {
            pugi::xml_text child_text = child_node.text();
            // Use Size directly as a safe value
            child_text.set(static_cast<unsigned long long>(Size));
        }

    } catch (...) {
        // Handle any unexpected exceptions
    }

    delete doc;
    return 0;
}