// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
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
#include <cstring>
#include <string_view>

static pugi::xml_document* create_document(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Try to parse the input data as XML
    // If parsing fails, create a minimal valid XML structure
    if (size > 0 && data != nullptr) {
        std::string xml_str(reinterpret_cast<const char*>(data), size);
        // Ensure the string is null-terminated for safety
        if (xml_str.back() != '\0') {
            xml_str += '\0';
        }
        if (doc->load_string(xml_str.c_str()) != pugi::status_ok) {
            // If parsing fails, load a default minimal XML
            if (doc->load_string("<root></root>") != pugi::status_ok) {
                delete doc;
                return nullptr;
            }
        }
    } else {
        if (doc->load_string("<root></root>") != pugi::status_ok) {
            delete doc;
            return nullptr;
        }
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    pugi::xml_document* doc = create_document(Data, Size);
    if (!doc) return 0;

    try {
        pugi::xml_node root = doc->document_element();
        if (!root) {
            delete doc;
            return 0;
        }

        // Create a new node to use as a target for insertion
        pugi::xml_node new_node = root.append_child("new_node");
        if (!new_node) {
            delete doc;
            return 0;
        }

        // 1. pugi::xml_node::insert_child_before (const char_t* name, const xml_node& node)
        pugi::xml_node inserted1 = root.insert_child_before("test1", new_node);

        // 2. pugi::xml_node::child (const char_t* name)
        pugi::xml_node child1 = root.child("test1");

        // 3. pugi::xml_node::child (const char_t* name)
        pugi::xml_node child2 = root.child("new_node");

        // 4. pugi::xml_node::child (const char_t* name)
        pugi::xml_node child3 = root.child("test1");

        // 5. pugi::xml_node::insert_child_before (string_view_t name, const xml_node& node)
        // Note: pugixml uses std::string_view for string_view_t in C++17+, or a custom type.
        // We use a const char* cast to ensure compatibility if the specific overload isn't found.
        // However, the prompt specifically asks for string_view_t usage.
        // Assuming C++17 or compatible string_view support in pugixml version.
        std::string_view sv_name = "test2";
        pugi::xml_node inserted2 = root.insert_child_before(sv_name, child1);

        // 6. pugi::xml_node::insert_child_before (const char_t* name, const xml_node& node)
        pugi::xml_node inserted3 = root.insert_child_before("test3", child2);

        // 7. pugi::xml_node::insert_child_before (string_view_t name, const xml_node& node)
        std::string_view sv_name2 = "test4";
        pugi::xml_node inserted4 = root.insert_child_before(sv_name2, child3);

        // 8. pugi::xml_node::insert_child_before (const char_t* name, const xml_node& node)
        pugi::xml_node inserted5 = root.insert_child_before("test5", inserted1);

        // 9. pugi::xml_node::insert_child_before (string_view_t name, const xml_node& node)
        std::string_view sv_name3 = "test6";
        pugi::xml_node inserted6 = root.insert_child_before(sv_name3, inserted2);

        // 10. pugi::xml_node::insert_child_before (const char_t* name, const xml_node& node)
        pugi::xml_node inserted7 = root.insert_child_before("test7", inserted3);

        // Cleanup
        delete doc;
    } catch (...) {
        delete doc;
        return 0;
    }

    return 0;
}