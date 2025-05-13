CREATE TABLE Voters (
    VoterID INT AUTO_INCREMENT PRIMARY KEY,
    VoterName VARCHAR(100),
    VoterCNIC VARCHAR(15) UNIQUE,
    VoterAge INT,
    VoterGender ENUM('Male', 'Female'),
    VoterAddress VARCHAR(255),
    ConstituencyID INT,
    PollingStationID INT,
    FOREIGN KEY (ConstituencyID) REFERENCES Constituencies(ConstituencyID)
    FOREIGN KEY (PollingStationID) REFERENCES PollingStations(PollingStationID)
);
CREATE TABLE Constituencies (
    ConstituencyID INT AUTO_INCREMENT PRIMARY KEY,
    ConstituencyName VARCHAR(100),
    CityID INT,
    FOREIGN KEY (CityID) REFERENCES Cities(CityID)
);
CREATE TABLE Cities (
    CityID INT AUTO_INCREMENT PRIMARY KEY,
    CityName VARCHAR(100)
);
CREATE TABLE Elections (
    ElectionID INT AUTO_INCREMENT PRIMARY KEY,
    ElectionName VARCHAR(100),
    ElectionType VARCHAR(100),
    ElectionDate DATE
);
CREATE TABLE PollingStations 
    PollingStationID INT AUTO_INCREMENT PRIMARY KEY,
    PollingStationName VARCHAR(100),
    PollingStationAddress VARCHAR(255),
    ConstituencyID INT,
    FOREIGN KEY (ConstituencyID) REFERENCES Constituencies(ConstituencyID)
);
CREATE TABLE Parties (
    PartyID INT AUTO_INCREMENT PRIMARY KEY,
    PartyName VARCHAR(100),
    PartySymbol VARCHAR(50)
);
CREATE TABLE Candidates (
    CandidateID INT AUTO_INCREMENT PRIMARY KEY,
    CandidateName VARCHAR(100),
    PartyID INT,
    ConstituencyID INT,
    FOREIGN KEY (PartyID) REFERENCES Parties(PartyID),
    FOREIGN KEY (ConstituencyID) REFERENCES Constituencies(ConstituencyID)
);
CREATE TABLE Votes (
    VoteID INT AUTO_INCREMENT PRIMARY KEY,
    VoterID INT,
    CandidateID INT,
    PollingStationID INT,
    ElectionID INT,
    VoteTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (VoterID) REFERENCES Voters(VoterID),
    FOREIGN KEY (CandidateID) REFERENCES Candidates(CandidateID),
    FOREIGN KEY (PollingStationID) REFERENCES PollingStations(PollingStationID),
    FOREIGN KEY (ElectionID) REFERENCES Elections(ElectionID)
);
CREATE TABLE Results (
    ResultID INT AUTO_INCREMENT PRIMARY KEY,
    PollingStationID INT,
    CandidateID INT,
    ElectionID INT,
    TotalVotes INT,
    FOREIGN KEY (PollingStationID) REFERENCES PollingStations(PollingStationID),
    FOREIGN KEY (CandidateID) REFERENCES Candidates(CandidateID),
    FOREIGN KEY (ElectionID) REFERENCES Elections(ElectionID)
);
 