#include "library.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

BinaryOutputDS readBinary(const std::string& filename)
{
    bool allTestsPass = true;
    std::cout << "Reading the OpenFOAM binary file" << std::endl;
    std::ifstream ifs(filename, std::ios::binary);

    // if (!ifs.is_open()) throw std::runtime_error(" === Unable to open the binary file to begin parsing ===");

    // Reading the Header
    // Declaring memory for header

    __uint32_t magicNumber{};
    __uint8_t formatVersion;
    __uint8_t elementType;
    int32_t nFaces;
    int32_t nCells;

    ifs.read(reinterpret_cast<char*>(&magicNumber), sizeof(magicNumber));
    std::cout << "-- Magic Number -- : Bytes Read: " << ifs.gcount() << std::endl;

    ifs.read(reinterpret_cast<char*>(&formatVersion), sizeof(formatVersion));
    std::cout << "-- Format Version -- : Bytes Read: " << ifs.gcount() << std::endl;

    ifs.read(reinterpret_cast<char*>(&elementType), sizeof(elementType));
    std::cout << "-- Element Type -- : Bytes Read: " << ifs.gcount() << std::endl;

    ifs.read(reinterpret_cast<char*>(&nFaces), sizeof(nFaces));
    std::cout << "-- nFaces -- : Bytes Read: " << ifs.gcount() << std::endl;

    ifs.read(reinterpret_cast<char*>(&nCells), sizeof(nCells));
    std::cout << "-- nCells -- : Bytes Read: " << ifs.gcount() << std::endl;


    if (magicNumber != 0xD07F0A01)
    {
        // throw std::runtime_error("=== MAGIC NUMBER INVALID. FILE POSSIBLY CORRUPTED! === ");
        allTestsPass = false;
    }

    // Reading data
    // declaring memory for data

    // reading SF

    std::vector<double> Sfx(nFaces);
    std::vector<double> Sfy(nFaces);
    std::vector<double> Sfz(nFaces);

    ifs.read(reinterpret_cast<char*>(Sfx.data()), nFaces * sizeof(double));
    std::cout << "-- Sfx -- : Bytes Read: " << ifs.gcount() << std::endl;

    ifs.read(reinterpret_cast<char*>(Sfy.data()), nFaces * sizeof(double));
    std::cout << "-- Sfy -- : Bytes Read: " << ifs.gcount() << std::endl;

    ifs.read(reinterpret_cast<char*>(Sfz.data()), nFaces * sizeof(double));
    std::cout << "-- Sfz -- : Bytes Read: " << ifs.gcount() << std::endl;

    // reading lambda

    std::vector<double> lambda(nFaces);

    ifs.read(reinterpret_cast<char*>(lambda.data()), nFaces * sizeof(double));
    std::cout << "-- lambda -- : Bytes Read: " << ifs.gcount() << std::endl;

    // checking whether the range of lambda's elements is within [0,1]
    auto max_element_lambda = std::max_element(lambda.begin(), lambda.end());
    if (max_element_lambda != lambda.end() && *max_element_lambda > 1)
    {
        allTestsPass = false;
        //throw std::runtime_error("-- Lambda -- invalid, max is greater than 1.");
    }
    auto min_element_lambda = std::min_element(lambda.begin(), lambda.end());
    if ( min_element_lambda != lambda.end() && *min_element_lambda < 0)
    {
        allTestsPass = false;
        // throw std::runtime_error("-- Lambda -- invalid, min is lesser than 0.");
    }




    // reading U

    std::vector<double> Ux(nCells);
    std::vector<double> Uy(nCells);
    std::vector<double> Uz(nCells);

    ifs.read(reinterpret_cast<char*>(Ux.data()), nCells * sizeof(double));
    std::cout << "-- Ux -- : Bytes Read: " << ifs.gcount() << std::endl;

    ifs.read(reinterpret_cast<char*>(Uy.data()), nCells * sizeof(double));
    std::cout << "-- Uy -- : Bytes Read: " << ifs.gcount() << std::endl;

    ifs.read(reinterpret_cast<char*>(Uz.data()), nCells * sizeof(double));
    std::cout << "-- Uz -- : Bytes Read: " << ifs.gcount() << std::endl;


    // reading owner and neighbour

    std::vector<int32_t> owner(nFaces);
    std::vector<int32_t> neighbour(nFaces);

    ifs.read(reinterpret_cast<char*>(owner.data()), nFaces * sizeof(int32_t));
    std::cout << "-- owner -- : Bytes Read: " << ifs.gcount() << std::endl;

    auto max_element_owner = std::max_element(owner.begin(), owner.end());
    if (max_element_owner != owner.end() && *max_element_owner >= nCells)
    {
        allTestsPass = false;
        // throw std::runtime_error("-- owner -- invalid, max is greater than nCells.");
    }
    auto min_element_owner = std::min_element(owner.begin(), owner.end());
    if (min_element_owner != owner.end() && *min_element_owner < 0)
    {
        allTestsPass = false;
        // throw std::runtime_error("-- owner -- invalid, min is lesser than 0.");
    }


    ifs.read(reinterpret_cast<char*>(neighbour.data()), nFaces * sizeof(int32_t));
    std::cout << "-- neighbour -- : Bytes Read: " << ifs.gcount() << std::endl;

    auto max_element_neighbour = std::max_element(neighbour.begin(), neighbour.end());
    if (max_element_neighbour != neighbour.end() && *max_element_neighbour >= nCells)
    {
        allTestsPass = false;
        // throw std::runtime_error("-- neighbour -- invalid, max is greater than nCells.");
    }
    auto min_element_neighbour = std::min_element(neighbour.begin(), neighbour.end());
    if (min_element_neighbour != neighbour.end() && *min_element_neighbour < 0)
    {
        allTestsPass = false;
        // throw std::runtime_error("-- neighbour -- invalid, min is lesser than 0.");
    }

    for (size_t i = 0; i < nFaces; i++)
    {
        if (owner.data()[i] > neighbour.data()[i])
        {
            // throw std::runtime_error("-- owner -- invalid, owner[i] is not < (less than) neighbour[i].");
            allTestsPass = false;
        }
    }

    if (allTestsPass == true)
    {
        std::cout << "===== All basic data tests pass. Data looks good =====" << std::endl;
    }

    // reading reference phi (phi_ref)

    std::vector<double> phi_ref(nFaces);

    ifs.read(reinterpret_cast<char*>(phi_ref.data()), nFaces * sizeof(double));
    std::cout << "-- phi_ref -- : Bytes Read: " << ifs.gcount() << std::endl;


    // Printing OUTPUT
    std::cout << "Magic Number: " << magicNumber << std::endl;
    std::cout << "Format Version: " << static_cast<int>(formatVersion) << std::endl;
    std::cout << "Element Type: " << static_cast<int>(elementType) << std::endl;
    std::cout << "nFaces: " << nFaces << std::endl;
    std::cout << "nCells: " << nCells << std::endl;

    // Adding data to the struct

    BinaryOutputDS output = {
        magicNumber, formatVersion, elementType, nFaces, nCells,
        Sfx, Sfy, Sfz,
        lambda,
        Ux, Uy, Uz,
        owner, neighbour,
        phi_ref
    };

    return output;
}
