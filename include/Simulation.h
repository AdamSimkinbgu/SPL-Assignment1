#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation
{
public:
    Simulation(const string &configFilePath);
    void start();
    void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
    void addAction(BaseAction *action);
    bool addSettlement(Settlement *settlement);
    bool addFacility(FacilityType facility);
    bool isSettlementExists(const string &settlementName);
    bool isPlanExists(const int planID);
    Settlement &getSettlement(const string &settlementName);
    Plan &getPlan(const int planID);
    void addSettlementFromConfig(std::vector<std::string>);
    void addFacilityFromConfig(std::vector<std::string>);
    void addPlanFromConfig(std::vector<std::string>);
    const vector<Plan> &getPlans() const;
    const vector<Settlement *> &getSettlements() const;
    const vector<BaseAction *> &getActionsLog() const;
    const int getNextPlanID() const;
    void step();
    void close();
    void open();

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Plan> plans;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;
};