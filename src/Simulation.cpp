#include "Simulation.h"
#include "Action.h"
#include "Auxiliary.h"
#include <iostream>
using std::string;

Simulation::Simulation(const string &config_file_path) : isRunning(false), planCounter(0)
{
    std::cout << "The simulation has started" << std::endl;
    std::ifstream configFile(config_file_path);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Unable to open configuration file: " + config_file_path);
    }

    std::string line;
    while (std::getline(configFile, line))
    {
        // Ignore comments and empty lines
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        // Parse the line into arguments
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);

        // Process each type of command
        if (arguments[0] == "settlement")
        {
            addSettlementFromConfig(arguments);
        }
        else if (arguments[0] == "facility")
        {
            addFacilityFromConfig(arguments);
        }
        else if (arguments[0] == "plan")
        {
            addPlanFromConfig(arguments);
        }
        else
        {
            std::cerr << "Unknown command in config file: " << arguments[0] << std::endl;
        }
    }

    configFile.close();
    std::cout << "Simulation object created" << std::endl;
}

void Simulation::addSettlementFromConfig(std::vector<std::string> args)
{
    SettlementType settleType = args[2] == "0" ? SettlementType::VILLAGE : args[2] == "1" ? SettlementType::CITY
                                                                                          : SettlementType::METROPOLIS;
    Settlement *temp = new Settlement(args[1], settleType);
    settlements.push_back(temp);
    std::cout << "Settlement " << args[1] << " added." << std::endl;
}

void Simulation::addFacilityFromConfig(std::vector<std::string> args)
{
    std::string nameFacility = args[1];
    FacilityCategory catFacility;
    if (args[2] == "0")
    {
        catFacility = FacilityCategory::LIFE_QUALITY;
    }
    else if (args[2] == "1")
    {
        catFacility = FacilityCategory::ECONOMY;
    }
    else if (args[2] == "2")
    {
        catFacility = FacilityCategory::ENVIRONMENT;
    }
    int price = std::stoi(args[3]);
    int lifeq_impact = std::stoi(args[4]);
    int eco_impact = std::stoi(args[5]);
    int env_impact = std::stoi(args[6]);
    facilitiesOptions.push_back(FacilityType(nameFacility, catFacility, price, lifeq_impact, eco_impact, env_impact));
    std::cout << "Facility " << nameFacility << " added." << std::endl;
}

void Simulation::addPlanFromConfig(std::vector<std::string> args)
{
    int planID = getNextPlanID();
    Settlement currSettle = getSettlement(args[1]);
    SelectionPolicy *currSP;
    if (args[2] == "env")
    {
        currSP = new NaiveSelection();
    }
    else if (args[2] == "bal")
    {
        currSP = new BalancedSelection(0, 0, 0);
    }
    else if (args[2] == "eco")
    {
        currSP = new EconomySelection();
    }
    else if (args[2] == "env")
    {
        currSP = new SustainabilitySelection();
    }
    else
    {
        currSP = nullptr;
    }
    const std::vector<FacilityType> facil;
    plans.push_back(Plan(planID, currSettle, currSP, facil));
    // delete currSP;
    std::cout << "Plan number " << planID << " added." << std::endl;
    // facil blayt
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
    if (!selectionPolicy) //is this realy nessesary? we do it in AddPlan::act
    {
        std::cout << "Error: Selection policy is null" << std::endl;
        return;
    }
    int nextID = planCounter++;
    Plan newPlan(nextID, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
    std::cout << "Plan added successfully with ID" << nextID << "for settlement: " << settlement.getName() << std::endl; // maybe not needed
}

// wtf is this? I think this is how it's supposed to be
void Simulation::addAction(BaseAction *action)
{
    if (!action)// not needed also i think
    {
        std::cout << "Error: Action points to null." << std::endl;
        return;
    }
    actionsLog.push_back(action);
    std::cout << "Action added to log, status: " << (action->getStatus() == ActionStatus::COMPLETED ? "COMPLETED." : "ERROR.") << std::endl;
    // last line not needed i think
}

// this will add a settlement into the appropriate vector - conditions applied
bool Simulation::addSettlement(Settlement *settlement)
{
    if (!settlement) // is this really nessesary? we do it in AddSettlement::act
    {
        std::cout << "Error: Settlement points to null." << std::endl;
        return false;
    }
    for (Settlement * exsistsSettlement: settlements)
    {
        if (exsistsSettlement->getName() == settlement->getName())
        {
            std::cout << "Settlement already exists" << std::endl;
            return false;
        }
    }
    settlements.push_back(settlement);
    std::cout << "Settlement added to settlements vector." << std::endl;
    return true;
}

// this is going to add a facility into the facilties vector
bool Simulation::addFacility(FacilityType facility)
{
    for (const FacilityType &existingFacility : facilitiesOptions)
    {
        if (existingFacility.getName() == facility.getName())
        {
            std::cout << "Facility already exists" << std::endl; // to delete later
            return false; // Facility cannot be added
        }
    }
    facilitiesOptions.push_back(facility);
    std::cout << "Facility added to facilityOptions vector." << std::endl; // to delete later
    return true;
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

bool Simulation::isPlanExists(const int planID)
{
    for (Plan currPlan : plans)
    {
        if (currPlan.getPlanID() == planID)
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

const vector<BaseAction *> &Simulation::getActionsLog() const
{
    return actionsLog;
}

const int Simulation::getNextPlanID() const
{
    return planCounter;
}

// executes a single step in the simulation
void Simulation::step()
{
    for (Plan &plan : plans)
    {
        plan.step();
    }
}

// closes the simulation after applying changes and a save, printing the actions done at last
void Simulation::close() {}

void Simulation::open() {
    isRunning = true;
}
