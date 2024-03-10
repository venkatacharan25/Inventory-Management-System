#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

char getch()
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}
#endif

using namespace std;

// Define a simple User class for login
class User
{
private:
    string username;
    string password;

public:
    User(string username, string password) : username(username), password(password) {}

    string getUsername() const
    {
        return username;
    }

    string getPassword() const
    {
        return password;
    }
};

class Product
{
private:
    int id;
    string name;
    string category;
    double price;
    int quantity;

public:
    Product(int id, string name, string category, double price, int quantity)
    {
        this->id = id;
        this->name = name;
        this->category = category;
        this->price = price;
        this->quantity = quantity;
    }

    int getId() const
    {
        return id;
    }

    void setId(int id)
    {
        this->id = id;
    }

    string getName() const
    {
        return name;
    }

    void setName(string name)
    {
        this->name = name;
    }

    string getCategory() const
    {
        return category;
    }

    void setCategory(string category)
    {
        this->category = category;
    }

    double getPrice() const
    {
        return price;
    }

    void setPrice(double price)
    {
        this->price = price;
    }

    int getQuantity() const
    {
        return quantity;
    }

    void setQuantity(int quantity)
    {
        this->quantity = quantity;
    }
};

class Inventory
{
private:
    vector<Product> products;

public:
    void addProduct(Product product)
    {
        bool found = false;
        for (auto &p : products)
        {
            if (p.getId() == product.getId())
            {
                cout << "Id already Exist." << endl;
                found = true;
                break;
            }
        }
        if (!found)
        {
            products.push_back(product);
            cout << "Product added successfully." << endl;
            cout << "-----------------------------------------------------------" << endl;
        }
    }

    void removeProduct(int id)
{
    bool found = false;
    for (auto i = products.begin(); i != products.end(); i++)
    {
        if (i->getId() == id)
        {
            char removeOption;
            cout << "Do you want to remove the entire product (E) or specify a quantity (Q)? ";
            cin >> removeOption;

            if (removeOption == 'E' || removeOption == 'e')
            {
                products.erase(i);
                found = true;
                cout << "Product removed successfully." << endl;
                cout << "-----------------------------------------------------------" << endl;
            }
            else if (removeOption == 'Q' || removeOption == 'q')
            {
                int quantityToRemove;
                cout << "Enter the quantity to remove: ";
                cin >> quantityToRemove;

                if (quantityToRemove >= i->getQuantity())
                {
                    products.erase(i);
                    found = true;
                    cout << "Product removed successfully." << endl;
                    cout << "-----------------------------------------------------------" << endl;
                }
                else
                {
                    i->setQuantity(i->getQuantity() - quantityToRemove);
                    cout << "Quantity updated successfully." << endl;
                    cout << "-----------------------------------------------------------" << endl;
                }
            }
            else
            {
                cout << "Invalid option. No changes made." << endl;
                cout << "-----------------------------------------------------------" << endl;
            }

            break;
        }
    }
    if (!found)
    {
        cout << "Id does not exist" << endl;
        cout << "-----------------------------------------------------------" << endl;
    }
}

    Product *findProduct(int id)
    {
        for (auto i = products.begin(); i != products.end(); i++)
        {
            if (i->getId() == id)
            {
                return &(*i);
            }
        }
        return nullptr;
    }

    void updateProduct(int id, string name, string category, double price, int quantity)
    {
        bool found = false;
        for (auto i = products.begin(); i != products.end(); i++)
        {
            if (i->getId() == id)
            {
                i->setName(name);
                i->setCategory(category);
                i->setPrice(price);
                i->setQuantity(quantity);
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "ID does not exist." << endl;
        }
    }

    void printProduct() const
    {
        for (auto i = products.begin(); i != products.end(); i++)
        {
            cout << "ID : " << i->getId() << endl;
            cout << "Name : " << i->getName() << endl;
            cout << "Category : " << i->getCategory() << endl;
            cout << "Price : $ " << i->getPrice() << endl;
            cout << "Quantity : " << i->getQuantity() << endl;
        }
    }

    void saveInventoryToFile(string filename)
    {
        ofstream file;
        file.open(filename, ios::out | ios::app);
        for (int i = 0; i < products.size(); i++)
        {
            Product p = products[i];
            file << p.getId() << "," << p.getName() << "," << p.getCategory() << "," << p.getPrice() << "," << p.getQuantity() << endl;
        }
        file.close();
    }

    void loadInventoryFromFile(string filename)
    {
        ifstream file;
        file.open(filename);

        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string idStr, name, category, priceStr, quantityStr;
                getline(ss, idStr, ',');
                getline(ss, name, ',');
                getline(ss, category, ',');
                getline(ss, priceStr, ',');
                getline(ss, quantityStr, ',');

                int id = stoi(idStr);
                double price = stod(priceStr);
                int quantity = stoi(quantityStr);

                Product p(id, name, category, price, quantity);
                products.push_back(p);
            }

            file.close();
        }
        else
        {
            cout << "Error: Could not open file " << filename << endl;
        }
    }
};
User login()
{
    string username;
    string password;

    cout << "Enter your username: ";
    cin >> username;

    cout << "Enter your password: ";
#ifdef _WIN32
    password = "";
    char ch;
    while ((ch = _getch()) != 13) // 13 is the ASCII code for Enter
    {
        cout << '*';
        password += ch;
    }
#else
    password = "";
    char ch;
    while ((ch = getch()) != 10) // 10 is the ASCII code for Enter on Linux
    {
        cout << '*';
        password += ch;
    }
#endif

    cout << endl;

    return User(username, password);
}
// Function to validate user credentials
bool isValidUser(const User &user, const vector<User> &validUsers)
{
    for (const auto &validUser : validUsers)
    {
        if (user.getUsername() == validUser.getUsername() && user.getPassword() == validUser.getPassword())
        {
            return true;
        }
    }
    return false;
}

