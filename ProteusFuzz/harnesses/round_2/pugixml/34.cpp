// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
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
#include <string>
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    write_dummy_file(Data, Size);

    bool loaded = false;
    if (doc.load_file("./dummy_file")) {
        loaded = true;
    }

    // If file load failed, try to parse as a string buffer.
    // We must ensure the buffer is null-terminated for load_string.
    if (!loaded) {
        // Create a temporary buffer with a null terminator
        std::vector<uint8_t> buffer(Size + 1);
        std::memcpy(buffer.data(), Data, Size);
        buffer[Size] = '\0'; // Ensure null termination
        
        if (doc.load_string(reinterpret_cast<const char*>(buffer.data()))) {
            loaded = true;
        }
    }

    if (!loaded) {
        // If parsing fails completely, create a minimal valid XML structure to test APIs
        doc.append_child("root");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }

    // Helper lambda to safely get a child name from fuzz data
    auto get_name = [&](size_t offset, size_t max_len) -> std::string {
        if (offset >= Size) return "";
        size_t len = 0;
        while (offset + len < Size && len < max_len && Data[offset + len] != 0) {
            len++;
        }
        return std::string(reinterpret_cast<const char*>(Data) + offset, len);
    };

    // 1. remove_child (const char_t* name)
    std::string name1 = get_name(0, 100);
    root.remove_child(name1.c_str());

    // 2. remove_child (const char_t* name)
    std::string name2 = get_name(100, 100);
    root.remove_child(name2.c_str());

    // 3. child (const char_t* name)
    std::string name3 = get_name(200, 100);
    pugi::xml_node node3 = root.child(name3.c_str());

    // 4. child (const char_t* name)
    std::string name4 = get_name(300, 100);
    pugi::xml_node node4 = root.child(name4.c_str());

    // 5. remove_child (const char_t* name)
    std::string name5 = get_name(400, 100);
    root.remove_child(name5.c_str());

    // 6. remove_child (const char_t* name)
    std::string name6 = get_name(500, 100);
    root.remove_child(name6.c_str());

    // 7. child (const char_t* name)
    std::string name7 = get_name(600, 100);
    pugi::xml_node node7 = root.child(name7.c_str());

    // 8. remove_child (const xml_node& n) - Overload 1
    // Try to remove node7 if it exists
    if (node7) {
        root.remove_child(node7);
    }

    // 9. child (const char_t* name)
    std::string name9 = get_name(700, 100);
    pugi::xml_node node9 = root.child(name9.c_str());

    // 10. remove_child (string_view_t name) - Overload 2
    std::string name10 = get_name(800, 100);
    std::string_view sv10 = name10;
    root.remove_child(sv10);

    // 11. remove_child (const char_t* name)
    std::string name11 = get_name(900, 100);
    root.remove_child(name11.c_str());

    // 12. remove_child (const char_t* name)
    std::string name12 = get_name(1000, 100);
    root.remove_child(name12.c_str());

    // 13. remove_child (const char_t* name)
    std::string name13 = get_name(1100, 100);
    root.remove_child(name13.c_str());

    // 14. remove_child (const char_t* name)
    std::string name14 = get_name(1200, 100);
    root.remove_child(name14.c_str());

    // 15. remove_child (const char_t* name)
    std::string name15 = get_name(1300, 100);
    root.remove_child(name15.c_str());

    return 0;
}