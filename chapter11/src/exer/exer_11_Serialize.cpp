/**
 * @file exer_11_Serialize.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-22
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include <fstream>
#include <string>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

#pragma pack(push, 1)
struct Packet_Packed {
    char   type;    
    int    length;  
    char   flag;    
    double value;
};
#pragma pack(pop)

void serialize(std::ostream& out, const Packet_Packed& p)
{
    out.write(reinterpret_cast<const char*>(&p.type), sizeof(p.type));
    out.write(reinterpret_cast<const char*>(&p.length),sizeof(p.length));
    out.write(reinterpret_cast<const char*>(&p.flag), sizeof(p.flag));
    out.write(reinterpret_cast<const char*>(&p.value), sizeof(p.value));
}

Packet_Packed deserialize(std::istream& in)
{
    Packet_Packed p;
    in.read(reinterpret_cast<char*>(&p.type), sizeof(p.type));
    in.read(reinterpret_cast<char*>(&p.length), sizeof(p.length));
    in.read(reinterpret_cast<char*>(&p.flag), sizeof(p.flag));
    in.read(reinterpret_cast<char*>(&p.value), sizeof(p.value));
    return p;
}

int main()
{
    Packet_Packed packet = {'A', 42, 'X', 3.14};

    // Serialize to file
    std::ofstream outFile(DATA + "packet.txt", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file for writing\n";
        return 1;
    }
    serialize(outFile, packet);
    outFile.close();

    // Deserialize from file
    std::ifstream inFile(DATA + "packet.txt", std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file for reading\n";
        return 1;
    }
    Packet_Packed deserializedPacket = deserialize(inFile);
    inFile.close();

    // Display deserialized data
    std::cout << "Deserialized Packet:\n"
              << "Type: " << deserializedPacket.type << '\n'
              << "Length: " << deserializedPacket.length << '\n'
              << "Flag: " << deserializedPacket.flag << '\n'
              << "Value: " << deserializedPacket.value << '\n';

    return 0;
}