#include "Simulation.h"
#include "Action.h"
#include "Auxiliary.h"
#include <iostream>
using std::string;

Simulation::Simulation(const string &config_file_path) : isRunning(false),
                                                         planCounter(0),
                                                         plans(),
                                                         actionsLog(),
                                                         settlements(),
                                                         facilitiesOptions()
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
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);
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

Simulation::~Simulation()
{
    clear();
}

Simulation::Simulation(const Simulation &other) : isRunning(other.isRunning),
                                                  planCounter(other.planCounter),
                                                  plans(),
                                                  actionsLog(),
                                                  settlements(),
                                                  facilitiesOptions()
{
    settlements.clear();
    for (Settlement *settlement : other.settlements)
    {
        Settlement *newSettlement = new Settlement(*settlement);
        settlements.push_back(newSettlement);
    }
    facilitiesOptions.clear();
    for (const auto &facility : other.facilitiesOptions)
    {
        FacilityType *newFacility = facility.clone();
        facilitiesOptions.push_back(*newFacility);
        delete newFacility;
    }
    plans.clear();
    for (const Plan &plan : other.plans)
    {
        Settlement &settlement = getSettlement(plan.getSettlement().getName());
        SelectionPolicy *selectionPolicy = plan.getSelectionPolicy()->clone();
        Plan newPlan(plan.getPlanID(), settlement, selectionPolicy, plan.getFacilitiesOpt());
        plans.push_back(newPlan);
    }
    actionsLog.clear();
    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }
}

Simulation &Simulation::operator=(const Simulation &other)
{
    if (this != &other)
    {
        clear();

        isRunning = other.isRunning;
        planCounter = other.planCounter;
        for (auto settlement : other.settlements)
        {
            Settlement *newSettlement = new Settlement(*settlement);
            settlements.push_back(newSettlement);
        }

        for (const auto &facility : other.facilitiesOptions)
        {
            FacilityType *newFacility = facility.clone();
            facilitiesOptions.push_back(*newFacility);
            delete newFacility;
        }

        for (const Plan &plan : other.plans)
        {
            Settlement &settlement = getSettlement(plan.getSettlement().getName());
            SelectionPolicy *selectionPolicy = plan.getSelectionPolicy()->clone();
            Plan newPlan(plan.getPlanID(), settlement, selectionPolicy, plan.getFacilitiesOpt());
            plans.push_back(newPlan);
        }

        for (auto action : other.actionsLog)
        {
            actionsLog.push_back(action->clone());
        }
    }
    return *this;
}

Simulation::Simulation(Simulation &&other) noexcept
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      plans(std::move(other.plans)),
      actionsLog(std::move(other.actionsLog)),
      settlements(std::move(other.settlements)),
      facilitiesOptions(std::move(other.facilitiesOptions))
{
    other.isRunning = false;
    other.planCounter = 0;
}

Simulation &Simulation::operator=(Simulation &&other) noexcept
{
    if (this != &other)
    {
        clear();

        isRunning = other.isRunning;
        planCounter = other.planCounter;

        plans = std::move(other.plans);
        actionsLog = std::move(other.actionsLog);
        settlements = std::move(other.settlements);
        facilitiesOptions = std::move(other.facilitiesOptions);

        other.isRunning = false;
        other.planCounter = 0;
    }
    return *this;
}

