#include "Settlement.h"
#include <stdexcept>
using std::string;

Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

Settlement::Settlement(const Settlement &settlement)
    : name(settlement.getName()), type(settlement.getType()) {}

int Settlement::calculateConstructionLimit() const
{
    switch (type)
    {
    case SettlementType::VILLAGE:
        return 1;
    case SettlementType::CITY:
        return 2;
    case SettlementType::METROPOLIS:
        return 3;
    default:
        throw std::invalid_argument("Invalid SettlementType");
    }
}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

const string Settlement::toString() const
{
    return "Settlement " + name + " is a " + typeToString();
}

const string Settlement::typeToString() const
{
    switch (type)
    {
    case SettlementType::VILLAGE:
        return "Village";
    case SettlementType::CITY:
        return "City";
    case SettlementType::METROPOLIS:
        return "Metropolis";
    default:
        return "Unknown";
    }
}