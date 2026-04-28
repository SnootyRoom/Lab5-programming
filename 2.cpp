#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
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
    fout.write((char *)&product, sizeof(product));
    fout.close();
}

std::vector<Product> getProducts()
{
    std::vector<Product> products;
    std::ifstream fin("products.bin", std::ios::binary);
    Product product;

    while (fin.read((char *)&product, sizeof(product)))
        products.push_back(product);

    fin.close();

    return products;
}

Product searchProduct(const std::vector<Product> &products, unsigned int target)
{
    auto it = std::find_if(products.begin(), products.end(), [&](const Product &products)
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

void printProducts(const std::vector<Product> &products)
{
    std::cout << "Product list" << std::endl;

    std::cout << std::left
              << std::setw(10) << "Code"
              << std::setw(25) << "Title"
              << std::setw(12) << "Price"
              << std::setw(10) << "Quantity"
              << std::endl;

    for (const Product &item : products)
    {
        std::cout << std::left
                  << std::setw(10) << item.code
                  << std::setw(25) << item.title
                  << std::setw(12) << item.price
                  << std::setw(10) << item.quantity
                  << std::endl;
    }
}

void printProduct(const Product &product)
{
    std::cout << "Info about product" << std::endl;
    std::cout << "Code: " << product.code << std::endl;
    std::cout << "Title: " << product.title << std::endl;
    std::cout << "Price: " << product.price << std::endl;
    std::cout << "Quantity: " << product.quantity << std::endl;
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
        if (ss >> product.code >> product.title >> product.price >> product.quantity)
            recordProduct(product);
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
            printProduct(searchProduct(products, code));
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
    std::cout << "1. product <code> <title> <price> <quantity>" << std::endl;
    std::cout << "2. product-list" << std::endl;
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