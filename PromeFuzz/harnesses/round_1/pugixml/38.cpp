// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
#include <string_view>

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return std::string_view();
    return std::string_view(reinterpret_cast<const char*>(data + offset), size - offset);
}

static const char* get_null_terminated(const uint8_t* data, size_t size, size_t offset) {
    static thread_local char buffer[4096];
    if (offset >= size) return "";
    
    size_t length = std::min(size - offset, sizeof(buffer) - 1);
    std::memcpy(buffer, data + offset, length);
    buffer[length] = '\0';
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* Data, size_t Size) {
    if (Size < 10) return 0;

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some initial structure
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    
    // Get string parameters from fuzzer input
    size_t offset = 0;
    
    // 1. insert_child_before(const char_t* name, const xml_node& node)
    const char* name1 = get_null_terminated(Data, Size, offset);
    offset += strlen(name1) + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node inserted1 = root.insert_child_before(name1, child2);
    
    // 2. child(const char_t* name)
    const char* name2 = get_null_terminated(Data, Size, offset);
    offset += strlen(name2) + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node found1 = root.child(name2);
    
    // 3. child(const char_t* name)
    const char* name3 = get_null_terminated(Data, Size, offset);
    offset += strlen(name3) + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node found2 = root.child(name3);
    
    // 4. child(const char_t* name)
    const char* name4 = get_null_terminated(Data, Size, offset);
    offset += strlen(name4) + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node found3 = root.child(name4);
    
    // 5. insert_child_before(string_view_t name, const xml_node& node)
    std::string_view name5 = get_string_view(Data, Size, offset);
    offset += name5.length() + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node inserted2 = root.insert_child_before(name5, child3);
    
    // 6. insert_child_before(const char_t* name, const xml_node& node)
    const char* name6 = get_null_terminated(Data, Size, offset);
    offset += strlen(name6) + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node inserted3 = root.insert_child_before(name6, inserted1);
    
    // 7. insert_child_before(string_view_t name, const xml_node& node)
    std::string_view name7 = get_string_view(Data, Size, offset);
    offset += name7.length() + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node inserted4 = root.insert_child_before(name7, child1);
    
    // 8. insert_child_before(const char_t* name, const xml_node& node)
    const char* name8 = get_null_terminated(Data, Size, offset);
    offset += strlen(name8) + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node inserted5 = root.insert_child_before(name8, found1.empty() ? child2 : found1);
    
    // 9. insert_child_before(string_view_t name, const xml_node& node)
    std::string_view name9 = get_string_view(Data, Size, offset);
    offset += name9.length() + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node inserted6 = root.insert_child_before(name9, found2.empty() ? child3 : found2);
    
    // 10. insert_child_before(const char_t* name, const xml_node& node)
    const char* name10 = get_null_terminated(Data, Size, offset);
    offset += strlen(name10) + 1;
    if (offset > Size) offset = Size;
    
    pugi::xml_node inserted7 = root.insert_child_before(name10, found3.empty() ? inserted2 : found3);
    
    return 0;
}