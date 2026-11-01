// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <cstring>
#include <string>

static bool ensure_null_terminated(const uint8_t* data, size_t size, std::string& out) {
    if (size == 0) {
        out = "";
        return true;
    }
    size_t null_pos = size;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == 0) {
            null_pos = i;
            break;
        }
    }
    out.assign(reinterpret_cast<const char*>(data), null_pos);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    if (!doc.load_string("<root/>")) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::append_child (overload: xml_node_type)
    pugi::xml_node node1 = root.append_child(pugi::node_element);
    
    // 2. pugi::xml_node::append_child (overload: const char_t* name)
    std::string name_str;
    if (ensure_null_terminated(Data, Size, name_str)) {
        // Ensure the name is valid XML (basic check for non-empty and no spaces for simplicity in fuzzing)
        if (!name_str.empty() && name_str.find(' ') == std::string::npos) {
            pugi::xml_node node2 = root.append_child(name_str.c_str());
        }
    }

    // 3. pugi::xml_node::child
    std::string child_name_str;
    if (ensure_null_terminated(Data, Size, child_name_str)) {
        root.child(child_name_str.c_str());
    }

    // 4. pugi::xml_node::append_buffer
    // Prepare a valid XML fragment from input if possible, otherwise use a safe default
    std::string buffer_content;
    if (Size > 0) {
        // Try to find a null terminator to make a valid string for XML fragment
        size_t end = Size;
        for (size_t i = 0; i < Size; ++i) {
            if (Data[i] == 0) {
                end = i;
                break;
            }
        }
        if (end > 0) {
            buffer_content.assign(reinterpret_cast<const char*>(Data), end);
        } else {
            buffer_content = "<dummy/>";
        }
    } else {
        buffer_content = "<dummy/>";
    }
    
    // Ensure the buffer is a valid XML fragment by wrapping if necessary or just trying
    // Fuzzing might provide invalid XML, which is fine as append_buffer handles errors
    root.append_buffer(buffer_content.data(), buffer_content.size());

    // 5. pugi::xml_node::child
    std::string child_name_str2;
    if (ensure_null_terminated(Data, Size, child_name_str2)) {
        root.child(child_name_str2.c_str());
    }

    // 6. pugi::xml_node::child
    std::string child_name_str3;
    if (ensure_null_terminated(Data, Size, child_name_str3)) {
        root.child(child_name_str3.c_str());
    }

    // 7. pugi::xml_node::remove_children
    root.remove_children();

    // 8. pugi::xml_node::child
    std::string child_name_str4;
    if (ensure_null_terminated(Data, Size, child_name_str4)) {
        root.child(child_name_str4.c_str());
    }

    // 9. pugi::xml_node::append_buffer
    std::string buffer_content2;
    if (Size > 0) {
        size_t end = Size;
        for (size_t i = 0; i < Size; ++i) {
            if (Data[i] == 0) {
                end = i;
                break;
            }
        }
        if (end > 0) {
            buffer_content2.assign(reinterpret_cast<const char*>(Data), end);
        } else {
            buffer_content2 = "<dummy2/>";
        }
    } else {
        buffer_content2 = "<dummy2/>";
    }
    root.append_buffer(buffer_content2.data(), buffer_content2.size());

    // 10. pugi::xml_node::child
    std::string child_name_str5;
    if (ensure_null_terminated(Data, Size, child_name_str5)) {
        root.child(child_name_str5.c_str());
    }

    return 0;
}