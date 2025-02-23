#include "innovationHistory.hpp"

std::vector<int> innovationHistory::nodeHist = std::vector<int>();
std::vector<GeneAtt> innovationHistory::geneHist = std::vector<GeneAtt>();

int innovationHistory::getInnoNode(int connection_inno) {
    for (int i = 0; i < innovationHistory::nodeHist.size(); i++)
        if (innovationHistory::nodeHist[i] == connection_inno)
            return i;
    
    // new node
    innovationHistory::nodeHist.push_back(connection_inno);
    return innovationHistory::nodeHist.size()-1;
}

int innovationHistory::getInnoGene(int from_inno, int to_inno) {
    for (int i = 0; i < innovationHistory::geneHist.size(); i++)
        if (innovationHistory::geneHist[i].from == from_inno && innovationHistory::geneHist[i].to == to_inno)
            return i;
    
    // new node
    innovationHistory::geneHist.emplace_back(from_inno, to_inno);
    return innovationHistory::geneHist.size()-1;
}