#include "Simulation.h"
#include "Action.h"
#include <iostream>
using std::string;

Simulation::Simulation(const string &config_file_path) : isRunning(false), planCounter(0)
{
    std::cout << "Simulation object created" << std::endl;
}

// this function will start the simulation process
void Simulation::start()
{
    isRunning = true;
    std::cout << "Simulation mode 'isRunning' is set to True" << std::endl;
}

// here we will be adding a new facility into the vector - conditions applied
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    // settlement name needs to be checked before the call to this function
    if (!selectionPolicy)
    {
        std::cout << "Error: Selection policy is null" << std::endl;
        return;
    }
    int nextID = planCounter++;
    Plan newPlan(nextID, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
    std::cout << "Plan added successfully with ID" << nextID << "for settlement: " << settlement.getName() << std::endl;
}

// wtf is this? I think this is how it's supposed to be
void Simulation::addAction(BaseAction *action)
{
    if (!action)
    {
        std::cout << "Error: Action points to null." << std::endl;
        return;
    }
    actionsLog.push_back(action);
    std::cout << "Action added to log, status: " << (action->getStatus() == ActionStatus::COMPLETED ? "COMPLETED." : "ERROR.") << std::endl;
}

// this will add a settlement into the appropriate vector - conditions applied
bool Simulation::addSettlement(Settlement *settlement)
{
    if (!settlement)
    {
        std::cout << "Error: Settlement points to null." << std::endl;
        return;
    }
    settlements.push_back(settlement);
    std::cout << "Settlement added to settlements vector." << std::endl;
}

// this is going to add a facility into the facilties vector
bool Simulation::addFacility(FacilityType facility)
{
    facilitiesOptions.push_back(facility);
    std::cout << "Facility added to facilityOptions vector." << std::endl;
}

// checks if a settlement DOES exist... pff...
bool Simulation::isSettlementExists(const string &settlementName)
{
    for (Settlement *settlement : settlements)
    {
        if (settlement->getName() == settlementName)
        {
            return true;
        }
    }
    return false;
}

// given a name of a settlement, returns the address to it
Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (Settlement *settlement : settlements)
    {
        if (settlement->getName() == settlementName)
        {
            return *settlement;
        }
    }
    return; // not sure what to do if not found, maybe the check for if the settlement exists needs to be outside of this function
}

// given the plan id, returns the address to it
Plan &Simulation::getPlan(const int planID)
{
    for (Plan currPlan : plans)
    {
        if (currPlan.getPlanID() == planID)
        {
            return currPlan;
        }
    }
}

const PlanStatus Plan::getPlanStatus() const
{
    return status;
}

// returns the address to the plans vector
const vector<Plan> &Simulation::getPlans() const
{
    return plans;
}

// returns the address to the settlement pointers vector
const vector<Settlement *> &Simulation::getSettlements() const
{
    return settlements;
}

const int Simulation::getNextPlanID() const
{
    return planCounter;
}

// executes a single step in the simulation
void Simulation::step()
{
    for (Plan plan : plans)
    {
        if (plan.getPlanStatus() == PlanStatus::AVALIABLE)
        {
        }
    }
}

// closes the simulation after applying changes and a save, printing the actions done at last
void Simulation::close() {}

// the fk is this too??
void Simulation::open() {}
