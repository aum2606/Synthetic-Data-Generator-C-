#ifndef FILE_EXPORT_H
#define FILE_EXPORT_H

#include <string>
#include <vector>
#include <unordered_map>

class FileExport {
public:
	//Export data to csv file
	static void exportToCSV(const std::string& filename,
		const std::vector<std::string>& headers, 
		const std::vector<std::vector<std::string>>& data);

	//export data to json file
	static void exportToJSON(const std::string& filename,
		const std::vector<std::string>& headers,
		const std::vector<std::vector<std::string>>& data);

	//export data to xml file
	static void exportToXML(const std::string& filename,
		const std::string& rootElement,
		const std::string& rowElement,
		const std::vector<std::string>& headers,
		const std::vector<std::vector<std::string>>& data);

	//export data to SQLITE database
	static void exportToSQLite(const std::string& filename,
		const std::string& tableName,
		const std::vector<std::string>& headers,
		const std::vector<std::vector<std::string>>& data);

	//export key-value data to INI file
	static void exportToINI(const std::string& filename,
		const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& sections);

};

#endif // !FILE_EXPORT_H

