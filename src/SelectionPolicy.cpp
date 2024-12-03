#include "SelectionPolicy.h"
#include <iostream>

bool SelectionPolicy::operator==(const SelectionPolicy &other) const
{
    return typeid(*this) == typeid(other);
}

// ########################## VVV Naive Selection VVV ###################################

NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) 
{}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (this->lastSelectedIndex == (int)facilitiesOptions.size())
    {
        this->lastSelectedIndex = 0;
    }
    else
    {
        this->lastSelectedIndex++;
    }
    return facilitiesOptions[this->lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return "nve";
}

const string NaiveSelection::fullToString() const
{
    return "Naive";
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this);
}

// ########################## VVV Balance Selection VVV #################################

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : 
LifeQualityScore(LifeQualityScore),
EconomyScore(EconomyScore),
EnvironmentScore(EnvironmentScore)
{}

int BalancedSelection::findMaxDiff(int a, int b, int c)
{
    return std::max(std::max(a, b), c) - std::min(std::min(a, b), c);
}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    const FacilityType *bestFacility = &facilitiesOptions[0];
    int templife = LifeQualityScore + facilitiesOptions[0].getLifeQualityScore();
    int tempecon = EconomyScore + facilitiesOptions[0].getEconomyScore();
    int tempenv = EnvironmentScore + facilitiesOptions[0].getEnvironmentScore();
    int currMinDiff = findMaxDiff(templife, tempecon, tempenv);
    for (int i = 1; i < (int)facilitiesOptions.size(); i++)
    {
        int templife = LifeQualityScore + facilitiesOptions[i].getLifeQualityScore();
        int tempecon = EconomyScore + facilitiesOptions[i].getEconomyScore();
        int tempenv = EnvironmentScore + facilitiesOptions[i].getEnvironmentScore();
        int tempDiff = findMaxDiff(templife, tempecon, tempenv);
        if (tempDiff < currMinDiff)
        {
            currMinDiff = tempDiff;
            bestFacility = &facilitiesOptions[i];
        }
    }
    return *bestFacility;
}

const string BalancedSelection::toString() const
{
    return "bal";
}

const string BalancedSelection::fullToString() const
{
    return "Balanced";
}

BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(*this);
}

// ########################## VVV Economy Selection VVV #################################

EconomySelection::EconomySelection() : lastSelectedIndex(-1) 
{}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex++;
    while (facilitiesOptions[this->lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY)
    {
        if (this->lastSelectedIndex == (int)facilitiesOptions.size())
        {
            this->lastSelectedIndex = 0;
        }
        else{
            this->lastSelectedIndex++;
        }
    }
    return facilitiesOptions[this->lastSelectedIndex];
}


const string EconomySelection::toString() const
{
    return "eco";
}

const string EconomySelection::fullToString() const
{
    return "Economy";
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this);
}

// ########################## VVV Sustainability Selection VVV ##########################

SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) 
{}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex++;
    while (facilitiesOptions[this->lastSelectedIndex].getCategory() != FacilityCategory::ENVIRONMENT)
    {
        if (this->lastSelectedIndex == (int)facilitiesOptions.size())
        {
            this->lastSelectedIndex = 0;
        }
        else{
        this->lastSelectedIndex++;
        }
    }
    return facilitiesOptions[this->lastSelectedIndex];
}

const string SustainabilitySelection::toString() const
{
    return "env";
}

const string SustainabilitySelection::fullToString() const
{
    return "Sustainability";
}
SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this);
}
