// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include <vector>
#include <sstream>

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Convert fuzz input to a string for parsing
    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    // Create an XML document
    pugi::xml_document doc;
    
    // Parse the input as XML; if parsing fails, return early
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    if (!result) {
        // Parsing failed, but we can still test with an empty document
        // or try to recover. Here we proceed with the parsed (possibly empty) doc.
    }

    // Get the root node
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Helper to get attribute name from input
    std::vector<std::string> attr_names;
    std::vector<std::string> child_names;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        if (token.size() > 0 && token.size() < 50) {
            attr_names.push_back(token);
            child_names.push_back(token);
        }
    }
    if (attr_names.empty()) {
        attr_names.push_back("id");
        attr_names.push_back("class");
        attr_names.push_back("name");
    }
    if (child_names.empty()) {
        child_names.push_back("child");
        child_names.push_back("node");
    }

    // Ensure we have enough names for the sequence
    while (attr_names.size() < 15) {
        attr_names.push_back("attr");
    }
    while (child_names.size() < 5) {
        child_names.push_back("child");
    }

    // Sequence of calls as specified:
    // 1. remove_attribute (const char_t*)
    if (!root.empty()) {
        root.remove_attribute(attr_names[0].c_str());
    }
    // 2. remove_attribute (const char_t*)
    if (!root.empty()) {
        root.remove_attribute(attr_names[1].c_str());
    }
    // 3. child (const char_t*)
    pugi::xml_node child1 = root.child(child_names[0].c_str());
    // 4. child (const char_t*)
    pugi::xml_node child2 = root.child(child_names[1].c_str());
    // 5. remove_attribute (const char_t*)
    if (!root.empty()) {
        root.remove_attribute(attr_names[2].c_str());
    }
    // 6. remove_attribute (const char_t*)
    if (!root.empty()) {
        root.remove_attribute(attr_names[3].c_str());
    }
    // 7. attribute (const char_t*)
    pugi::xml_attribute attr1 = root.attribute(attr_names[4].c_str());
    // 8. remove_attribute (const char_t*)
    if (!root.empty()) {
        root.remove_attribute(attr_names[5].c_str());
    }
    // 9. attribute (const char_t*)
    pugi::xml_attribute attr2 = root.attribute(attr_names[6].c_str());
    // 10. remove_attribute (const char_t*)
    if (!root.empty()) {
        root.remove_attribute(attr_names[7].c_str());
    }
    // 11. remove_attribute (const char_t*)
    if (!root.empty()) {
        root.remove_attribute(attr_names[8].c_str());
    }
    // 12. remove_attribute (const char_t*)
    if (!root.empty()) {
        root.remove_attribute(attr_names[9].c_str());
    }
    // 13. remove_attribute (const char_t*)
    if (!root.empty()) {
        root.remove_attribute(attr_names[10].c_str());
    }

    // Now test the other overloads of remove_attribute
    // Overload: remove_attribute(const xml_attribute& a)
    // Try to remove an attribute by reference
    pugi::xml_attribute attr_ref = root.attribute(attr_names[11].c_str());
    if (attr_ref) {
        root.remove_attribute(attr_ref);
    }
    // Overload: remove_attribute(string_view_t name)
    // Use std::string_view for string_view_t parameter
    std::string_view sv_name(attr_names[12]);
    if (!root.empty()) {
        root.remove_attribute(sv_name);
    }

    // Additional exploration: try on child nodes if they exist
    if (child1) {
        child1.remove_attribute(attr_names[13].c_str());
        child1.remove_attribute(attr_names[14].c_str());
        pugi::xml_attribute c_attr = child1.attribute(attr_names[0].c_str());
        if (c_attr) {
            child1.remove_attribute(c_attr);
        }
        std::string_view sv_c(attr_names[1]);
        child1.remove_attribute(sv_c);
    }

    if (child2) {
        child2.remove_attribute(attr_names[2].c_str());
        child2.remove_attribute(attr_names[3].c_str());
        pugi::xml_attribute c_attr2 = child2.attribute(attr_names[4].c_str());
        if (c_attr2) {
            child2.remove_attribute(c_attr2);
        }
        std::string_view sv_c2(attr_names[5]);
        child2.remove_attribute(sv_c2);
    }

    return 0;
}