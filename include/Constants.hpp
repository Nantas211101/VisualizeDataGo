#pragma once
#include <string>

struct Constants {
    const char acceptedChar[11] = {' ', '0', '1', '2', '3', '4',
                                   '5', '6', '7', '8', '9'};
    const int numOfAcceptedChar = 11;

    struct MAXI {
        static constexpr int numOfNode = 10;
        static constexpr int valueOfNode = 99;
    };

    struct MINI {
        static constexpr int numOfNode = 1;
        static constexpr int valueOfNode = 1;
    };

    static constexpr int posPadding_y = 10;

    const std::string outOfSizeError = "Number of node should in [0, 7]";
    std::string infoError1 =
        "Only integer are accepted, number of node should in [0, 7]";
    std::string infoError2 = "Value of node should in [1, 99]";
};

bool isContain(int n, int l, int r);

bool isContain(std::string s, int l, int r);