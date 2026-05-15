// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
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

static void initializeDocument(pugi::xml_document& doc) {
    const char* xml_content = "<root><child1>text1</child1><child2>text2</child2></root>";
    doc.load_string(xml_content);
}

static std::string createNullTerminatedString(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) return "";
    size_t len = std::min(size, max_len);
    std::string result(reinterpret_cast<const char*>(data), len);
    if (result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

static long extractLong(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(long) > size) return 0;
    long value;
    std::memcpy(&value, data + offset, sizeof(long));
    return value;
}

static unsigned long extractUnsignedLong(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(unsigned long) > size) return 0;
    unsigned long value;
    std::memcpy(&value, data + offset, sizeof(unsigned long));
    return value;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* Data, size_t Size) {
    if (Size < 10) return 0;

    pugi::xml_document doc;
    initializeDocument(doc);
    
    pugi::xml_node root = doc.root();
    if (!root) return 0;

    size_t offset = 0;
    
    std::string child_name = createNullTerminatedString(Data, Size, 50);
    offset += std::min(Size, (size_t)50);
    
    pugi::xml_node child_node = root.child(child_name.c_str());
    
    std::string append_name1 = createNullTerminatedString(Data + offset, Size - offset, 50);
    offset += std::min(Size - offset, (size_t)50);
    
    pugi::xml_node appended1 = child_node.append_child(append_name1.c_str());
    
    std::string append_name2 = createNullTerminatedString(Data + offset, Size - offset, 50);
    offset += std::min(Size - offset, (size_t)50);
    
    pugi::xml_node appended2 = child_node.append_child(append_name2.c_str());
    
    pugi::xml_text text1 = appended2.text();
    long long_val = extractLong(Data, Size, offset);
    offset += sizeof(long);
    text1.set(long_val);
    
    unsigned long ulong_val = extractUnsignedLong(Data, Size, offset);
    offset += sizeof(unsigned long);
    text1.set(ulong_val);
    
    std::string append_name3 = createNullTerminatedString(Data + offset, Size - offset, 50);
    offset += std::min(Size - offset, (size_t)50);
    
    pugi::xml_node appended3 = child_node.append_child(append_name3.c_str());
    
    std::string append_name4 = createNullTerminatedString(Data + offset, Size - offset, 50);
    offset += std::min(Size - offset, (size_t)50);
    
    pugi::xml_node appended4 = child_node.append_child(append_name4.c_str());
    
    pugi::xml_text text2 = appended4.text();
    long long_val2 = extractLong(Data, Size, offset);
    offset += sizeof(long);
    text2.set(long_val2);
    
    unsigned long ulong_val2 = extractUnsignedLong(Data, Size, offset);
    text2.set(ulong_val2);
    
    return 0;
}