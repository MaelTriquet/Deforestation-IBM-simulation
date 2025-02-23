#pragma once
#include <vector>

struct GeneAtt {
    int from;
    int to;

    GeneAtt(int from_, int to_):
        from{from_},
        to{to_}
    {}
};

class innovationHistory {
public:
    static std::vector<int> nodeHist;
    static std::vector<GeneAtt> geneHist;

    static int getInnoNode(int connection_inno);
    static int getInnoGene(int from_inno, int to_inno);
};
