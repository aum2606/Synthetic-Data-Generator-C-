#include "TabularData.h"
#include <iostream>
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

void testTabularDataGeneration() {
	std::cout << "Testing tabular data generation..." << std::endl;
	//test with default columns
	TabularData tabular1(10, 3);
	tabular1.generate();
	auto data1 = tabular1.getData();

	assert(data1.size() == 10);
	assert(data1[0].size() == 3);

	//Test with custom column definitions
	std::vector<ColumnDefinition> columns;

	ColumnDefinition col1;
	col1.name = "ID";
	col1.type = ColumnType::INTEGER;
	col1.parameters["min"] = "1";
	col1.parameters["max"] = "100";

	ColumnDefinition col2;
	col2.name = "Name";
	col2.type = ColumnType::CATEGORICAL;
	col2.parameters["values"] = "Alice,Bob,Charlie,David,Eve";

	ColumnDefinition col3;
	col3.name = "Value";
	col3.type = ColumnType::FLOAT;
	col3.parameters["min"] = "0.0";
	col3.parameters["max"] = "100.0";

	columns.push_back(col1);
	columns.push_back(col2);
	columns.push_back(col3);

	TabularData tabular2(5, columns);
	tabular2.generate();
	auto data2 = tabular2.getData();

	assert(data2.size() == 5);
	assert(data2[0].size() == 3);	
	

	//test export to csv
	std::string csvFilename = "test_tabular_data.csv";
	tabular2.exportToCSV(csvFilename);
	assert(fs::exists(csvFilename));

	//test export to json
	std::string jsonFilename = "test_tabular_data.json";
	tabular2.exportToJSON(jsonFilename);
	assert(fs::exists(jsonFilename));

	//clean up test files
	fs::remove(csvFilename);
	fs::remove(jsonFilename);

	std::cout << "TabularData tests passed! " << std::endl;
}

int main() {
	testTabularDataGeneration();
	return 0;
}