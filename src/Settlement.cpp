#include "Settlement.h"
#include <iostream>
using std::string;

Settlement::Settlement(const string &name, SettlementType type):
    name(name), type(type){
        switch (type)
        {
        case SettlementType::VILLAGE:
            constructionLimit = 1;
            break;
        case SettlementType::CITY:
            constructionLimit = 2;
            break;
        case SettlementType::METROPOLIS:
            constructionLimit = 3;
            break;
        default:
            break;
        }
    };

Settlement::Settlement(const Settlement &settlement):
    name(settlement.getName()), type(settlement.getType()){
        switch (type)
        {
        case SettlementType::VILLAGE:
            constructionLimit = 1;
            break;
        case SettlementType::CITY:
            constructionLimit = 2;
            break;
        case SettlementType::METROPOLIS:
            constructionLimit = 3;
            break;
        default:
            break;
        }
    };

const string &Settlement::getName() const{ return name;};

SettlementType Settlement::getType() const{
    return type;
};

const string Settlement::toString() const{
    switch(type){
        case SettlementType::VILLAGE:
            return "Village";
        case SettlementType::CITY:
            return "City";
        case SettlementType::METROPOLIS:
            return "Metropolis";
    }
}

//const string &getName() const;
// SettlementType getType() const;
// const string toString() const;