#include <iostream>
#include "models/Candidate.h"
#include "models/City.h"
#include "models/Constituency.h"
#include "models/Election.h"
#include "models/Party.h"
#include "models/PollingStation.h"
#include "models/Result.h"
#include "models/Vote.h"
#include "models/Voter.h"
#include "core/UserCLI.h"
#include "core/AdminCLI.h"

// Logic files
    //Candidates
    extern void addCandidate(const Candidate& newCandidate);
    extern void editCandidate(int CandidateID, const string& newName, int newPartyID, int newConstituencyID);
    extern void deleteCandidateByID(int CandidateID);
    extern void listAllCandidates();
    extern void viewCandidatesByConstituency(int constID); 

    //City
    extern void addCity(const City& newCity);
    extern void editCity(int cityID, const string& newName);
    extern void deleteCityByID(int cityID);
    extern void listAllCities();

    //Constituency
    extern void addConstituency(const Constituency& newConst);
    extern void editConstituency(int id, const string& newName);
    extern void deleteConstituency(int id);
    extern void listConstituenciesByCity(int cityID);

    //Election
    extern void createElection(const Election& newElection);
    extern void editElection(int ElectionID, const string& newName, const string& newType, const string& newDate);
    extern void deleteElection(int ElectionID);
    extern void listAllElections();

    //Party
    extern void addParty(const Party& newParty);
    extern void editParty(int id, const string& newName, const string& newSymbol);
    extern void deleteParty(int id);
    extern void listAllParties();

    //PollingStation
    extern void addPollingStation(const PollingStation& newStation);
    extern void editPollingStation(int id, const string& newName, const string& newAddress);
    extern void deletePollingStation(int id);
    extern void listStationsByConstituency(int constID);

    //Vote
    extern void castVote(const Vote& newVote);
    extern void listAllVotes();
    extern string getCurrentTimestamp();

    //Voter
    extern void registerVoter(const Voter& newVoter);
    extern void editVoterByID(int VoterID, const Voter& updatedVoter);
    extern void deleteVoterByID(int VoterID);
    extern void listAllVoters();
    extern void loginByCNIC(const string& VoterCNIC);
    extern void viewProfile(const Voter& v);

    //Result
    extern void computeConstituencyResult(int ElectionID, int ConstituencyID);
    extern void viewResultByConstituency(int ElectionID, int ConstituencyID);
    extern void listAllResults();

    //UserCLI
    extern void userPanel();

    //AdminCLI
    extern void adminPanel();
using namespace std;    

int main() {

    // userPanel();
    adminPanel();
    // Example usage of the functions
    // //Election
    // Election election1(1, "General Election", "General", "2023-10-01");
    // createElection(election1);
    // listAllElections();
    // editElection(1, "Updated Election", "Updated Type", "2023-11-01");
    // //City
    // City city1(1, "Karachi");
    // addCity(city1);
    // listAllCities();
    // editCity(1, "Updated Karachi");
    // //Constituency
    // Constituency const1(1, "Karachi East", 1);
    // addConstituency(const1);
    // listConstituenciesByCity(1);
    // editConstituency(1, "Updated Karachi East");
    // //Party
    // Party party1(1, "Party A", "PA");
    // addParty(party1);
    // listAllParties();
    // editParty(1, "Updated Party A", "UPA");
    // //PollingStation
    // PollingStation station1(1, "Station A", "Address A", 1);
    // addPollingStation(station1);
    // listStationsByConstituency(1);
    // editPollingStation(1, "Updated Station A", "Updated Address A");
    // //Candidate
    // Candidate candidate1(1, "Candidate A", 1, 1);
    // addCandidate(candidate1);
    // listAllCandidates();
    // editCandidate(1, "Updated Candidate A", 1, 1);
    // //Vote
    // Vote vote1(1, 101, 202, 303, 404, 999, getCurrentTimestamp());
    // castVote(vote1);
    // listAllVotes();
    // //Voter
    // Voter voter1(1, "Voter A", "1234567890123", "Male", 30, "Address A", 1, 1);
    // registerVoter(voter1);
    // listAllVoters();
    // editVoterByID(1, Voter(1, "Updated Voter A", "1234567890123", "Gender A", 31, "Updated Address A", 1, 1));
    // //View Profile
    // viewProfile(voter1);
    // //Result
    // Result result1(1, 1, 1, 1, 100, 1);
    // result1.declareResult();
    // result1.displayResultInfo();
    // //Compute Result
    // computeConstituencyResult(1, 1);
    // viewResultByConstituency(1, 1);
    // listAllResults();

    return 0;
}