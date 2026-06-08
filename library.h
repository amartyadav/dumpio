#ifndef DUMPIO_LIBRARY_H
#define DUMPIO_LIBRARY_H
#include <string>
#include <vector>

struct BinaryOutputDS
{
    __uint32_t magicNumber{};
    __uint8_t formatVersion;
    __uint8_t elementType;
    int32_t nFaces;
    int32_t nCells;

    std::vector<double> Sfx;
    std::vector<double> Sfy;
    std::vector<double> Sfz;

    std::vector<double> lambda;

    std::vector<double> Ux;
    std::vector<double> Uy;
    std::vector<double> Uz;

    std::vector<int32_t> owner;
    std::vector<int32_t> neighbour;

    std::vector<double> phi_ref;

};

BinaryOutputDS readBinary(const std::string& filename);

#endif // dumpio
