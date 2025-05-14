#include <iostream>
#include <fstream>
#include "../include/json.hpp" // Make sure this is in your project directory

using json = nlohmann::json;
using namespace std;

int main() {
    // Create JSON object
    json voter = {
        {"id", "V001"},
        {"name", "Hanzla Ahmad"},
        {"age", 21},
        {"voted", true}
    };

    // Save to file
    ofstream file("voter.json");
    file << voter.dump(4);
    file.close();

    // Read from file
    ifstream infile("voter.json");
    json data;
    infile >> data;
    infile.close();

    cout << "Voter Name: " << data["name"] << endl;
    cout << "Has Voted? " << (data["voted"] ? "Yes" : "Nope") << endl;

    return 0;
}
