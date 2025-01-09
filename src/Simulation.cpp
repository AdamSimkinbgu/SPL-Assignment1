#include "Simulation.h"
#include "Action.h"
#include "Auxiliary.h"
#include <iostream>
#include <cstdlib> // Include the necessary header file

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
        throw std::runtime_error("Error: Unable to open configuration file: " + config_file_path);
    }
    std::string line;
    while (std::getline(configFile, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);
        if (arguments.size() == 3 && arguments[0] == "settlement")
        {
            addSettlementFromConfig(arguments);
        }
        else if (arguments.size() == 7 && arguments[0] == "facility")
        {
            addFacilityFromConfig(arguments);
        }
        else if (arguments.size() == 3 && arguments[0] == "plan")
        {
            addPlanFromConfig(arguments);
        }
        else
        {
            std::cerr << "Error: Unknown command in config file: " << arguments[0] << std::endl;
        }
    }
    configFile.close();
    // std::cout << "Simulation object created" << std::endl;
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
    if (args[2] != "0" && args[2] != "1" && args[2] != "2")
    {
        std::cout << "Error: Unknown settlement type" << std::endl;
        return;
    }
    if (isSettlementExists(args[1]))
    {
        std::cout << "Error: Settlement already exists" << std::endl;
        return;
    }
    SettlementType settleType = args[2] == "0" ? SettlementType::VILLAGE : args[2] == "1" ? SettlementType::CITY
                                                                                          : SettlementType::METROPOLIS;
    Settlement *temp = new Settlement(args[1], settleType);
    settlements.push_back(temp);
    // std::cout << "Settlement " << args[1] << " added." << std::endl;
}

void Simulation::addFacilityFromConfig(std::vector<std::string> args)
{
    std::string nameFacility = args[1];
    for (FacilityType &facility : facilitiesOptions)
    {
        if (facility.getName() == nameFacility)
        {
            std::cout << "Error: Facility already exists" << std::endl;
            return;
        }
    }
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
    else
    {
        std::cout << "Error: Unknown facility category" << std::endl;
        return;
    }
    for (int i = 3; i < 7; i++)
    {
        for (char s : args[i])
        {
            if (!std::isdigit(s))
            {
                std::cout << "Error: Invalid input" << std::endl;
                return;
            }
        }
    }
    int price = std::stoi(args[3]);
    int lifeq_impact = std::stoi(args[4]);
    int eco_impact = std::stoi(args[5]);
    int env_impact = std::stoi(args[6]);
    facilitiesOptions.push_back(FacilityType(nameFacility, catFacility, price, lifeq_impact, eco_impact, env_impact));
    // std::cout << "Facility " << nameFacility << " added." << std::endl;
}

bool isAllDigits(const std::string &str)
{
    for (char c : str)
    {
        if (!std::isdigit(c))
        {
            return false;
        }
    }
    return true;
}

void Simulation::addPlanFromConfig(std::vector<std::string> args)
{
    if (!isSettlementExists(args[1]))
    {
        std::cout << "Error: Settlement does not exist" << std::endl;
        return;
    }
    int planID = getNextPlanID();
    Settlement &currSettle = getSettlement(args[1]);
    SelectionPolicy *currSP;
    if (args[2] == "nve")
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
        std::cout << "Error: Unknown selection policy" << std::endl;
        return;
    }
    Plan newPlan(planID, currSettle, currSP, facilitiesOptions);
    plans.push_back(newPlan);
    // std::cout << "Plan number " << planID << " added." << std::endl;
}

