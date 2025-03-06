#include "FileExport.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

void FileExport::exportToCSV(const std::string& filename,
	const std::vector<std::string>& headers,
	const std::vector<std::vector<std::string>>& data) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file for writing: " + filename);
	}

	//writer header
	for (size_t i = 0;i < headers.size();++i) {
		file << headers[i];
		if (i < headers.size() - 1) {
			file << ",";
		}
	}
	file << "\n";

    // Write data
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            // Check if the value contains commas or quotes
            if (row[i].find(',') != std::string::npos || row[i].find('"') != std::string::npos) {
                // Escape quotes and wrap in quotes
                std::string escapedValue = row[i];
                size_t pos = 0;
                while ((pos = escapedValue.find('"', pos)) != std::string::npos) {
                    escapedValue.replace(pos, 1, "\"\"");
                    pos += 2;
                }
                file << "\"" << escapedValue << "\"";
            }
            else {
                file << row[i];
            }

            if (i < row.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}

void FileExport::exportToJSON(const std::string& filename,
    const std::vector<std::string>& headers,
    const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    file << "[\n";

    for (size_t i = 0; i < data.size(); ++i) {
        file << "  {\n";

        for (size_t j = 0; j < headers.size() && j < data[i].size(); ++j) {
            file << "    \"" << headers[j] << "\": ";

            // Check if the value is numeric
            bool isNumeric = true;
            for (char c : data[i][j]) {
                if (!std::isdigit(c) && c != '.' && c != '-' && c != '+' && c != 'e' && c != 'E') {
                    isNumeric = false;
                    break;
                }
            }

            // Check if the value is boolean
            bool isBoolean = (data[i][j] == "true" || data[i][j] == "false");

            if (isNumeric || isBoolean) {
                file << data[i][j];
            }
            else {
                // Escape special characters in JSON strings
                std::string escapedValue = data[i][j];
                size_t pos = 0;
                while ((pos = escapedValue.find('\\', pos)) != std::string::npos) {
                    escapedValue.replace(pos, 1, "\\\\");
                    pos += 2;
                }
                pos = 0;
                while ((pos = escapedValue.find('"', pos)) != std::string::npos) {
                    escapedValue.replace(pos, 1, "\\\"");
                    pos += 2;
                }
                pos = 0;
                while ((pos = escapedValue.find('\n', pos)) != std::string::npos) {
                    escapedValue.replace(pos, 1, "\\n");
                    pos += 2;
                }

                file << "\"" << escapedValue << "\"";
            }

            if (j < headers.size() - 1 && j < data[i].size() - 1) {
                file << ",";
            }
            file << "\n";
        }

        file << "  }";
        if (i < data.size() - 1) {
            file << ",";
        }
        file << "\n";
    }

    file << "]\n";
    file.close();
}

void FileExport::exportToXML(const std::string& filename,
	const std::string& rootElement,
	const std::string& rowElement,
	const std::vector<std::string>& headers,
	const std::vector<std::vector<std::string>>& data) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file for writing: " + filename);
	}
	file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	file << "<" << rootElement << ">\n";
	for (const auto& row : data) {
		file << "  <" << rowElement << ">\n";
        for (size_t i = 0;i < headers.size() && i < row.size();++i) {
            //escape special characters in xml
			std::string escapedValue = row[i];
			size_t pos = 0;
            while ((pos = escapedValue.find('&', pos)) != std::string::npos) {
				escapedValue.replace(pos, 1, "&amp;");
				pos += 5;
			
            }
            pos = 0;
            while ((pos = escapedValue.find('<', pos)) != std::string::npos) {
				escapedValue.replace(pos, 1, "&lt;");
				pos += 4;
            }
            pos = 0;
            while ((pos = escapedValue.find('>', pos)) != std::string::npos) {
				escapedValue.replace(pos, 1, "&gt;");
				pos += 4;
            }
			pos = 0;
            while ((pos = escapedValue.find('"', pos)) != std::string::npos) {
                escapedValue.replace(pos, 1, "&quot;");
                pos += 6;
            }
            pos = 0;
            while ((pos = escapedValue.find('\'', pos)) != std::string::npos) {
                escapedValue.replace(pos, 1, "&apos;");
                pos += 6;
            }
			file << "    <" << headers[i] << ">" << escapedValue << "</" << headers[i] << ">\n";
        }
		file << "  </" << rowElement << ">\n";
	}
	file.close();
}

void FileExport::exportToSQLite(const std::string& filename,
    const std::string& tablename,
    const std::vector<std::string>& headers,
    const std::vector<std::vector<std::string>>& data) {

	//this is a simplified version that generates sql statements to insert data into a table
	// in a real implementation, you would use a library like sqlite3 to interact with the database

	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file for writing: " + filename);
	}

    //create table statement
	file << "CREATE TABLE IF NOT EXISTS" << tablename << " (\n";
	for (size_t i = 0; i < headers.size(); ++i) {
		file << "  " << headers[i] << " TEXT";
		if (i < headers.size() - 1) {
			file << ",";
		}
		file << "\n";
	}
	file << ");\n";

    //insert statements
    for (const auto& row : data) {
		file << "INSERT INTO " << tablename << " (";
        for (size_t i = 0; i < headers.size(); ++i) {
			file << headers[i];
            if (i < headers.size() - 1) {
				file << ", ";
            }
        }
		file << ") VALUES (";
        for (size_t i = 0;i < row.size() && i < headers.size();++i) {
            //escape single quotes in sql
			std::string escapedValue = row[i];
			size_t pos = 0;
            while ((pos = escapedValue.find('\'', pos)) != std::string::npos) {
				escapedValue.replace(pos, 1, "''");
				pos += 2;
            }
			file << "'" << escapedValue << "'";
            if (i < row.size() - 1 && i < headers.size() - 1) {
				file << ", ";
            }
        }
		file << ");\n";
    }
    file.close();

    std::cout << "Note: This is a SQL script, not an actual SQLite database file.\n";
    std::cout << "To create a real SQLite database, you would need to use the SQLite C/C++ API.\n";
}

void FileExport::exportToINI(const std::string& filename,
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& sections) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    for (const auto& section : sections) {
        file << "[" << section.first << "]\n";

        for (const auto& keyValue : section.second) {
            file << keyValue.first << " = " << keyValue.second << "\n";
        }

        file << "\n";
    }

    file.close();
}