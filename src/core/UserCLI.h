#ifndef USERCLI_H
#define USERCLI_H
#include <models/Voter.h>

// Forward declaration of Voter
Voter* voterLogin();
void showUserMenu(Voter* voter);
void userPanel();

#endif
