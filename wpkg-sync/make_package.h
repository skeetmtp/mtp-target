
#ifndef MAKE_PAKAGE
#define MAKE_PAKAGE

#include "server.h"

void makePackage(const std::string &packageDescriptionFileName);
void installPackage(const std::string &packageName);
void removePackage(const std::string &packageName);

void getInstalledPackages(std::vector<CPackageSum> &packages);

#endif
