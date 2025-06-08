#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H



#include <custom/config.h>

struct Admin {
    string username;
    string password;

    nlohmann::json toJSON() const {
        return nlohmann::json{
            {"username", username},
            {"password", password}
        };
    }

    static Admin fromJSON(const nlohmann::json& j) {
        return Admin{
            j.at("username").get<string>(),
            j.at("password").get<string>()
        };
    }
};

bool loadAdmins(vector<Admin>& adminList);
bool saveAdmins(const vector<Admin>& adminList);
bool loginAdmin(const vector<Admin>& adminList);

#endif