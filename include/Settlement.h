#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType
{
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement
{
public:
    Settlement(const string &name, SettlementType type);
    Settlement(const Settlement &settlement);
    int calculateConstructionLimit() const;
    const string &getName() const;
    SettlementType getType() const;
    const string toString() const;
    const string typeToString() const;

private:
    const string name;
    SettlementType type;
};