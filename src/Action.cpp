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

string BaseAction::getActionStatus() const
{
    if (status == ActionStatus::COMPLETED)
    {
        return "COMPLETED";
    }
    else
    {
        return "ERROR";
    }
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    std::cout << "Error: " + errorMsg << std::endl;
}

const std::string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

SelectionPolicy* BaseAction::getSelectionPolicy(const string &selectionPolicy)
{
    if (selectionPolicy == "nve")
    {
        return new NaiveSelection();
    }
    else if (selectionPolicy == "bal")
    {
        return new BalancedSelection(0, 0, 0);
    }
    else if (selectionPolicy == "eco")
    {
        return new EconomySelection();
    }
    else if (selectionPolicy == "env")
    {
        return new SustainabilitySelection();
    }
    else
    {
        return nullptr;
    }
}

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation)
{
    vector<Plan> plans = simulation.getPlans();
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
complete();
simulation.addAction(this);
}

const string SimulateStep::toString() const {
    return "SimulateStep" + std::to_string(this->numOfSteps) + getActionStatus();
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

// ################################################################################################################################################

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : 
    settlementName(settlementName),
    selectionPolicy(selectionPolicy) {};

void AddPlan::act(Simulation &simulation)
{
    for (Settlement *settlement : simulation.getSettlements())
    {
        if (settlement->getName() == settlementName)
        {
            if (SelectionPolicy *currSP = getSelectionPolicy(selectionPolicy))
            {
                simulation.addPlan(*settlement, currSP);
                delete currSP;
                std::cout << "Plan added successfully" << std::endl; // to delete later
                complete();
                simulation.addAction(this);
                return;
            }
                                
        }
    }
    error("Settlement not found or invalid selection policy");
    simulation.addAction(this);
}


AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}

const string AddPlan::toString() const {
    return "plan" + this->settlementName + this->selectionPolicy + getActionStatus();
}

// ################################################################################################################################################

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : 
settlementName(settlementName),
settlementType(settlementType) {}


void AddSettlement::act(Simulation &simulation)
{
    Settlement *newSettle = new Settlement(settlementName, settlementType);
    if (simulation.addSettlement(newSettle)){
        complete();
    } else {
        error("Settlement already exists");
    };
    simulation.addAction(this);
    // delete newSettle?;
}

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const {
    string s = this->settlementType == SettlementType::VILLAGE ? "VILLAGE" :
     this->settlementType == SettlementType::CITY ? "CITY" :
        "METROPOLIS";
    return "settlement" + this->settlementName + s + getActionStatus();
}

// ################################################################################################################################################

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, 
                         const int price, const int lifeQualityScore, const int economyScore, 
                         const int environmentScore) : 
                         facilityName(facilityName), 
                         facilityCategory(facilityCategory), 
                         price(price), 
                         lifeQualityScore(lifeQualityScore), 
                         economyScore(economyScore), 
                         environmentScore(environmentScore) {
                            if (facilityCategory == FacilityCategory::ECONOMY){
                                string facilitycat = "ECONOMY";
                            }
                            else if (facilityCategory == FacilityCategory::LIFE_QUALITY){
                                string facilitycat = "LIFE_QUALITY";
                            }
                            else {
                                string facilitycat = "ENVIRONMENT";
                            }
                         }

void AddFacility::act(Simulation &simulation) {
    FacilityType *facil = new FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (simulation.addFacility(*facil)){
        complete();
    } else {
        error("Facility already exists");
    };
    simulation.addAction(this);
    // delete facil?;
}


AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}

const string AddFacility::toString() const {
    return "facility" + this->facilityName + this->facilitycat + 
    std::to_string(this->price) + 
    std::to_string(this->lifeQualityScore) + 
    std::to_string(this->economyScore) + 
    std::to_string(this->environmentScore) + 
    getActionStatus();
}

// ################################################################################################################################################

PrintPlanStatus::PrintPlanStatus(int planID) : planId(planID) {}

void PrintPlanStatus::act(Simulation &simulation) {
    if (simulation.isPlanExists(planId)){
        Plan p = simulation.getPlan(planId);
        std::cout << p.toString();
        complete();
    } else {
        error("Plan doesn't exist");
    }
    simulation.addAction(this);
}

PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const {
    return "PrintPlanStatus" + std::to_string(this->planId) + getActionStatus();
}

// ################################################################################################################################################

ChangePlanPolicy::ChangePlanPolicy(const int planID, const string &newPolicy) : planId(planID), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    if (simulation.isPlanExists(planId))
    {
        Plan p = simulation.getPlan(planId);
        if (SelectionPolicy *newPol = getSelectionPolicy(newPolicy)){
            if (p.getSelectionPolicy() == newPol)
            {
             error("Cannot change selection policy");
             simulation.addAction(this);
             return;   
            }
            else{
                p.setSelectionPolicy(newPol);
                complete();
                simulation.addAction(this);
            }
            
        }
        else
        {
            error("Cannot change selection policy");
            simulation.addAction(this);
        }

    }
    else
    {
        error("Cannot change selection policy");
        simulation.addAction(this);
    }
}

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const {
    return "changePolicy" + std::to_string(this->planId) + this->newPolicy + getActionStatus();
}

// ################################################################################################################################################

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    for (BaseAction *action : simulation.getActionsLog())
    {
        std::cout << action->toString() << std::endl;
    }
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const {
    return "PrintActionsLog" + getActionStatus();
}

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