void Simulation::start()
{
    open();
    while (isRunning)
    {
        // ***COMMENT DEBUGGING HELPERS LATER***
        std::string command;
        // std::cout << "----------- end of command -----------" << std::endl; // debugging helper
        std::getline(std::cin, command);
        vector<string> commandArgs = Auxiliary::parseArguments(command);
        // std::cout << "---> " << command << std::endl; // debugging helper
        if (commandArgs.empty())
        {
            std::cout << "Empty command" << std::endl;
        }
        else if (commandArgs.size() == 1)
        {
            if (commandArgs[0] == "log")
            {
                PrintActionsLog *printActionsLog = new PrintActionsLog();
                printActionsLog->act(*this);
            }
            else if (commandArgs[0] == "close")
            {
                close();
                Close *close = new Close();
                close->act(*this);
                clear();
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
        }
        else if (commandArgs.size() == 2)
        {
            if (commandArgs[0] == "step")
            {
                if (!isAllDigits(commandArgs[1]))
                {
                    std::cout << "Error: Invalid input for step" << std::endl;
                    continue;
                }
                SimulateStep *step = new SimulateStep(std::stoi(commandArgs[1]));
                step->act(*this);
            }
            else if (commandArgs[0] == "planStatus")
            {
                if (!isAllDigits(commandArgs[1]))
                {
                    std::cout << "Error: Invalid input for planStatus" << std::endl;
                    continue;
                }
                PrintPlanStatus *printPlanStatus = new PrintPlanStatus(std::stoi(commandArgs[1]));
                printPlanStatus->act(*this);
            }
        }
        else if (commandArgs.size() == 3)
        {
            if (commandArgs[0] == "plan")
            {
                if (!isSettlementExists(commandArgs[1]))
                {
                    std::cout << "Error: Settlement does not exist" << std::endl;
                    continue;
                }
                if (commandArgs[2] != "nve" && commandArgs[2] != "bal" && commandArgs[2] != "eco" && commandArgs[2] != "env")
                {
                    std::cout << "Error: Unknown selection policy" << std::endl;
                    continue;
                }
                AddPlan *addPlan = new AddPlan(commandArgs[1], commandArgs[2]);
                addPlan->act(*this);
            }
            else if (commandArgs[0] == "settlement")
            {
                if (isSettlementExists(commandArgs[1]))
                {
                    std::cout << "Error: Settlement already exists" << std::endl;
                    continue;
                }
                if (commandArgs[2] != "0" && commandArgs[2] != "1" && commandArgs[2] != "2")
                {
                    std::cout << "Error: Unknown settlement type" << std::endl;
                    continue;
                }
                SettlementType settleType = commandArgs[2] == "0" ? SettlementType::VILLAGE : commandArgs[2] == "1" ? SettlementType::CITY
                                                                                                                    : SettlementType::METROPOLIS;
                AddSettlement *addSettlement = new AddSettlement(commandArgs[1], settleType);
                addSettlement->act(*this);
            }
            else if (commandArgs[0] == "changePolicy")
            {
                if (!isPlanExists(std::stoi(commandArgs[1])))
                {
                    std::cout << "Error: Plan does not exist" << std::endl;
                    continue;
                }
                if (commandArgs[2] != "nve" && commandArgs[2] != "bal" && commandArgs[2] != "eco" && commandArgs[2] != "env")
                {
                    std::cout << "Error: Unknown selection policy" << std::endl;
                    continue;
                }
                ChangePlanPolicy *changePlanPolicy = new ChangePlanPolicy(std::stoi(commandArgs[1]), commandArgs[2]);
                changePlanPolicy->act(*this);
            }
        }
        else if (commandArgs.size() == 7)
        {
            if (commandArgs[0] == "facility")
            {
                for (FacilityType &facility : facilitiesOptions)
                {
                    if (facility.getName() == commandArgs[1])
                    {
                        std::cout << "Error: Facility already exists" << std::endl;
                        return;
                    }
                }
                for (int i = 3; i < 7; i++)
                {
                    for (char s : commandArgs[i])
                    {
                        if (!std::isdigit(s))
                        {
                            std::cout << "Error: Invalid input" << std::endl;
                            return;
                        }
                    }
                }
                if (commandArgs[2] != "0" && commandArgs[2] != "1" && commandArgs[2] != "2")
                {
                    std::cout << "Error: Unknown settlement type" << std::endl;
                    return;
                }
                FacilityCategory category = commandArgs[2] == "0" ? FacilityCategory::LIFE_QUALITY : commandArgs[2] == "1" ? FacilityCategory::ECONOMY
                                                                                                                           : FacilityCategory::ENVIRONMENT;
                int price = std::stoi(commandArgs[3]);
                int lifeQualityScore = std::stoi(commandArgs[4]);
                int economyScore = std::stoi(commandArgs[5]);
                int environmentScore = std::stoi(commandArgs[6]);
                AddFacility *addFacility = new AddFacility(commandArgs[1], category, price, lifeQualityScore, economyScore, environmentScore);
                addFacility->act(*this);
            }
        }
        else
        {
            std::cout << "Error: Unknown command" << std::endl;
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
    // std::cout << "Plan added successfully with ID" << nextID << "for settlement: " << settlement.getName() << std::endl; // to delete later
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
            std::cout << "Error: Settlement already exists" << std::endl;
            return false;
        }
    }
    settlements.push_back(settlement);
    // std::cout << "Settlement added to settlements vector." << std::endl; // to delete later
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
    // std::cout << "Facility added to facilityOptions vector." << std::endl; // to delete later
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
    // clear();
    isRunning = false;
}

void Simulation::open()
{
    isRunning = true;
}