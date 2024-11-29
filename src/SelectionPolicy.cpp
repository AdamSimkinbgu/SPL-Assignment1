#include <SelectionPolicy.h>
#include <iostream>

NaiveSelection::NaiveSelection() {
    lastSelectedIndex = NULL;
}
const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    if (this->lastSelectedIndex == NULL){
        this->lastSelectedIndex = 0;
    }
    else {
        this->lastSelectedIndex++;
    }
    if (this->lastSelectedIndex == facilitiesOptions.size()){
        this->lastSelectedIndex = 0;
    }
    return facilitiesOptions[this->lastSelectedIndex];
    }


const string NaiveSelection::toString() const {
    return "You are using NaiveSelection";
}

NaiveSelection *NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}
NaiveSelection::~NaiveSelection() {
    lastSelectedIndex = NULL;
}


BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
LifeQualityScore(LifeQualityScore),
EconomyScore(EconomyScore),
EnvironmentScore(EnvironmentScore) 
{}
const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    const FacilityType *bestFacility = &facilitiesOptions[0];
    int templife = LifeQualityScore + facilitiesOptions[0].getLifeQualityScore();
    int tempecon = EconomyScore + facilitiesOptions[0].getEconomyScore();
    int tempenv = EnvironmentScore + facilitiesOptions[0].getEnvironmentScore();
    int currMinDiff = findMaxDiff(templife, tempecon, tempenv);
    for (int i = 1; i < facilitiesOptions.size(); i++){
        int templife = LifeQualityScore + facilitiesOptions[i].getLifeQualityScore();
        int tempecon = EconomyScore + facilitiesOptions[i].getEconomyScore();
        int tempenv = EnvironmentScore + facilitiesOptions[i].getEnvironmentScore();
        int tempDiff = findMaxDiff(templife, tempecon, tempenv);
        if (tempDiff < currMinDiff){
            currMinDiff = tempDiff;
            bestFacility = &facilitiesOptions[i];
        }
    }
    return *bestFacility;
}

int findMaxDiff(int a, int b, int c){
    int max = a;
    if (b > max){
        max = b;
    }
    if (c > max){
        max = c;
    }
    int min = a;
    if (b < min){
        min = b;
    }
    if (c < min){
        min = c;
    }
    return max - min;
}

const string BalancedSelection::toString() const {
    return "You are using BalancedSelection";
}
BalancedSelection *BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}
BalancedSelection::~BalancedSelection() {}





EconomySelection::EconomySelection() {
    lastSelectedIndex = NULL;
}
const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    if (this->lastSelectedIndex == NULL){
        this -> lastSelectedIndex = 0;
    }
    else {
        this->lastSelectedIndex++;
    }
    while (facilitiesOptions[this->lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY){
        if (this->lastSelectedIndex == facilitiesOptions.size()){
            this->lastSelectedIndex = 0;
        }
        this->lastSelectedIndex++;
    }
    return facilitiesOptions[this->lastSelectedIndex];
}

const string EconomySelection::toString() const {
    return "You are using EconomySelection";
}
EconomySelection *EconomySelection::clone() const {
    return new EconomySelection(*this);
}
EconomySelection::~EconomySelection() {}

SustainabilitySelection::SustainabilitySelection() {
    lastSelectedIndex = NULL;
}
const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    if (this->lastSelectedIndex == NULL){
        this -> lastSelectedIndex = 0;
    }
    else {
        this->lastSelectedIndex++;
    }
    while (facilitiesOptions[this->lastSelectedIndex].getCategory() != FacilityCategory::ENVIRONMENT){
        if (this->lastSelectedIndex == facilitiesOptions.size()){
            this->lastSelectedIndex = 0;
        }
        this->lastSelectedIndex++;
    }
    return facilitiesOptions[this->lastSelectedIndex];
}

const string SustainabilitySelection::toString() const {
    return "You are using SustainabilitySelection";
}
SustainabilitySelection *SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}
SustainabilitySelection::~SustainabilitySelection() {}

