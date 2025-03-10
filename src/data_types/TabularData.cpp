#include "TabularData.h"
#include "RandomGenerators.h"
#include "Distributions.h"
#include "FileExport.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <iomanip>

TabularData::TabularData(int numRows, int numColumns) : numRows(numRows) {
	//Create default column definitions
	std::vector<ColumnDefinition> defaultColumns;
	for (int i = 0;i < numColumns;++i) {
		ColumnDefinition col;
		col.name = "Column_" + std::to_string(i + 1);

		//assign different types to columns
		switch (i % 5) {
		case 0:
			col.type = ColumnType::INTEGER;
			col.parameters["min"] = "0";
			col.parameters["max"] = "100";
			break;
		case 1:
			col.type = ColumnType::FLOAT;
			col.parameters["min"] = "0.0";
			col.parameters["max"] = "1.0";
			break;
		case 2:
			col.type = ColumnType::CATEGORICAL;
			col.parameters["categories"] = "A,B,C,D,E";
			break;
		case 3:
			col.type = ColumnType::DATE;
			col.parameters["start"] = "2020-01-01";
			col.parameters["end"] = "2023-12-31";
			break;
		case 4:
			col.type = ColumnType::BOOLEAN;
			break;
		}
		defaultColumns.push_back(col);
	}
	setColumnDefinitions(defaultColumns);
}

TabularData::TabularData(int numRows, const std::vector<ColumnDefinition>& columns) {
	this->numRows = numRows;
	this->columns = columns;
}

void TabularData::setColumnDefinitions(const std::vector<ColumnDefinition>& columns) {
	this->columns = columns;
}

void TabularData::generate() {
	data.clear();
	data.resize(numRows);
	for (int i = 0;i < numRows;++i) {
		data[i].resize(columns.size());
		for (size_t j = 0;j < columns.size();++j) {
			data[i][j] = generateCellValue(columns[j]);
		}
	}
}

std::string TabularData::generateCellValue(const ColumnDefinition& column) {
    switch (column.type) {
        case ColumnType::INTEGER:
            return generateIntegerValue(column);
        case ColumnType::FLOAT:
            return generateFloatValue(column);
        case ColumnType::CATEGORICAL:
            return generateCategoricalValue(column);
        case ColumnType::DATE:
            return generateDateValue(column);
        case ColumnType::BOOLEAN:
            return generateBooleanValue();
        default:
            return "";
    }
}


std::string TabularData::generateIntegerValue(const ColumnDefinition& column) {
	int min = 0, max = 100;
	auto it = column.parameters.find("min");
	if (it != column.parameters.end()) {
		min = std::stoi(it->second);
	}
	it = column.parameters.find("max");
	if (it != column.parameters.end()) {
		max = std::stoi(it->second);
	}
	return std::to_string(RandomGenerators::getRandomInt(min, max));
}

std::string TabularData::generateFloatValue(const ColumnDefinition& column) {
	double min = 0.0, max = 1.0;

	auto it = column.parameters.find("min");
	if (it != column.parameters.end()) {
		min = std::stod(it->second);
	}

	it = column.parameters.find("max");
	if (it != column.parameters.end()) {
		max = std::stod(it->second);
	}

	std::ostringstream ss;
	ss << std::fixed << std::setprecision(4) << RandomGenerators::getRandomDouble(min, max);
	return ss.str();
}

std::string TabularData::generateCategoricalValue(const ColumnDefinition& column) {
	auto it = column.parameters.find("categories");
	if (it == column.parameters.end()) {
		return "Category_" + std::to_string(RandomGenerators::getRandomInt(1,5));
	}

	std::string categoriesStr = it->second;
	std::vector<std::string> categories;

	std::istringstream ss(categoriesStr);
	std::string category;
	while (std::getline(ss, category, ',')) {
		categories.push_back(category);
	}
	if (categories.empty()) {
		return "Category_1";
	}

	return categories[RandomGenerators::getRandomInt(0, categories.size() - 1)];
}

std::string TabularData::generateDateValue(const ColumnDefinition& column) {
	std::string startDate = "2020-01-01";
	std::string endDate = "2023-12-31";
	auto it = column.parameters.find("start");
	if (it != column.parameters.end()) {
		startDate = it->second;
	}
	it = column.parameters.find("end");
	if (it != column.parameters.end()) {
		endDate = it->second;
	}
	
	//convert dates to time_t
	std::tm start = {};
	std::istringstream(startDate) >> std::get_time(&start, "%Y-%m-%d");
	std::time_t startTime = std::mktime(&start);

	std::tm end = {};
	std::istringstream(endDate) >> std::get_time(&end, "%Y-%m-%d");
	std::time_t endTime = std::mktime(&end);

	//Generate random time between start and end
	std::time_t randomTime = startTime + RandomGenerators::getRandomInt(0, endTime - startTime);

	//convert back to string
	std::tm randomTm;
	localtime_s(&randomTm, &randomTime);
	char buffer[11];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &randomTm);
	return std::string(buffer);
}

std::string TabularData::generateBooleanValue() {
	return RandomGenerators::getRandomInt(0,1) ? "true" : "false";
}

void TabularData::exportToCSV(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file for writing: " + filename);
	}

	//write header
	for (size_t i = 0;i < columns.size();++i) {
		file << columns[i].name;
		if (i < columns.size() - 1) {
			file << ",";
		}
	}
	file << "\n";

	//write data
	for (const auto& row : data) {
		for (size_t i = 0;i < row.size();++i) {
			file << row[i];
			if (i < row.size() - 1) {
				file << ",";
			}
		}
		file << "\n";
	}
	file.close();
}

void TabularData::exportToJSON(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file for writing: " + filename);
	}

	file << "[\n";

	for (size_t i = 0; i < data.size(); ++i) {
		file << "  {\n";

		for (size_t j = 0; j < columns.size(); ++j) {
			file << "    \"" << columns[j].name << "\": ";

			// Format based on column type
			if (columns[j].type == ColumnType::INTEGER ||
				columns[j].type == ColumnType::FLOAT ||
				columns[j].type == ColumnType::BOOLEAN) {
				file << data[i][j];
			}
			else {
				file << "\"" << data[i][j] << "\"";
			}

			if (j < columns.size() - 1) {
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

std::vector<std::vector<std::string>> TabularData::getData() const {
	return data;
}