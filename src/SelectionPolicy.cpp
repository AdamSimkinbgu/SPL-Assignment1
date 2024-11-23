#include <SelectionPolicy.h>
#include <iostream>

NaiveSelection::NaiveSelection() {}
const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {}
const string NaiveSelection::toString() const {}
NaiveSelection *NaiveSelection::clone() const {}
NaiveSelection::~NaiveSelection() {}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) {}
const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {}
const string BalancedSelection::toString() const {}
BalancedSelection *BalancedSelection::clone() const {}
BalancedSelection::~BalancedSelection() {}

EconomySelection::EconomySelection() {}
const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {}
const string EconomySelection::toString() const {}
EconomySelection *EconomySelection::clone() const {}
EconomySelection::~EconomySelection() {}

SustainabilitySelection::SustainabilitySelection() {}
const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {}
const string SustainabilitySelection::toString() const {}
SustainabilitySelection *SustainabilitySelection::clone() const {}
SustainabilitySelection::~SustainabilitySelection() {}
