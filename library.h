#ifndef DUMPIO_LIBRARY_H
#define DUMPIO_LIBRARY_H
#include <string>
#include <vector>
typedef float real_t;

struct BinaryOutputDS
{
    __uint32_t magicNumber{};
    __uint8_t formatVersion;
    __uint8_t elementType;
    int32_t nFaces;
    int32_t nCells;

    std::vector<real_t> Sfx;
    std::vector<real_t> Sfy;
    std::vector<real_t> Sfz;

    std::vector<real_t> lambda;

    std::vector<real_t> Ux;
    std::vector<real_t> Uy;
    std::vector<real_t> Uz;

    std::vector<int32_t> owner;
    std::vector<int32_t> neighbour;

    std::vector<real_t> phi_ref;

};

BinaryOutputDS readBinary(const std::string& filename);

#endif // dumpio
