// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
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

static std::string generate_xml_from_input(const uint8_t* data, size_t size) {
    std::ostringstream oss;
    oss << "<root>";
    for (size_t i = 0; i < size; ++i) {
        uint8_t byte = data[i];
        if (byte == 0 || byte == '<' || byte == '>' || byte == '&' || byte == '"') {
            continue;
        }
        oss << "<tag" << (byte % 100) << " val='" << (char)byte << "'/>";
    }
    oss << "</root>";
    return oss.str();
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string xml_string = generate_xml_from_input(Data, Size);
    
    pugi::xml_document doc;
    if (!doc.load_string(xml_string.c_str())) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // Sequence of API calls as requested
    pugi::xml_node n1 = root.child("tag0");
    pugi::xml_node n2 = n1.child("tag1");
    
    pugi::xml_node proto1 = root.child("tag2");
    pugi::xml_node after1 = n2.insert_copy_after(proto1, n1);
    
    pugi::xml_node n3 = after1.child("tag3");
    pugi::xml_node n4 = n3.insert_copy_after(proto1, n2);
    
    pugi::xml_node n5 = n4.child("tag4");
    pugi::xml_node n6 = n5.child("tag5");
    pugi::xml_node n7 = n6.child("tag6");
    pugi::xml_node n8 = n7.child("tag7");
    
    pugi::xml_node proto2 = root.child("tag8");
    pugi::xml_node after2 = n8.insert_copy_after(proto2, n7);
    
    pugi::xml_node n9 = after2.child("tag9");
    pugi::xml_node fc1 = n9.first_child();
    const pugi::char_t* name1 = fc1.name();
    (void)name1;
    
    pugi::xml_node n10 = fc1.child("tag10");
    pugi::xml_node proto3 = root.child("tag11");
    pugi::xml_node after3 = n10.insert_copy_after(proto3, fc1);
    
    pugi::xml_node n11 = after3.child("tag12");
    pugi::xml_node fc2 = n11.first_child();
    pugi::xml_node n12 = fc2.child("tag13");
    pugi::xml_node lc1 = n12.last_child();
    const pugi::char_t* val1 = lc1.value();
    (void)val1;
    
    pugi::xml_node n13 = lc1.child("tag14");
    pugi::xml_node proto4 = root.child("tag15");
    pugi::xml_node after4 = n13.insert_copy_after(proto4, lc1);
    
    pugi::xml_node n14 = after4.child("tag16");
    pugi::xml_node fc3 = n14.first_child();
    pugi::xml_node n15 = fc3.child("tag17");
    pugi::xml_node fc4 = n15.first_child();
    const pugi::char_t* val2 = fc4.value();
    (void)val2;

    return 0;
}