int main()
{
    // Define valid users
    vector<User> validUsers = {
        User("admin", "admin123"),
        User("user1", "pass123")};

    // Perform user login
    User currentUser = login();

    // Validate user credentials
    if (!isValidUser(currentUser, validUsers))
    {
        cout << "Invalid username or password. Exiting program." << endl;
        return 1;
    }

    Inventory inventory;

    cout << "-----------------------------------------------------------" << endl;
    cout << "---------------Inventory Management System ----------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "------------------------- Welcome! ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;

    char choice;
    do
    {
        cout << "Please choose an option:" << endl;
        cout << "1. Add a product" << endl;
        cout << "2. Remove a product" << endl;
        cout << "3. Find a product" << endl;
        cout << "4. Update a product" << endl;
        cout << "5. View all products" << endl;
        cout << "6. Save inventory to file" << endl;
        cout << "7. Load Inventory from file" << endl;
        cout << "Q. Quit" << endl;
        cin >> choice;

        switch (choice)
        {
        case '1':
        {
            int id;
            string name, category;
            double price;
            int quantity;
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter product name: ";
            cin >> name;
            cout << "Enter product category: ";
            cin >> category;
            cout << "Enter product price: $ ";
            cin >> price;
            cout << "Enter product quantity: ";
            cin >> quantity;
            Product product(id, name, category, price, quantity);
            inventory.addProduct(product);
            break;
        }

        case '2':
        {
            int id;
            cout << "Enter product id: ";
            cin >> id;
            inventory.removeProduct(id);
            break;
        }

        case '3':
        {
            int id;
            cout << "Enter product id: ";
            cin >> id;
            Product *product = inventory.findProduct(id);
            if (product)
            {
                cout << "Name: " << product->getName() << endl;
                cout << "Category: " << product->getCategory() << endl;
                cout << "Price: $ " << product->getPrice() << endl;
                cout << "Quantity: " << product->getQuantity() << endl;
                cout << "-----------------------------------------------------------" << endl;
            }
            else
            {
                cout << "Product not found." << endl;
                cout << "-----------------------------------------------------------" << endl;
            }
            break;
        }

        case '4':
        {
            int id;
            string name, category;
            double price;
            int quantity;
            cout << "Enter the product id: ";
            cin >> id;
            cout << "Enter new product name: ";
            cin >> name;
            cout << "Enter new product category: ";
            cin >> category;
            cout << "Enter new product price: $ ";
            cin >> price;
            cout << "Enter new product quantity: ";
            cin >> quantity;
            inventory.updateProduct(id, name, category, price, quantity);
            cout << "Product updated successfully." << endl;
            cout << "-----------------------------------------------------------" << endl;
            break;
        }

        case '5':
        {
            inventory.printProduct();
            break;
        }
        case '6':
        {
            inventory.saveInventoryToFile("inventory.csv");
            cout << "Inventory saved to file." << endl;
            cout << "-----------------------------------------------------------" << endl;
            break;
        }
        case '7':
        {
            inventory.loadInventoryFromFile("inventory.csv");
            cout << "Inventory loaded from file." << endl;
            cout << "-----------------------------------------------------------" << endl;
            break;
        }
        case 'Q':
        case 'q':
            cout << "Goodbye!" << endl;
            cout << "-----------------------------------------------------------" << endl;
            return 0;

        default:
            cout << "Invalid Choice. Please Try again" << endl;
            cout << "-----------------------------------------------------------" << endl;
            break;
        }
    } while (true);

    return 0;
}