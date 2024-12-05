#include "Facility.h"
#include <iostream>
using std::string;

FacilityType::FacilityType(const string &name, const FacilityCategory category,
                           const int price, const int lifeQuality_score, const int economy_score, const int environment_score) : name(name),
                                                                                                                                 category(category),
                                                                                                                                 price(price),
                                                                                                                                 lifeQuality_score(lifeQuality_score),
                                                                                                                                 economy_score(economy_score),
                                                                                                                                 environment_score(environment_score) {}

bool FacilityType::operator==(const FacilityType &other) const
{
    return name == other.name &&
           category == other.category &&
           price == other.price &&
           lifeQuality_score == other.lifeQuality_score &&
           economy_score == other.economy_score &&
           environment_score == other.environment_score;
}

FacilityType *FacilityType::clone() const
{
    return new FacilityType(*this);
}

const string &FacilityType::getName() const
{
    return name;
}
int FacilityType::getCost() const
{
    return price;
}

int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const
{
    return environment_score;
}

int FacilityType::getEconomyScore() const
{
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const
{
    return category;
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price,
                   const int lifeQuality_score, const int economy_score, const int environment_score) : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
                                                                                                        settlementName(settlementName),
                                                                                                        status(FacilityStatus::UNDER_CONSTRUCTIONS),
                                                                                                        timeLeft(price)

{
}

Facility::Facility(const FacilityType &type, const string &settlementName) : FacilityType(type.getName(),
                                                                                          type.getCategory(),
                                                                                          type.getCost(),
                                                                                          type.getLifeQualityScore(),
                                                                                          type.getEconomyScore(),
                                                                                          type.getEnvironmentScore()),
                                                                             settlementName(settlementName),
                                                                             status(FacilityStatus::UNDER_CONSTRUCTIONS),
                                                                             timeLeft(type.getCost())
{
}

bool Facility::operator==(const Facility &other) const
{
    return FacilityType::operator==(other) && // Compare inherited FacilityType attributes
           settlementName == other.settlementName &&
           status == other.status &&
           timeLeft == other.timeLeft;
}

const string &Facility::getSettlementName() const
{
    return settlementName;
}

int Facility::getTimeLeft() const
{
    return timeLeft;
}
void Facility::reduceTimeLeft()
{
    this->timeLeft--;
}

void Facility::step()
{
    reduceTimeLeft();
    if (getTimeLeft() == 0)
    {
        status = FacilityStatus::OPERATIONAL;
    }
}

void Facility::setStatus(FacilityStatus status)
{
    this->status = status;
}

const FacilityStatus &Facility::getStatus() const
{
    return status;
}

const string Facility::getStringStatus() const
{
    if (this->getStatus() == FacilityStatus::OPERATIONAL)
    {
        return "OPERATIONAL";
    }
    else
    {
        return "UNDER_CONSTRUCTIONS";
    }
}
const string Facility::toString() const
{
    return "FacilityName: " + this->getName() + "\nFacilityStatus: " + this->getStringStatus();
}
