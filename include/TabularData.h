#ifndef TABULAR_DATA_H
#define TABULAR_DATA_H
#include <vector>
#include <string>
#include <unordered_map>
#include "RandomGenerators.h"

enum class ColumnType {
	INTEGER,
	FLOAT,
	CATEGORICAL,
	DATE,
	BOOLEAN
};

struct ColumnDefinition {
	std::string name;
	ColumnType type;
	std::unordered_map<std::string,std::string> parameters;

};

class TabularData {
public:
	TabularData(int numRows, int numColumns);
	TabularData(int numRows, const std::vector<ColumnDefinition>& columns);

	void setColumnDefinitions(const std::vector<ColumnDefinition>& columns);
	void generate();
	void exportToCSV(const std::string& filename)const;
	void exportToJSON(const std::string& filename)const;

	std::vector<std::vector<std::string>> getData() const;

private:
	int numRows;
	std::vector<ColumnDefinition> columns;
	std::vector<std::vector<std::string>> data;

	std::string generateCellValue(const ColumnDefinition& column);
	std::string generateIntegerValue(const ColumnDefinition& column);
	std::string generateFloatValue(const ColumnDefinition& column);	
	std::string generateCategoricalValue(const ColumnDefinition& column);
	std::string generateDateValue(const ColumnDefinition& columns);
	std::string generateBooleanValue();
};

#endif // !TABULAR_DATA_H
