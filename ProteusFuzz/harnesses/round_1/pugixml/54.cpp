// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

static bool safe_parse_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return false;
    }
    // Ensure null-termination for safety
    std::string buffer(reinterpret_cast<const char*>(data), size);
    if (buffer.empty()) return false;
    
    // Add a null terminator if not present
    if (buffer.back() != '\0') {
        buffer += '\0';
    }

    pugi::xml_parse_result result = doc.load_string(buffer.c_str());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    if (!safe_parse_document(doc, Data, Size)) {
        // If parsing fails, try to create a minimal valid document to test traversal logic
        doc.append_child("root");
    }

    pugi::xml_node root = doc.first_child();
    if (!root) {
        // If no root, try to create one
        root = doc.append_child("root");
    }

    // 1. pugi::xml_node::child
    // Use a name derived from input or a fixed string if input is insufficient
    // CRITICAL FIX: Store the name string in a variable with lifetime extending beyond the call
    std::string child_name_str = "target";
    if (Size > 0) {
        // Create a temporary string from input to use as name, ensuring null termination
        child_name_str.assign(reinterpret_cast<const char*>(Data), Size);
        // Replace non-printable or dangerous chars for safety in name
        for (char& c : child_name_str) {
            if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '-') {
                c = 'x';
            }
        }
        if (child_name_str.empty()) child_name_str = "target";
    }
    
    const char* child_name = child_name_str.c_str();

    pugi::xml_node child_node = root.child(child_name);
    
    // If child not found, create one to ensure subsequent calls have a valid target
    if (!child_node) {
        child_node = root.append_child(child_name);
    }

    // 2. pugi::xml_node::text
    pugi::xml_text text_obj = child_node.text();

    // 3. pugi::xml_text::set
    // Prepare data for set. Ensure it's null-terminated or use size.
    size_t set_size = 0;
    const char* set_data = "initial";
    
    // CRITICAL FIX: Store set data in a variable with lifetime extending beyond the call
    std::string set_data_str = "initial";
    if (Size > 0) {
        // Use input data, but ensure we don't exceed bounds and handle nulls
        set_data_str.assign(reinterpret_cast<const char*>(Data), Size);
        set_data = set_data_str.c_str();
        set_size = set_data_str.size();
    }

    // Call set. Note: xml_text::set might fail if node is invalid, but we ensured validity.
    // We ignore the return value for fuzzing stability, just exercising the path.
    text_obj.set(set_data, set_size);

    // 4. pugi::xml_node::first_child
    pugi::xml_node first_child = child_node.first_child();
    
    // 5. pugi::xml_text::set (on the first_child if valid, else on child_node again)
    if (first_child) {
        pugi::xml_text text_first = first_child.text();
        text_first.set(set_data, set_size);
    } else {
        // Fallback to set on the original child_node text again to exercise the path
        pugi::xml_text text_fallback = child_node.text();
        text_fallback.set(set_data, set_size);
    }

    // 6. pugi::xml_node::first_child (again)
    pugi::xml_node first_child_2 = child_node.first_child();

    // 7. pugi::xml_node::last_child
    pugi::xml_node last_child = child_node.last_child();

    // 8. pugi::xml_text::set
    if (last_child) {
        pugi::xml_text text_last = last_child.text();
        text_last.set(set_data, set_size);
    } else {
        // Fallback
        pugi::xml_text text_fallback2 = child_node.text();
        text_fallback2.set(set_data, set_size);
    }

    // 9. pugi::xml_node::first_child
    pugi::xml_node first_child_3 = child_node.first_child();

    // 10. pugi::xml_node::last_child
    pugi::xml_node last_child_2 = child_node.last_child();

    // 11. pugi::xml_text::set
    if (last_child_2) {
        pugi::xml_text text_last_2 = last_child_2.text();
        text_last_2.set(set_data, set_size);
    } else {
        pugi::xml_text text_fallback3 = child_node.text();
        text_fallback3.set(set_data, set_size);
    }

    // 12. pugi::xml_node::first_child
    pugi::xml_node first_child_4 = child_node.first_child();

    // Cleanup is handled by RAII (pugi::xml_document destructor)
    return 0;
}