void Simulation::clear()
{
    plans.clear();
    for (auto settlement : settlements)
    {
        delete settlement;
    }
    settlements.clear();

    for (auto action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();
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
    else
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
    Settlement &currSettle = getSettlement(args[1]);

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
    Plan newPlan(planID, currSettle, currSP, facilitiesOptions);
    plans.push_back(newPlan);
    std::cout << "Plan number " << planID << " added." << std::endl;
}

void Simulation::start()
{
    open();
    while (isRunning)
    {
        // ***COMMENT DEBUGGING HELPERS LATER***
        std::string command;
        std::cout << "----------- end of command -----------" << std::endl; // debugging helper
        std::getline(std::cin, command);
        vector<string> commandArgs = Auxiliary::parseArguments(command);
        std::cout << "---> " << command << std::endl; // debugging helper
        if (commandArgs.empty())
        {
            std::cout << "Empty command" << std::endl;
        }
        else if (commandArgs[0] == "log")
        {
            PrintActionsLog *printActionsLog = new PrintActionsLog();
            printActionsLog->act(*this);
        }
        else if (commandArgs[0] == "close")
        {
            close();
            Close *close = new Close();
            close->act(*this);
        }
        else if (commandArgs[0] == "backup")
        {
            BackupSimulation *backup = new BackupSimulation();
            backup->act(*this);
        }
        else if (commandArgs[0] == "restore")
        {
            RestoreSimulation *restore = new RestoreSimulation();
            restore->act(*this);
        }
        else if (commandArgs.size() < 2)
        {
            std::cout << "The command with too little arguments" << std::endl;
        }
        else if (commandArgs[0] == "step")
        {
            SimulateStep *step = new SimulateStep(std::stoi(commandArgs[1]));
            step->act(*this);
        }
        else if (commandArgs[0] == "plan")
        {
            AddPlan *addPlan = new AddPlan(commandArgs[1], commandArgs[2]);
            addPlan->act(*this);
        }
        else if (commandArgs[0] == "settlement")
        {
            AddSettlement *addSettlement = new AddSettlement(commandArgs[1], commandArgs[2] == "0" ? SettlementType::VILLAGE : commandArgs[2] == "1" ? SettlementType::CITY
                                                                                                                                                     : SettlementType::METROPOLIS);
            addSettlement->act(*this);
        }
        else if (commandArgs[0] == "facility")
        {
            FacilityCategory category = commandArgs[2] == "0" ? FacilityCategory::ECONOMY : commandArgs[2] == "1" ? FacilityCategory::LIFE_QUALITY
                                                                                                                  : FacilityCategory::ENVIRONMENT;
            int price = std::stoi(commandArgs[3]);
            int lifeQualityScore = std::stoi(commandArgs[4]);
            int economyScore = std::stoi(commandArgs[5]);
            int environmentScore = std::stoi(commandArgs[6]);
            AddFacility *addFacility = new AddFacility(commandArgs[1], category, price, lifeQualityScore, economyScore, environmentScore);
            addFacility->act(*this);
        }
        else if (commandArgs[0] == "planStatus")
        {
            PrintPlanStatus *printPlanStatus = new PrintPlanStatus(std::stoi(commandArgs[1]));
            printPlanStatus->act(*this);
        }
        else if (commandArgs[0] == "changePolicy")
        {
            ChangePlanPolicy *changePlanPolicy = new ChangePlanPolicy(std::stoi(commandArgs[1]), commandArgs[2]);
            changePlanPolicy->act(*this);
        }
        else
        {
            std::cout << "Unknown command" << std::endl;
        }
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    if (!selectionPolicy)
    {
        std::cout << "Error: Selection policy is null" << std::endl;
        return;
    }
    int nextID = planCounter++;
    Plan newPlan(nextID, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
    std::cout << "Plan added successfully with ID" << nextID << "for settlement: " << settlement.getName() << std::endl; // to delete later
}

void Simulation::addAction(BaseAction *action)
{
    if (!action)
    {
        std::cout << "Error: Action points to null." << std::endl;
        return;
    }
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement)
{
    if (!settlement)
    {
        std::cout << "Error: Settlement points to null." << std::endl;
        return false;
    }
    for (Settlement *exsistsSettlement : settlements)
    {
        if (exsistsSettlement->getName() == settlement->getName())
        {
            std::cout << "Settlement already exists" << std::endl;
            return false;
        }
    }
    settlements.push_back(settlement);
    std::cout << "Settlement added to settlements vector." << std::endl; // to delete later
    return true;
}

bool Simulation::addFacility(FacilityType facility)
{
    for (const FacilityType &existingFacility : facilitiesOptions)
    {
        if (existingFacility.getName() == facility.getName())
        {
            std::cout << "Facility already exists" << std::endl;
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    std::cout << "Facility added to facilityOptions vector." << std::endl; // to delete later
    return true;
}

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

Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (auto &currSettlement : settlements)
    {
        if (currSettlement->getName() == settlementName)
        {
            return *currSettlement;
        }
    }
    throw std::runtime_error("Settlement not found");
}

Plan &Simulation::getPlan(const int planID)
{
    for (auto &currPlan : plans)
    {
        if (currPlan.getPlanID() == planID)
        {
            return currPlan;
        }
    }
    throw std::runtime_error("Plan not found");
}

PlanStatus Plan::getPlanStatus() const
{
    return status;
}

const vector<Plan> &Simulation::getPlans() const
{
    return plans;
}

const vector<Settlement *> &Simulation::getSettlements() const
{
    return settlements;
}

const vector<BaseAction *> &Simulation::getActionsLog() const
{
    return actionsLog;
}

int Simulation::getNextPlanID()
{
    return planCounter++;
}

void Simulation::step()
{
    for (Plan &plan : plans)
    {
        plan.step();
    }
}

void Simulation::close()
{
    clear();
    isRunning = false;
}

void Simulation::open()
{
    isRunning = true;
}