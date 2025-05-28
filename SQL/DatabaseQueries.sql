JOIN
SELECT 
    v.VoteID,
    vt.VoterName,
    cd.CandidateName,
    pt.PartyName,
    ps.PollingStationName,
    v.VoteTime
FROM Votes v
JOIN Voters vt ON v.VoterID = vt.VoterID
JOIN Candidates cd ON v.CandidateID = cd.CandidateID
JOIN Parties pt ON cd.PartyID = pt.PartyID
JOIN PollingStations ps ON v.PollingStationID = ps.PollingStationID
WHERE v.ElectionID = 1;

------------------------------------------------------------------

Aggregate Function
SELECT 
    cd.CandidateName,
    c.ConstituencyName,
    COUNT(*) AS TotalVotes
FROM Votes v
JOIN Candidates cd ON v.CandidateID = cd.CandidateID
JOIN Constituencies c ON cd.ConstituencyID = c.ConstituencyID
WHERE v.ElectionID = 1
GROUP BY cd.CandidateName, c.ConstituencyName


SELECT COUNT(*) AS TotalVotes
FROM Votes v
WHERE v.ElectionID = 1;

------------------------------------------------------------------

SubQueries
SELECT VoterName
FROM Voters
WHERE VoterID NOT IN (
    SELECT VoterID FROM Votes WHERE ElectionID = 1
);

------------------------------------------------------------------