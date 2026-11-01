// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <algorithm>

static std::string create_xml_from_input(const uint8_t* data, size_t size) {
    std::string result = "<root>";
    // Limit the amount of data used to prevent excessive memory usage
    size_t max_len = size > 10000 ? 10000 : size;
    
    // Append input data as text content, escaping basic XML characters to ensure validity
    for (size_t i = 0; i < max_len; ++i) {
        char c = static_cast<char>(data[i]);
        if (c == '<') {
            result += "&lt;";
        } else if (c == '>') {
            result += "&gt;";
        } else if (c == '&') {
            result += "&amp;";
        } else if (c == '"') {
            result += "&quot;";
        } else if (c == '\'') {
            result += "&apos;";
        } else if (c >= 32 && c < 127) {
            result += c;
        } else {
            // Replace non-printable characters with a placeholder
            result += " ";
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string xml_str = create_xml_from_input(Data, Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    if (!result) {
        // If parsing fails, try a minimal valid XML to ensure we can test the API chain
        // This ensures the fuzzer doesn't just exit on malformed input
        doc.reset();
        doc.append_child("root");
    }

    // Start traversal from root
    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root child found, try to create one or use document root
        root = doc.first_child();
        if (!root) {
            root = doc.append_child("root");
        }
    }

    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child("child");
    if (!child_node) {
        // Create a child if it doesn't exist to allow further operations
        child_node = root.append_child("child");
    }

    // 2. pugi::xml_node::text
    pugi::xml_text text_proxy = child_node.text();

    // 3. pugi::xml_text::set
    // Use a portion of the input data as the text content
    size_t text_len = (Size < 100) ? Size : 100;
    std::string text_content(reinterpret_cast<const char*>(Data), text_len);
    text_proxy.set(text_content.c_str(), text_content.size());

    // 4. pugi::xml_node::first_child
    pugi::xml_node first_child = child_node.first_child();

    // 5. pugi::xml_text::set (on the first child if it exists, otherwise on child_node again)
    if (first_child) {
        first_child.text().set(text_content.c_str(), text_content.size());
    } else {
        // Fallback to child_node if first_child is empty
        child_node.text().set(text_content.c_str(), text_content.size());
    }

    // 6. pugi::xml_node::first_child (again)
    first_child = child_node.first_child();

    // 7. pugi::xml_node::last_child
    pugi::xml_node last_child = child_node.last_child();

    // 8. pugi::xml_text::set (on last_child if it exists)
    if (last_child) {
        last_child.text().set(text_content.c_str(), text_content.size());
    } else {
        // Fallback
        child_node.text().set(text_content.c_str(), text_content.size());
    }

    // 9. pugi::xml_node::first_child (again)
    first_child = child_node.first_child();

    // 10. pugi::xml_node::last_child (again)
    last_child = child_node.last_child();

    // 11. pugi::xml_text::set (on first_child if it exists)
    if (first_child) {
        first_child.text().set(text_content.c_str(), text_content.size());
    } else {
        child_node.text().set(text_content.c_str(), text_content.size());
    }

    // 12. pugi::xml_node::first_child (final)
    first_child = child_node.first_child();
    
    // No specific cleanup needed for pugixml as it manages its own memory
    // The document is destroyed when it goes out of scope
    
    return 0;
}