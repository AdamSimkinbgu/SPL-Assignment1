#include <SelectionPolicy.h>
#include <iostream>

NaiveSelection::NaiveSelection() {
    lastSelectedIndex = NULL;
}
const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    if (this->lastSelectedIndex == NULL){
        this->lastSelectedIndex = 0;
        return facilitiesOptions[this->lastSelectedIndex];
    }
    else{
        if (this->lastSelectedIndex == facilitiesOptions.size() - 1){
            this->lastSelectedIndex = 0;
            return facilitiesOptions[this->lastSelectedIndex];
        }
        else{
            this->lastSelectedIndex++;
            return facilitiesOptions[this->lastSelectedIndex];
        }
    }
}

const string NaiveSelection::toString() const {
    // to do
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
    // to do
}
BalancedSelection *BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}
BalancedSelection::~BalancedSelection() {}





EconomySelection::EconomySelection() {
    lastSelectedIndex = NULL;
}
const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    if (facilitiesOptions.size() == 0){
        return;
    }
    int currEconomyMin = facilitiesOptions[0].getEconomyScore();
    int currIdx = 0;
    for (int i = 1; i < facilitiesOptions.size(); i++){
        if (facilitiesOptions[i].getEconomyScore() < currEconomyMin){
            currEconomyMin = facilitiesOptions[i].getEconomyScore();
            int currIdx = i;
        }
    }
    this->lastSelectedIndex = currIdx;
    return facilitiesOptions[currIdx];
    // do i need to check if curridx == lastselectedindex and return other option?
}

const string EconomySelection::toString() const {
    // to do
}
EconomySelection *EconomySelection::clone() const {
    return new EconomySelection(*this);
}
EconomySelection::~EconomySelection() {}

SustainabilitySelection::SustainabilitySelection() {
    lastSelectedIndex = NULL;
}
const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {

}
const string SustainabilitySelection::toString() const {}
SustainabilitySelection *SustainabilitySelection::clone() const {}
SustainabilitySelection::~SustainabilitySelection() {}

//class SelectionPolicy {
//     public:
//         virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
//         virtual const string toString() const = 0;
//         virtual SelectionPolicy* clone() const = 0;
//         virtual ~SelectionPolicy();
// };

// class SustainabilitySelection: public SelectionPolicy {
//     public:
//         SustainabilitySelection();
//         const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
//         const string toString() const override;
//         SustainabilitySelection *clone() const override;
//         ~SustainabilitySelection() override = default;
//     private:
//         int lastSelectedIndex;
// };