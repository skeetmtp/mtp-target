// wpkg_sync manages libraries installation and update
// Copyright (C) 2004 Ace / Melting Pot
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef SERVER_H
#define SERVER_H

#include <string>

struct CPackageSum
{
	CPackageSum() { Init = false; }
	CPackageSum(std::string name, std::string version, std::string description, std::string maintainer);
	CPackageSum(std::string raw);
	std::string toString();
	
	const std::string &name() const { return Name; }
	const std::string &version() const { return Version; }
	time_t			   date() const { return Date; }
	const std::string &description() const { return Description; }
	const std::string &maintainer() const { return Maintainer; }
	const std::string &localVersion() const { return LocalVersion; }
	
	void localVersion(const std::string &localVersion) { LocalVersion = localVersion; }

private:

	bool Init;

	std::string Name;
	std::string Version;
	time_t Date;
	std::string Description;
	std::string Maintainer;

	// if not empty, it means that the package is already installed with this version
	std::string LocalVersion;
};

void uploadPackage(const std::string &packageFileName);
void downloadPackage(const std::string &packageName);

void getPackageList(std::vector<CPackageSum> &packages);

void updatePackageList();

void downloadFile(const std::string &src, const std::string &dst);
void uploadFile(const std::string &src, const std::string &dst);

#endif
