#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

class Plan;

class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual const string fullToString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual FacilityType selectMe(const Plan& plan) = 0;
        virtual ~SelectionPolicy();
        virtual bool operator==(const SelectionPolicy &other) const;

};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        FacilityType selectMe(const Plan& plan) override;
        const string toString() const override;
        virtual const string fullToString() const override;
        NaiveSelection *clone() const override;
        ~NaiveSelection() override = default;
    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        FacilityType selectMe(const Plan& plan) override;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        virtual const string fullToString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;
        int findMaxDiff(int a, int b, int c);
        void setLifeQualityScore(int LifeQualityScore);
        void setEconomyScore(int EconomyScore);
        void setEnvironmentScore(int EnvironmentScore);
        
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        FacilityType selectMe(const Plan& plan) override;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        virtual const string fullToString() const override;
        EconomySelection *clone() const override;
        ~EconomySelection() override = default;
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        FacilityType selectMe(const Plan& plan) override;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        virtual const string fullToString() const override;
        SustainabilitySelection *clone() const override;
        ~SustainabilitySelection() override = default;
    private:
        int lastSelectedIndex;
};