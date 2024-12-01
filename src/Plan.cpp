#include "Plan.h"
#include "Facility.h"
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
      environment_score(0) {};

Plan::~Plan()
{
    clear();
}

Plan::Plan(const Plan &plan) : plan_id(plan.plan_id),
                               settlement(plan.settlement),
                               selectionPolicy(plan.selectionPolicy),
                               facilityOptions(copyFacilityOptions(plan.facilityOptions))
{
    status = plan.status;
    life_quality_score = plan.life_quality_score;
    economy_score = plan.economy_score;
    environment_score = plan.environment_score;
    for (Facility *facility : plan.facilities)
    {
        facilities.push_back((new Facility(*facility, facility->getSettlementName())));
    }
    for (Facility *facility : plan.underConstruction)
    {
        underConstruction.push_back(new Facility(*facility, facility->getSettlementName()));
    }
}

Plan &Plan::operator=(const Plan &other)
{
    if (this != &other)
    {
        clear();

        plan_id = other.plan_id;
        settlement = other.getSettlement();
        selectionPolicy = other.selectionPolicy;
        status = other.status;
        facilityOptions = other.facilityOptions;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;

        // Deep copy of facilities
        for (auto facility : other.facilities)
        {
            facilities.push_back(new Facility(*facility));
        }

        // Deep copy of underConstruction
        for (auto facility : other.underConstruction)
        {
            underConstruction.push_back(new Facility(*facility));
        }
    }
    return *this;
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

Plan &Plan::operator=(Plan &&other) noexcept
{
    if (this != &other)
    {
        clear();

        plan_id = other.plan_id;
        settlement = other.settlement;
        selectionPolicy = other.selectionPolicy;
        status = other.status;
        facilities = std::move(other.facilities);
        underConstruction = std::move(other.underConstruction);
        facilityOptions = other.facilityOptions;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;

        other.selectionPolicy = nullptr;
    }
    return *this;
}

void Plan::clear()
{
    // Clean up dynamically allocated selectionPolicy
    delete selectionPolicy;
    selectionPolicy = nullptr;

    // Clean up dynamically allocated facilities
    for (auto facility : facilities)
    {
        delete facility;
    }
    facilities.clear();

    for (auto facility : underConstruction)
    {
        delete facility;
    }
    underConstruction.clear();
}

vector<FacilityType> Plan::copyFacilityOptions(const vector<FacilityType> &facilityOptions)
{
    vector<FacilityType> clonedFacilityOptions;
    for (const FacilityType facilityType : facilityOptions)
    {
        (clonedFacilityOptions.push_back(facilityType));
    }
    return clonedFacilityOptions;
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}
const int Plan::getEconomyScore() const
{
    return economy_score;
}
const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

const int Plan::getPlanID() const
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
    std::cout << "previusPolicy: " << this->selectionPolicy << std::endl;
    this->selectionPolicy = selectionPolicy;
    std::cout << "newPolicy: " << this->selectionPolicy->fullToString() << std::endl;
}

void Plan::step()
{
    while (underConstruction.size() < settlement.getConstructionLimit())
    {
        FacilityType selectedFacility = selectionPolicy->selectFacility(facilityOptions);
        underConstruction.push_back(new Facility(selectedFacility, settlement.getName()));
    }
    for (int i = 0; i < underConstruction.size(); i++)
    {
        underConstruction[i]->step();
        if (underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL)
        {
            facilities.push_back(underConstruction[i]);
            underConstruction.erase(underConstruction.begin() + i);
        }
    }
    if (underConstruction.size() == this->settlement.getConstructionLimit())
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
    vector<Facility *> facilities_copy;
    for (auto &facility : facilities)
    {
        facilities_copy.push_back(new Facility(*facility, facility->getSettlementName()));
    }
    return facilities_copy;
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
    string facilities_str = "";
    for (Facility *facility : facilities)
    {
        facilities_str += "\n" + facility->toString();
    }
    for (Facility *facility : underConstruction)
    {
        facilities_str += "\n" + facility->toString();
    }
    return facilities_str;
}

const string Plan::toString() const
{
    return "planId: " + std::to_string(this->plan_id) + " settlementName " + this->settlement.getName() +
           "\n" + "planStatus: " + (this->status == PlanStatus::AVALIABLE ? "AVALIABLE" : "BUSY") + "\n" +
           "selectionPolicy: " + selectionPolicy->toString() + "\n" + "LifeQualityScore: " + std::to_string(this->life_quality_score) +
           "\n" + "EconomyScore: " + std::to_string(this->economy_score) + "\n" + "EnvironmentScore: " + std::to_string(this->environment_score) +
           printAllFacilities();
}
