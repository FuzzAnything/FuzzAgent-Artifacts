// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <string_view>

static void initialize_xml_document(pugi::xml_document& doc) {
    const char* xml_content = 
        "<root>"
        "<child1 attr='value1'>Text1</child1>"
        "<child2 attr='value2'>Text2</child2>"
        "<child3 attr='value3'>Text3</child3>"
        "<child4 attr='value4'>Text4</child4>"
        "<child5 attr='value5'>Text5</child5>"
        "<duplicate>First</duplicate>"
        "<duplicate>Second</duplicate>"
        "<duplicate>Third</duplicate>"
        "</root>";
    doc.load_string(xml_content);
}

static std::string create_name_from_data(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "default";
    
    size_t name_len = std::min<size_t>(data[offset] % 32, size - offset - 1);
    if (name_len == 0) name_len = 1;
    
    std::string name(name_len, '\0');
    for (size_t i = 0; i < name_len; i++) {
        name[i] = static_cast<char>((data[offset + 1 + i] % 26) + 'a');
    }
    return name;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xml_document doc;
    initialize_xml_document(doc);
    
    pugi::xml_node root = doc.child("root");
    if (!root) return 0;
    
    size_t data_offset = 0;
    
    // Function 1: remove_child(string_view_t)
    {
        std::string name1 = create_name_from_data(Data, Size, data_offset);
        data_offset = (data_offset + name1.length() + 1) % Size;
        std::string_view sv1(name1);
        root.remove_child(sv1);
    }
    
    // Function 2: remove_child(const xml_node&)
    {
        pugi::xml_node child_to_remove = root.first_child();
        if (child_to_remove) {
            root.remove_child(child_to_remove);
        }
    }
    
    // Function 3: child(const char_t*)
    {
        std::string name3 = create_name_from_data(Data, Size, data_offset);
        data_offset = (data_offset + name3.length() + 1) % Size;
        if (!name3.empty() && name3.back() != '\0') {
            name3.push_back('\0');
        }
        root.child(name3.c_str());
    }
    
    // Function 4: child(const char_t*)
    {
        std::string name4 = "child";
        if (Size > data_offset) {
            name4 += std::to_string(Data[data_offset] % 10);
            data_offset = (data_offset + 1) % Size;
        }
        name4.push_back('\0');
        root.child(name4.c_str());
    }
    
    // Function 5: remove_child(const char_t*)
    {
        std::string name5 = "duplicate";
        name5.push_back('\0');
        while (root.remove_child(name5.c_str())) {
            // Loop to delete all matches as per hint
        }
    }
    
    // Function 6: remove_child(string_view_t)
    {
        std::string name6 = "nonexistent";
        std::string_view sv6(name6);
        root.remove_child(sv6);
    }
    
    // Function 7: child(const char_t*)
    {
        const char* name7 = "child3";
        root.child(name7);
    }
    
    // Function 8: remove_child(const xml_node&)
    {
        pugi::xml_node child = root.child("child4");
        if (child) {
            root.remove_child(child);
        }
    }
    
    // Function 9: child(const char_t*)
    {
        std::string name9 = "child";
        if (Size > data_offset) {
            name9 += std::to_string(Data[data_offset] % 10);
            data_offset = (data_offset + 1) % Size;
        }
        name9.push_back('\0');
        root.child(name9.c_str());
    }
    
    // Function 10: remove_child(const char_t*)
    {
        const char* name10 = "child5";
        root.remove_child(name10);
    }
    
    // Function 11: remove_child(string_view_t)
    {
        std::string name11 = "text";
        std::string_view sv11(name11);
        root.remove_child(sv11);
    }
    
    // Function 12: remove_child(const xml_node&)
    {
        pugi::xml_node last_child = root.last_child();
        if (last_child) {
            root.remove_child(last_child);
        }
    }
    
    // Function 13: remove_child(const char_t*)
    {
        const char* name13 = "";
        root.remove_child(name13);
    }
    
    // Function 14: remove_child(string_view_t)
    {
        std::string name14 = create_name_from_data(Data, Size, data_offset);
        data_offset = (data_offset + name14.length() + 1) % Size;
        std::string_view sv14(name14);
        root.remove_child(sv14);
    }
    
    // Function 15: remove_child(const xml_node&)
    {
        pugi::xml_node middle_child;
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            middle_child = child;
            break;
        }
        if (middle_child) {
            root.remove_child(middle_child);
        }
    }
    
    return 0;
}