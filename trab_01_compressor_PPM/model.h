#ifndef MODEL_H
#define MODEL_H

#include "definitions.h"
#include "tree_list.h"
#include "tree_map.h"
#include "probability.h"
#include <iostream>

class Model{

private:
    uchar k;
    Tree* tree;
    SymbolSet context_minus_1;
    SymbolUSet exc_mec;    

public:
    Model(uchar k, uint tree_mode = 0U);
    ~Model();
    void updateModel(const Context& context, const Symbol& symbol);
    void clearModel();
    
    ProbabilitiesSet getProbabilities(const Context& context, const Symbol& symbol);
    ProbabilitiesSet getProbabilities(const Symbol& symbol);
    ProbabilityRange getSingleProbability(const Context& context, const Symbol& symbol);
    ProbabilityRange getSingleProbability(const Symbol& symbol);
    Symbol getSymbol(const Context& context, uint count);
    Symbol getSymbol(uint count);
    uint getContextSize(const Context& context);
    uint getContextSize();
    uchar getK() const;
};

#endif