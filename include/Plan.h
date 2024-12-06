#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus
{
    AVALIABLE,
    BUSY,
};

class Plan
{
public:
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
    ~Plan();
    Plan(const Plan &other);
    Plan &operator=(const Plan &other)=delete;
    Plan(Plan &&other) noexcept;
    Plan &operator=(Plan &&other) noexcept=delete;

    int getlifeQualityScore() const;
    int getEconomyScore() const;
    int getEnvironmentScore() const;
    int getPlanID() const;

    SelectionPolicy *getSelectionPolicy() const;
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    void step();
    PlanStatus getPlanStatus() const;
    void printStatus();
    const vector<Facility *> &getFacilities() const;
    const vector<FacilityType> &getFacilitiesOpt() const;
    const vector<Facility *> &getUnderConstruction() const;
    void addFacility(Facility *facility);
    const string toString() const;
    const Settlement &getSettlement() const;
    string printAllFacilities() const;
    void printbeforeclosed() const;

private:
    int plan_id;
    const Settlement &settlement;
    SelectionPolicy *selectionPolicy; // What happens if we change this to a reference?
    PlanStatus status;
    vector<Facility *> facilities;
    vector<Facility *> underConstruction;
    const vector<FacilityType> &facilityOptions;
    int life_quality_score, economy_score, environment_score;
    void clear();
};