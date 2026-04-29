#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <iomanip>

struct Product
{
    double price;
    unsigned int code;
    unsigned int quantity;
    char title[50];
};

void recordProduct(const Product &product)
{
    std::ofstream fout("products.bin", std::ios::binary | std::ios::app);
    if (!fout.is_open())
    {
        std::cout << "File not found!" << std::endl;
        return ;
    }

    fout.write((char *)&product, sizeof(product));
    fout.close();
}

std::vector<Product> getProducts()
{
    std::vector<Product> products;
    std::ifstream fin("products.bin", std::ios::binary);
    if (!fin.is_open())
    {
        std::cout << "File not found!" << std::endl;
        return {};
    }

    Product product;

    while (fin.read((char *)&product, sizeof(product)))
        products.push_back(product);

    fin.close();

    return products;
}

Product searchProduct(const std::vector<Product> &products, unsigned int target)
{
    auto it = std::find_if(products.begin(), products.end(), [target](const Product &products)
                           { return products.code == target; });

    if (it != products.end())
        return *it;

    std::cout << "No product with this code!" << std::endl;
    return {0.0, 0, 0, ""};
}

double summaryProducts(const std::vector<Product> &products)
{
    double summary = 0;
    for (const Product &item : products)
    {
        summary += item.quantity * item.price;
    }

    return summary;
}


void printTableHeader()
{
    std::cout << std::left
              << std::setw(10) << "Code"
              << std::setw(50) << "Title"
              << std::setw(12) << "Price"
              << std::setw(10) << "Quantity"
              << std::endl;
}

void printProduct(const Product &product)
{

    std::cout << std::left
              << std::setw(10) << product.code
              << std::setw(50) << product.title
              << std::setw(12) << product.price
              << std::setw(10) << product.quantity
              << std::endl;
}

void printProducts(const std::vector<Product> &products)
{
    std::cout << "Product list" << std::endl;
    printTableHeader();

    for (const Product &item : products)
    {
        printProduct(item);
    }
}

void parseCommand(const std::string &commandLine)
{
    std::stringstream ss(commandLine);
    std::string command;
    unsigned int code = 0;
    std::vector<Product> products = getProducts();
    ss >> command;

    if (command == "product")
    {
        Product product;
        std::string tempTitle = "";
        if (ss >> product.code >> tempTitle >> product.price >> product.quantity)
        {
            std::strncpy(product.title, tempTitle.c_str(), sizeof(product.title));
            recordProduct(product);
        }
        else
            std::cout << "Error: wrong format." << std::endl;
    }
    else if (command == "product-list")
    {
        printProducts(products);
    }
    else if (command == "search")
    {
        if (ss >> code)
        {
            printTableHeader();
            printProduct(searchProduct(products, code));
        }
        else
            std::cout << "Error: wrong format." << std::endl;
    }
    else if (command == "product-summary")
    {
        std::cout << summaryProducts(products) << std::endl;
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
    std::cout << "!!! Input title less then 50 symbols !!!" << std::endl;
    std::cout << "1. product <code> <title> <price> <quantity>" << std::endl;
    std::cout << "2. product-list" << std::endl;b
    std::cout << "3. search <id>" << std::endl;
    std::cout << "4. product-summary" << std::endl;
    std::cout << "5. exit" << std::endl;

    while (commandLine != "exit")
    {
        std::cout << "Input command: " << std::endl;
        std::getline(std::cin, commandLine);
        parseCommand(commandLine);
    }

    return 0;
}