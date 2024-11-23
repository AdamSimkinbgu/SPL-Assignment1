#include "Plan.h"
#include <iostream>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      facilityOptions(facilityOptions),
      status(PlanStatus::AVALIABLE), // Initialize other members as needed
      life_quality_score(0),
      economy_score(0),
      environment_score(0)
{
}
const int Plan::getlifeQualityScore() const {}
const int Plan::getEconomyScore() const {}
const int Plan::getEnvironmentScore() const {}
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {}
void Plan::step() {}
void Plan::printStatus() {}
const vector<Facility *> &Plan::getFacilities() const {}
void Plan::addFacility(Facility *facility) {}
const string Plan::toString() const {}
