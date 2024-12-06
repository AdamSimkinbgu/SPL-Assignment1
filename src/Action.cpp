#include "Action.h"
#include <iostream>
#include "Settlement.h"
#include "Plan.h"
#include "SelectionPolicy.h"
#include "Facility.h"

// extern Simulation *backup;

BaseAction::BaseAction() : errorMsg(), status() {}

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

const std::string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

SelectionPolicy *BaseAction::getSelectionPolicy(const string &selectionPolicy)
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
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
    simulation.addAction(this);
}

const string SimulateStep::toString() const
{
    return "SimulateStep " + std::to_string(this->numOfSteps) + " " + getActionStatus() + "\n";
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

// ################################################################################################################################################

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName),
                                                                                selectionPolicy(selectionPolicy) {}

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
                // std::cout << "Plan added successfully" << std::endl; // to delete later
                complete();
                simulation.addAction(this);
                return;
            }
        }
    }
    error("Settlement not found or invalid selection policy");
    simulation.addAction(this);
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}

const string AddPlan::toString() const
{
    return "plan " + this->settlementName + " " + this->selectionPolicy + " " + getActionStatus() + "\n";
}

// ################################################################################################################################################

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName),
                                                                                            settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation)
{
    Settlement *newSettle = new Settlement(settlementName, settlementType);
    if (simulation.addSettlement(newSettle))
    {
        complete();
    }
    else
    {
        error("Settlement already exists");
    }
    simulation.addAction(this);
    // delete newSettle?;
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const
{
    string s = this->settlementType == SettlementType::VILLAGE ? "VILLAGE" : this->settlementType == SettlementType::CITY ? "CITY"
                                                                                                                          : "METROPOLIS";
    return "settlement " + this->settlementName + " " + s + " " + getActionStatus() + "\n";
}

// ################################################################################################################################################

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory,
                         const int price, const int lifeQualityScore, const int economyScore,
                         const int environmentScore) : facilityName(facilityName),
                                                       facilityCategory(facilityCategory),
                                                       price(price),
                                                       lifeQualityScore(lifeQualityScore),
                                                       economyScore(economyScore),
                                                       environmentScore(environmentScore)
{
}

string AddFacility::facilitycatrep() const
{
    if (facilityCategory == FacilityCategory::ECONOMY)
    {
        return "ECONOMY";
    }
    else if (facilityCategory == FacilityCategory::LIFE_QUALITY)
    {
        return "LIFE_QUALITY";
    }
    else
    {
        return "ENVIRONMENT";
    }
}

void AddFacility::act(Simulation &simulation)
{
    FacilityType *facil = new FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (simulation.addFacility(*facil))
    {
        complete();
    }
    else
    {
        error("Facility already exists");
    }
    simulation.addAction(this);
    // delete facil?;
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}

const string AddFacility::toString() const
{
    return "facility " + this->facilityName + " " + facilitycatrep() +
           " " + std::to_string(this->price) +
           " " + std::to_string(this->lifeQualityScore) +
           " " + std::to_string(this->economyScore) +
           " " + std::to_string(this->environmentScore) +
           " " + getActionStatus() + "\n";
}

// ################################################################################################################################################

PrintPlanStatus::PrintPlanStatus(int planID) : planId(planID) {}

void PrintPlanStatus::act(Simulation &simulation)
{
    if (simulation.isPlanExists(planId))
    {
        Plan p = simulation.getPlan(planId);
        std::cout << p.toString(); // drops a line at the end
        complete();
    }
    else
    {
        error("Plan doesn't exist");
    }
    simulation.addAction(this);
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const
{
    return "PrintPlanStatus " + std::to_string(this->planId) + " " + getActionStatus();
}

// ################################################################################################################################################

ChangePlanPolicy::ChangePlanPolicy(const int planID, const string &newPolicy) : planId(planID), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation)
{
    if (simulation.isPlanExists(planId))
    {
        Plan p = simulation.getPlan(planId);
        if (SelectionPolicy *newPol = getSelectionPolicy(newPolicy))
        {
            if (p.getSelectionPolicy() == newPol)
            {
                error("Cannot change selection policy");
                simulation.addAction(this);
                return;
            }
            else
            {
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

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const
{
    return "changePolicy " + std::to_string(this->planId) + " " + this->newPolicy + " " + getActionStatus() + "\n";
}

// ################################################################################################################################################

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation)
{
    for (auto action : simulation.getActionsLog())
    {
        std::cout << action->toString() << std::endl;
    }
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const
{
    return "PrintActionsLog " + getActionStatus() + "\n";
}

// ################################################################################################################################################

Close::Close() {}

void Close::act(Simulation &simulation)
{
    simulation.close();
}

Close *Close::clone() const
{
    return new Close(*this);
}

const string Close::toString() const
{
    return "Close " + getActionStatus() + "\n";
}

// ################################################################################################################################################

BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation)
{
    extern Simulation *backup;
    delete backup;
    backup = new Simulation(simulation);
    simulation.addAction(this);
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const
{
    return "backup " + getActionStatus() + "\n";
}

// ################################################################################################################################################

RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation)
{
    extern Simulation *backup;
    if (backup != nullptr)
    {
        simulation = *backup;
        complete();
    }
    else
    {
        error("No backup available");
    }
    simulation.addAction(this);
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const
{
    return "restore " + getActionStatus() + "\n";
}
