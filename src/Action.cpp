#include "Action.h"
#include <iostream>
#include "Settlement.h"
#include "Plan.h"
#include "SelectionPolicy.h"
#include "Facility.h"

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::act(Simulation &simulation) {}

const string BaseAction::toString() const {}

BaseAction *BaseAction::clone() const {}

BaseAction::~BaseAction() {}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg)
{
    ActionStatus status = ActionStatus::ERROR;
    std::cout << "Error: " + errorMsg << std::endl;
}

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation)
{
    vector<Plan> plans = simulation.getPlans();
    for (Plan plan : plans)
    {
        plan.step();
    }
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName),
                                                                                selectionPolicy(selectionPolicy) {};

void AddPlan::act(Simulation &simulation)
{
    for (Settlement *settlement : simulation.getSettlements())
    {
        if (settlement->getName() == settlementName)
        {
            for (std::string s : vector<std::string>{"nve", "bal", "eco", "env"})
            {
                if (selectionPolicy == s)
                {
                    SelectionPolicy *currSP;
                    if (s == "env")
                    {
                        currSP = new NaiveSelection();
                    }
                    else if (s == "bal")
                    {
                        currSP = new BalancedSelection(0, 0, 0);
                    }
                    else if (s == "eco")
                    {
                        currSP = new EconomySelection();
                    }
                    else if (s == "env")
                    {
                        currSP = new SustainabilitySelection();
                    }
                    else
                    {
                        currSP = nullptr;
                    }
                    simulation.addPlan(*settlement, currSP);
                    delete currSP;
                    std::cout << "Plan added successfully" << std::endl;
                    return;
                }
            }
        }
    }
    std::cout << "Cannot create this plan." << std::endl;
}

AddPlan *AddPlan::clone() const {}

const string AddPlan::toString() const {}

// ################################################################################################################################################

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName), settlementType(settlementType) {};

void AddSettlement::act(Simulation &simulation)
{
    Settlement *newSettle = new Settlement(settlementName, settlementType);
    simulation.addSettlement(newSettle);
    std::cout << "Settlement " << settlementName << " added successfully." << std::endl;
}

AddSettlement *AddSettlement::clone() const {}

const string AddSettlement::toString() const {}

// ################################################################################################################################################

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {}

AddFacility *AddFacility::clone() const {}

const string AddFacility::toString() const {}

// ################################################################################################################################################

PrintPlanStatus::PrintPlanStatus(int planID) : planId(planID) {}

void PrintPlanStatus::act(Simulation &simulation) {}

PrintPlanStatus *PrintPlanStatus::clone() const {}

const string PrintPlanStatus::toString() const {}

// ################################################################################################################################################

ChangePlanPolicy::ChangePlanPolicy(const int planID, const string &newPolicy) : planId(planID), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {}

ChangePlanPolicy *ChangePlanPolicy::clone() const {}

const string ChangePlanPolicy::toString() const {}

// ################################################################################################################################################

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {}

PrintActionsLog *PrintActionsLog::clone() const {}

const string PrintActionsLog::toString() const {}

// ################################################################################################################################################

Close::Close() {}

void Close::act(Simulation &simulation) {}

Close *Close::clone() const {}

const string Close::toString() const {}

// ################################################################################################################################################

BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation) {}

BackupSimulation *BackupSimulation::clone() const {}

const string BackupSimulation::toString() const {}

// ################################################################################################################################################

RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) {}

RestoreSimulation *RestoreSimulation::clone() const {}

const string RestoreSimulation::toString() const {}
