#include "Simulation.h"
#include <iostream>
using std::string;

Simulation::Simulation(const string &config_file_path) : isRunning(false), planCounter(0)
{
    std::cout << "Simulation started motherfucker" << std::endl;
}

void start();

void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);

void addAction(BaseAction *action);

bool addSettlement(Settlement settlement);

bool addFacility(FacilityType facility);

bool isSettlementExists(const string &settlementName);

Settlement &getSettlement(const string &settlementName);

Plan &getPlan(const int planID);

const vector<Plan> &Simulation::getPlans() const{
    return plans;
}

void step();

void close();

void open();
