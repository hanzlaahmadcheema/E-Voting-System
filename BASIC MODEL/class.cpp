#include <iostream>
#include <string>
using namespace std;
using namespace std;

//class voter
class Voter {
private:
    string voterName;
    int age;
    string cnicNumber;

//constructor    
public:
    Voter(string name, int Age, string cnic) {
         voterName = name;
         age = Age;
         cnicNumber = cnic;
    }
//getters and setters 
    void setName(string name){
        voterName = name;
    }   
    string getName(){
        return voterName;
    }
    void setAge(int Age){
        age = Age;
    }
    int getAge(){
        return age;
    }
    void setCnic(string cnic){
        cnicNumber = cnic;
    }
    string getCnic(){
        return cnicNumber;
    }
    string toString(){
        return "Voter(Name: " + voterName + ", Age: " + to_string(age) + ", CNIC: " + cnicNumber + ")";
    }
};

//class city
class City {
private:
    string name;
    string province;

//constructor    
public:
    City(string n, string p){
        name = n;
        province = p;
    }

//getters and setters    
    void setName(string n){
        name = n;
    }
    string getName(){
        return name;
    }
    void setProvince(string p){
        province = p;
    }
    string getProvince(){
        return province;
    }
    string toString(){
        return "City(Name: " + name + ", Province: " + province + ")";
    }
};

//class constituency        
class Constituency {
private:
    int code;
    string name;

//constructor
    public:
    Constituency(int c, string n){
        code =c;
        name = n;
    }

    //getters and setters
        void setCode(int c){
            code = c;
        }
        int getCode(){
            return code;
        }
        void setName(string n){
            name = n;
        }
        string getName(){
            return name;
        }
    string toString() {
        return "Constituency(Code: " + to_string(code) + ", Name: " + name + ")";
    }
};

//class polling station
class PollingStation {
private:
    string stationName;
    string location;

//constructor    
public:
    PollingStation(string name, string loca){
        stationName = name;
        location = loca;
    }
    //getters and setters
    void setStationName(string name){
        stationName = name;  
    }
    string getStationName(){
        return stationName;
    }
    void setLocation(string loca){
        location = loca;
    }
    string getLocation(){
        return location;
    }
    string toString(){
        return "PollingStation(Name: " + stationName + ", Location: " + location + ")";
    }
};

//class party
class Party {
private:
    string name;
    string leader;

//constructor    
    public:
    Party(string n, string lead){
        name = n;
        leader = lead;
    }
//getters and setters
    void setName(string n){
        name = n;
    }
    string getName(){
        return name;
    }
    void setLeader(string lead){
        leader = lead;
    }
    string getLeader(){
        return leader;
    }
    string toString() {
        return "Party(Name: " + name + ", Leader: " + leader + ")";
    }
} 