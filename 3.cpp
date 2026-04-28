#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <vector>

struct Sale
{
    std::string date;
    std::string category;
    double summary;
    unsigned int buyer;
};

std::vector<Sale> loadSales()
{
    std::vector<Sale> sales;
    Sale sale;
    std::ifstream fin("sales.csv");
    std::string line;

    while (std::getline(fin, line))
    {
        std::stringstream ss(line);
        std::getline(ss, sale.date, ',');
        ss >> sale.buyer;
        ss.ignore();
        ss >> sale.summary;
        ss.ignore();
        std::getline(ss, sale.category);

        sales.push_back(sale);
    }

    fin.close();

    return sales;
}

void saveSales(const std::vector<Sale> &sales)
{
    std::ofstream fout("filtered_sales.csv");

    for (const Sale &item : sales)
        fout << item.date << "," << item.buyer << "," << item.summary << "," << item.category << std::endl;

    fout.close();
}

double getSummarySales(const std::vector<Sale> &sales)
{
    double summary = 0;

    for (const Sale &item : sales)
        summary += item.summary;

    return summary;
}

std::vector<Sale> filterSalaryByDate(const std::vector<Sale> &sales, const std::string &startDate, const std::string endDate)
{
    std::vector<Sale> filteredSales;

    for (const Sale &item : sales)
    {
        if (item.date <= endDate && item.date >= startDate)
            filteredSales.push_back(item);
    }

    return filteredSales;
}

bool checkDateFormat(std::string date)
{
    unsigned int yearCount = 0, monthCount = 0, dayCount = 0;

    if (date.length() != 10) return false;

    for (unsigned int i = 0; i < 4; i++)
        if (std::isdigit(date[i]))
            yearCount++;

    for (unsigned int i = 5; i < 7; i++)
        if (std::isdigit(date[i]))
            monthCount++;

    for (unsigned int i = 8; i < 10; i++)
        if (std::isdigit(date[i]))
            dayCount++;

    return yearCount == 4 && dayCount == 2 && monthCount == 2 && date[4] == '-' && date[7] == '-';
}

void parseCommand(const std::string &commandLine)
{
    std::vector<Sale> sales = loadSales();
    std::stringstream ss(commandLine);
    std::string command;
    ss >> command;

    if (command == "sales-filter")
    {
        std::string startDate, endDate;

        if (ss >> startDate >> endDate)
            if (checkDateFormat(startDate) && checkDateFormat(endDate))
                saveSales(filterSalaryByDate(sales, startDate, endDate));
            else
                std::cout << "Error: wrong format." << std::endl;
        else
            std::cout<<"Error: wrong agruments"<<std::endl;
    }
    else if (command == "sales-summary")
    {
        std::cout << getSummarySales(sales) << std::endl;
    }
    else if (!command.empty() && command != "exit")
    {
        std::cout << "Unknow command!" << std::endl;
    }
}

int main(int, char **)
{
    std::string commandLine = "";

    std::cout << "Commands:" << std::endl;
    std::cout << "!!! Date format {yyyy-mm-dd} !!!" << std::endl;
    std::cout << "1. sales-filter <start date> <end date>" << std::endl;
    std::cout << "2. sales-summary" << std::endl;
    std::cout << "3. exit" << std::endl;

    while (commandLine != "exit")
    {
        std::cout << "Input command: " << std::endl;
        std::getline(std::cin, commandLine);
        parseCommand(commandLine);
    }

    // for(const Sale item: loadSales()) std::cout<<item.buyer<<" - "<<item.category<<" - "<<item.date<<" - "<<item.summary<<std::endl;

    return 0;
}