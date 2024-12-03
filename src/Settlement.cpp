#include "Settlement.h"
#include <stdexcept>
using std::string;

// Main constructor
Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

// Copy constructor
Settlement::Settlement(const Settlement &settlement)
    : name(settlement.getName()), type(settlement.getType()){}

// Helper function to calculate the construction limit from SettlementType
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

// Getter for name
const string &Settlement::getName() const
{
    return name;
}

// Getter for type
SettlementType Settlement::getType() const
{
    return type;
}



// String representation of Settlement
const string Settlement::toString() const
{
    return "Settlement " + name + " is a " + typeToString();
}

// Convert SettlementType to string
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