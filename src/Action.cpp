#include "Action.h"
#include <iostream>
#include "Settlement.h"
#include "Plan.h"
#include "SelectionPolicy.h"
#include "Facility.h"



// enum class ActionStatus{
//     COMPLETED, ERROR
// };

// class BaseAction{
//     public:
//         BaseAction();
//         ActionStatus getStatus() const;
//         virtual void act(Simulation& simulation)=0;
//         virtual const string toString() const=0;
//         virtual BaseAction* clone() const = 0;
//         virtual ~BaseAction() = default;

//     protected:
//         void complete();
//         void error(string errorMsg);
//         const string &getErrorMsg() const;

//     private:
//         string errorMsg;
//         ActionStatus status;
// };



ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    ActionStatus status = ActionStatus::ERROR;
    std::cout << "Error: " + errorMsg << std::endl;
}

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {
}

void SimulateStep::act(Simulation &simulation) {
    vector <Plan> plans = simulation.getPlans();
    for (Plan plan : plans){
        plan.step();
    }
}



    

// class SimulateStep : public BaseAction {

//     public:
//         SimulateStep(const int numOfSteps);
//         void act(Simulation &simulation) override;
//         const string toString() const override;
//         SimulateStep *clone() const override;
//     private:
//         const int numOfSteps;
// };

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):
    settlementName(settlementName),
    selectionPolicy(selectionPolicy) {};

void AddPlan::act(Simulation &simulation) {
    vector<Plan> plans = simulation.getPlans();
    for (Plan plan : plans){
        if (plan.getSettlement().getName() == settlementName){
            
        }
    }

}
// class AddPlan : public BaseAction {
//     public:
//         AddPlan(const string &settlementName, const string &selectionPolicy);
//         void act(Simulation &simulation) override;
//         const string toString() const override;
//         AddPlan *clone() const override;
//     private:
//         const string settlementName;
//         const string selectionPolicy;
// };

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType):
settlementName(settlementName), settlementType(settlementType) {};
    


// class AddSettlement : public BaseAction {
//     public:
//         AddSettlement(const string &settlementName,SettlementType settlementType);
//         void act(Simulation &simulation) override;
//         AddSettlement *clone() const override;
//         const string toString() const override;
//     private:
//         const string settlementName;
//         const SettlementType settlementType;
// };



// class AddFacility : public BaseAction {
//     public:
//         AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore);
//         void act(Simulation &simulation) override;
//         AddFacility *clone() const override;
//         const string toString() const override;
//     private:
//         const string facilityName;
//         const FacilityCategory facilityCategory;
//         const int price;
//         const int lifeQualityScore;
//         const int economyScore;
//         const int environmentScore;

// };

// class PrintPlanStatus: public BaseAction {
//     public:
//         PrintPlanStatus(int planId);
//         void act(Simulation &simulation) override;
//         PrintPlanStatus *clone() const override;
//         const string toString() const override;
//     private:
//         const int planId;
// };


// class ChangePlanPolicy : public BaseAction {
//     public:
//         ChangePlanPolicy(const int planId, const string &newPolicy);
//         void act(Simulation &simulation) override;
//         ChangePlanPolicy *clone() const override;
//         const string toString() const override;
//     private:
//         const int planId;
//         const string newPolicy;
// };


// class PrintActionsLog : public BaseAction {
//     public:
//         PrintActionsLog();
//         void act(Simulation &simulation) override;
//         PrintActionsLog *clone() const override;
//         const string toString() const override;
//     private:
// };

// class Close : public BaseAction {
//     public:
//         Close();
//         void act(Simulation &simulation) override;
//         Close *clone() const override;
//         const string toString() const override;
//     private:
// };

// class BackupSimulation : public BaseAction {
//     public:
//         BackupSimulation();
//         void act(Simulation &simulation) override;
//         BackupSimulation *clone() const override;
//         const string toString() const override;
//     private:
// };


// class RestoreSimulation : public BaseAction {
//     public:
//         RestoreSimulation();
//         void act(Simulation &simulation) override;
//         RestoreSimulation *clone() const override;
//         const string toString() const override;
//     private:
// };