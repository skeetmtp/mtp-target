
#ifndef FTP_H
#define FTP_H

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

void openConnection(const std::string &uri);
void closeConnection();

void uploadPackage(const std::string &packageFileName);
void downloadPackage(const std::string &packageName);

void getPackageList(std::vector<CPackageSum> &packages);

void updatePackageList();

#endif
