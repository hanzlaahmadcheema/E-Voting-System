#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <string>
#include <vector>
#include "../../include/json.hpp"

struct Admin {
    std::string username;
    std::string password;

    nlohmann::json toJSON() const {
        return nlohmann::json{
            {"username", username},
            {"password", password}
        };
    }

    static Admin fromJSON(const nlohmann::json& j) {
        return Admin{
            j.at("username").get<std::string>(),
            j.at("password").get<std::string>()
        };
    }
};

bool loadAdmins(std::vector<Admin>& adminList);
bool saveAdmins(const std::vector<Admin>& adminList);
bool loginAdmin(const std::vector<Admin>& adminList);

#endif