#include "Plan.h"
#include "Facility.h"
#include <iostream>

Plan::Plan(const int planId, const Settlement &settlement,
           SelectionPolicy *selectionPolicy,
           const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilities(),
      underConstruction(),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(0),
      environment_score(0)
{
}

Plan::~Plan()
{
    clear();
}

Plan::Plan(const Plan &plan)
    : plan_id(plan.plan_id),
      settlement(plan.settlement),
      selectionPolicy(plan.selectionPolicy ? plan.selectionPolicy->clone() : nullptr),
      status(plan.status),
      facilities(),        // Explicitly initialize
      underConstruction(), // Explicitly initialize
      facilityOptions(plan.facilityOptions),
      life_quality_score(plan.life_quality_score),
      economy_score(plan.economy_score),
      environment_score(plan.environment_score)
{
    for (Facility *facility : plan.facilities)
    {
        facilities.push_back(new Facility(*facility, facility->getSettlementName()));
    }
    for (Facility *facility : plan.underConstruction)
    {
        underConstruction.push_back(new Facility(*facility, facility->getSettlementName()));
    }
}

Plan::Plan(Plan &&other) noexcept
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy),
      status(other.status),
      facilities(std::move(other.facilities)),
      underConstruction(std::move(other.underConstruction)),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    other.selectionPolicy = nullptr;
}

void Plan::clear()
{
    delete selectionPolicy;
    selectionPolicy = nullptr;

    for (auto *facility : facilities)
    {
        delete facility;
    }
    facilities.clear();

    for (auto *facility : underConstruction)
    {
        delete facility;
    }
    underConstruction.clear();
}

int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}
int Plan::getEconomyScore() const
{
    return economy_score;
}
int Plan::getEnvironmentScore() const
{
    return environment_score;
}

int Plan::getPlanID() const
{
    return plan_id;
}

SelectionPolicy *Plan::getSelectionPolicy() const
{
    return selectionPolicy;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    std::cout << "planId: " << this->plan_id << std::endl;
    std::cout << "previusPolicy: " << this->selectionPolicy->fullToString() << std::endl;
    delete this->selectionPolicy;
    this->selectionPolicy = selectionPolicy;
    std::cout << "newPolicy: " << this->selectionPolicy->fullToString() << std::endl;
}

void Plan::step()
{
    while ((int)underConstruction.size() < this->settlement.calculateConstructionLimit() && status == PlanStatus::AVALIABLE)
    {
        FacilityType selectedFacility = selectionPolicy->selectMe(*this); // you are a genius itay :)
        underConstruction.push_back(new Facility(selectedFacility, settlement.getName()));
    }
    for (int i = 0; i < (int)underConstruction.size(); i++)
    {
        underConstruction[i]->step();
        if (underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL)
        {
            facilities.push_back(underConstruction[i]);
            life_quality_score += underConstruction[i]->getLifeQualityScore();
            economy_score += underConstruction[i]->getEconomyScore();
            environment_score += underConstruction[i]->getEnvironmentScore();
            underConstruction.erase(underConstruction.begin() + i);
        }
    }
    if ((int)underConstruction.size() == this->settlement.calculateConstructionLimit())
    {
        status = PlanStatus::BUSY;
    }
    else
    {
        status = PlanStatus::AVALIABLE;
    }
}

void Plan::printStatus()
{
    if (status == PlanStatus::AVALIABLE)
    {
        std::cout << "Status : AVALIABLE" << std::endl;
    }
    else
    {
        std::cout << "Status : BUSY" << std::endl;
    }
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

const vector<FacilityType> &Plan::getFacilitiesOpt() const
{
    return facilityOptions;
}

const vector<Facility *> &Plan::getUnderConstruction() const
{
    return underConstruction;
}

void Plan::addFacility(Facility *facility)
{
    facilities.push_back(facility);
}

const Settlement &Plan::getSettlement() const
{
    return settlement;
}

string Plan::printAllFacilities() const
{
    string facilitiesStr = "";
    for (Facility *facility : facilities)
    {
        facilitiesStr += "\n" + facility->toString();
    }
    for (Facility *facility : underConstruction)
    {
        facilitiesStr += "\n" + facility->toString();
    }
    return facilitiesStr + "\n";
}

const string Plan::toString() const
{
    return "planId: " + std::to_string(this->plan_id) + "\nsettlementName: " + this->settlement.getName() +
           "\n" + "planStatus: " + (this->status == PlanStatus::AVALIABLE ? "AVALIABLE" : "BUSY") + "\n" +
           "selectionPolicy: " + selectionPolicy->toString() + "\n" + "LifeQualityScore: " + std::to_string(this->life_quality_score) +
           "\n" + "EconomyScore: " + std::to_string(this->economy_score) + "\n" + "EnvironmentScore: " + std::to_string(this->environment_score) +
           printAllFacilities();
}

void Plan::printbeforeclosed() const
{
    std::cout << "planId: " << plan_id << std::endl;
    std::cout << "settlementName " << settlement.getName() << std::endl;
    std::cout << "LifeQuality_Score: " << life_quality_score << std::endl;
    std::cout << "Economy_Score: " << economy_score << std::endl;
    std::cout << "Environment_Score: " << environment_score << std::endl;
}
