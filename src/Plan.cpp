#include "Plan.h"
#include <iostream>

Plan::Plan(const int planId, const Settlement &settlement, 
SelectionPolicy *selectionPolicy, 
const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      facilityOptions(facilityOptions),
      status(PlanStatus::AVALIABLE), // Initialize other members as needed
      life_quality_score(0),
      economy_score(0),
      environment_score(0)
      {
      };

Plan::Plan(const Plan &plan):
    Plan(plan.plan_id, plan.settlement, plan.selectionPolicy,
     plan.cloneFacilityOptions(plan.facilityOptions))
{
    status = plan.status;
    life_quality_score = plan.life_quality_score;
    economy_score = plan.economy_score;
    environment_score = plan.environment_score;
    for (auto &facility : plan.facilities){
        facilities.push_back((new Facility(*facility, facility->getSettlementName())));
    }
    for (auto &facility : plan.underConstruction){
        underConstruction.push_back(new Facility(*facility, facility->getSettlementName()));
    }
}

vector<FacilityType> &Plan::cloneFacilityOptions(const vector<FacilityType> &facilityOptions) const {
    vector<FacilityType> clonedFacilityOptions;
    for (auto &facilityType : facilityOptions){
        clonedFacilityOptions.push_back(FacilityType(facilityType));
    }
    return clonedFacilityOptions;
}

const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}
const int Plan::getEconomyScore() const {
    return economy_score;
}
const int Plan::getEnvironmentScore() const {
    return environment_score;
}
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    this->selectionPolicy = selectionPolicy;
}
void Plan::step() {}
void Plan::printStatus() {}
const vector<Facility *> &Plan::getFacilities() const {
    vector<Facility *> facilities_copy;
    for (auto &facility : facilities){
        facilities_copy.push_back(new Facility(*facility, facility->getSettlementName()));
    }
}

void Plan::addFacility(Facility *facility) {
    
}
const string Plan::toString() const {}
