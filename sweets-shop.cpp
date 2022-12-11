#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

class Product {
    private:
        const int id;
        char *name = NULL;
        double price;
        int promotions[7];
        static int instances;
    
    public:
        Product() :id(Product::instances++) { }

        Product(
            const char *name, 
            double price) : id(Product::instances++) {
            this->name = new char[strlen(name) + 1];
            if(this->name == NULL) {
                cout << "Memory allocation error on Product() constructor, this->name field." << endl;
                exit(1);
            }

            strcpy(this->name, name);
            this->price = price;
            for (int i = 0; i < 7; ++i) {
                promotions[i] = 0;
            }
        }

        Product(
            const char *name,
            double price,
            int promotions[]
        ) :id(Product::instances++) {
            this->name = new char[strlen(name) + 1];
            if(this->name == NULL) {
                cout << "Memory allocation error on Product() constructor, this->name field." << endl;
                exit(1);
            }

            strcpy(this->name, name);
            this->price = price;
            for(int i = 0; i < 7; ++i) {
                if(promotions[i] < 0 || promotions[i] > 100) {
                    cout << "The discount for a certain product must be between 0 and 100 inclusive." << endl;
                    exit(1);
                }
                
                this->promotions[i] = promotions[i];
            }
        }

        Product(const Product& product) :id(product.id) {
            this->setPrice(product.price);
            this->setName(product.name);
            this->setPromotions(product.promotions, 7);
        }

        Product& operator=(const Product& product) {
            this->setPrice(product.price);
            this->setName(product.name);
            this->setPromotions(product.promotions, 7);
            return *this;
        }

        int getId() {
            return id;
        }

        char *getName() {
            return name;
        }

        double getPrice() {
            return price;
        }

        int *getPromotions() {
            return this->promotions;
        }

        void setPrice(double price) {
            if(price <= 0) {
                cout << "The price of the product must be greater than 0." << endl;
                exit(1);
            } 

            this->price = price;
        }

        void setName(const char name[]) {
            if(strlen(name) == 0) {
                cout << "The name of the product is missing." << endl;
            }

            this->name = new char[strlen(name) + 1];
            if(this->name == NULL) {
                cout << "Memory allocation error on Product() constructor, this->name field." << endl;
                exit(1);
            }

            strcpy(this->name, name);
        }

        void setPromotions(const int promotions[], int length) {
            if(length != 7) {
                cout << "The size of promotions array is not equal to 7.";
                exit(1);
            }

            for(int i = 0; i < 7; ++i) {
                if(promotions[i] < 0 || promotions[i] > 100) {
                    cout << "The discount for a certain product must be between 0 and 100 inclusive." << endl;
                    exit(1);
                }
                
                this->promotions[i] = promotions[i];
            }
        }

        Product& operator+(double priceCoefficient) {
            this->price += priceCoefficient;
            return *this;
        }

        Product& operator+=(double priceCoefficient) {
            this->price += priceCoefficient;
            return *this;
        }

        int &operator[](int index) {
            if(index < 0 || index >= 7){
                cout << endl << "Invalid  day (Must be between 0 and 7).";
                exit(1);
            }
            return this->promotions[index];
        }

        Product& operator--(){
            this->price--;
            return *this;
        }
        
        Product operator--(int){
            Product prev = *this;
            --(*this);
            return prev;
        }

        operator double(){
            return price;
        }

        bool operator!(){
            for(int i = 0; i < 7; ++i) {
               if(promotions[i] != 0) {
                   return true;
               }
            }

            return false;
        }

        bool operator>(const Product& product){
            return this->price > product.price;
        }

        bool operator ==(const Product& product){
            return strcmp(this->name, product.name) == 0 && this->price == product.price;
        }

        bool operator!=(const Product& product) {
            return !(strcmp(this->name, product.name) == 0 && this->price == product.price);
        }

        double calculatePrice(int day) {
            int promotion = promotions[day];
            double discount = ((double)promotion / 100.0) * price;
            return price - discount;
        }

        bool hasDiscount() {
            int counter = 0;
            for (int i = 0; i < 7; ++i) {
                if (promotions[i] > 0) {
                    ++counter;
                }
            }

            return counter != 0;
        }

        friend ostream &operator<<(ostream& out, const Product& product) {
            if(product.name != NULL) {
                out << "The " << product.name << " product with ID=" << product.id << " has a price of " << product.price << "RON. The promotions for this product per day are: " << endl;
                out << "   MONDAY: " << product.promotions[0] << "%;" << endl;
                out << "  TUESDAY: " << product.promotions[1] << "%;" << endl;
                out << "WEDNESDAY: " << product.promotions[2] << "%;" << endl;
                out << " THURSDAY: " << product.promotions[3] << "%;" << endl;
                out << "   FRIDAY: " << product.promotions[4] << "%;" << endl;
                out << " SATURDAY: " << product.promotions[5] << "%;" << endl;
                out << "   SUNDAY: " << product.promotions[6] << "%." << endl;
            } else {
                cout << "The product was not completely initialized." << endl;
            }

            return out;
        }

        friend istream &operator>>(istream &in, Product& product) {
            delete[] product.name;
            char aux[100];
            cout << "Introduce the product name: ";
            in>>aux;
            product.name = new char[strlen(aux) + 1];
            strcpy(product.name, aux);

            cout << "Introduce the price for this product: ";
            in>>product.price;

            cout << "Introduce the promotions for this product per day."<< endl;
            cout << "   MONDAY:";
            in>>product.promotions[0];
            cout << "  TUESDAY:";
            in>>product.promotions[1];
            cout << "WEDNESDAY:";
            in>>product.promotions[2];
            cout << " THURSDAY:";
            in>>product.promotions[3];
            cout << "   FRIDAY:";
            in>>product.promotions[4];
            cout << " SATURDAY:";
            in>>product.promotions[5];
            cout << "   SUNDAY:";
            in>>product.promotions[6];
            cout << endl;

            return in;
        }

        ~Product() {
            delete[] name;
            --Product::instances;
          //  cout << "A product has been destroyed." << endl;
        }
};

int Product::instances = 0;

class Buyer {
    protected:
        char name[30];
        double budget;
        int numberOfProducts;
        int maxNumberOfProducts;
        // HAS-A
        Product *products = NULL;

        void performCopying(const Buyer& buyer) {
            strcpy(this->name, buyer.name);
            this->budget = buyer.budget;
            this->numberOfProducts = buyer.numberOfProducts;
            this->products = new Product[buyer.maxNumberOfProducts];
            if(this->products == NULL) {
                cout << "Memory allocation error. products was not properly initialized.";
                exit(1);
            }

            for(int i = 0; i < this->numberOfProducts; ++i) {
                this->products[i] = buyer.products[i];
            }
        }

        void removeLastProduct() {
            Product *temp = products;
            --numberOfProducts;
            products = new Product[numberOfProducts];
            if(products == NULL) {
                cout << "The resizing for products did not go well.";
                exit(1);
            }

            for(int i = 0; i < numberOfProducts; ++i) {
                products[i] = temp[i];
            }

            delete[] temp;
        }

    public:
        Buyer() {
            numberOfProducts = 0;
            maxNumberOfProducts = 10;
            products = new Product[maxNumberOfProducts];
            if(products == NULL) {
                cout << "Memory allocation error. products was not properly initialized.";
                exit(1);
            }
        }

        Buyer(const char name[]) {
            strcpy(this->name, name);
            budget = 0;
            numberOfProducts = 0;
            maxNumberOfProducts = 10;
            products = new Product[maxNumberOfProducts];
            if(products == NULL) {
                cout << "Memory allocation error. products was not properly initialized.";
                exit(1);
            }
        }

        Buyer(
            const char name[],
            double budget,
            int maxNumberOfProducts
        ) {
            strcpy(this->name, name);
            this->budget = budget;
            numberOfProducts = 0;
            this->maxNumberOfProducts = maxNumberOfProducts;
            products = new Product[maxNumberOfProducts];
            if(products == NULL) {
                cout << "Memory allocation error. products was not properly initialized.";
                exit(1);
            }
        }

        Buyer(const Buyer& buyer){
            performCopying(buyer);
        }

        Buyer& operator=(const Buyer& buyer){
            performCopying(buyer);
            return *this;
        }

        char *getName(){
            return this->name;
        }

        double getBudget(){
            return this->budget;
        }

        int getNumberOfProducts() {
            return this->numberOfProducts;
        }
        
        int getMaxNumberOfProducts() {
            return this->maxNumberOfProducts;
        }
    
        Product *getProducts(){
            return this->products;
        }

        void setBudget(double budget){
            if(budget < 0){
                cout << "The budget must be greater than zero." << endl;
                exit(1);
            }
            this->budget = budget;
        }
 
        virtual double calculateTotalPrice() {
            double sum = 0;
            for(int i = 0; i < numberOfProducts; ++i) {
                sum += (double)products[i];
            }

            return sum;
        }

        void addProduct(Product& product, int day) {
            if(products == NULL) {
                products = new Product[maxNumberOfProducts];
                if(products == NULL) {
                    cout << "Memory allocation error. products was not properly initialized.";
                    exit(1);
                }
            }

            double totalPrice = calculateTotalPrice();
            double newProductPrice = product.calculatePrice(day);
            if(totalPrice + newProductPrice > budget) {
                cout << "The product " << product.getName() << " is too expensive for your budget." << endl;
                return;
            }

            if(numberOfProducts == maxNumberOfProducts) {
                Product *temp = products;
                maxNumberOfProducts += 10;
                products = new Product[maxNumberOfProducts];
                for(int i = 0; i < numberOfProducts; ++i) {
                    products[i] = temp[i];
                }
                
                delete[] temp;
            }
            
            product.setPrice(newProductPrice);
            products[numberOfProducts] = product;
            ++numberOfProducts;
        }

        void pay() {
            double totalPrice = calculateTotalPrice();
            budget -= totalPrice;
            numberOfProducts = 0;
            delete[] products;
            products = NULL;
            cout << "The buyer " << name << " payed." << "The shopping took " << totalPrice << "RON. The buyer " << name << " has a budget of " 
                << budget << "RON left." << endl;
        }

        bool operator!() {
            numberOfProducts = 0;
            delete[] products;
            products = NULL;

            if(products != NULL) {
                cout << "The products could not be deleted." << endl;
                return 0;
            }

            cout << "Deleted all the products successfully." << endl;
            return 1;
        }   

        Buyer& operator+(double newBudget) {
            this->budget += newBudget;
            return *this;
        }

        Product operator[](int index) {
            return products[index];
        }

        bool operator==(Buyer& buyer){
            if(this->budget == buyer.budget)
                return true;
            else return false;
        }

         operator double (){
           return calculateTotalPrice();
        }

        bool operator <(const Buyer& buyer){
            return this->numberOfProducts < buyer.numberOfProducts;
        }

        void setName(const char name[]) {
            if (strlen(name) > 0) {
                strcpy(this->name, name);
            }
        }

        Buyer& operator--() {
            removeLastProduct();
            return *this;            
        }

        Buyer operator--(int) {
            Buyer buyer = *this;
            --(*this);
            return buyer;
        }

        friend ostream &operator<<(ostream& out, const Buyer& buyer) {
            out << "Buyer's name: " << buyer.name << endl;
            out << "Buyer's budget: " << buyer.budget << endl;
            out << "Number of products: " << buyer.numberOfProducts << endl;
            if(buyer.numberOfProducts != 0) {
                out << "The products are: " << endl;
            }

            for(int i = 0; i < buyer.numberOfProducts; ++i) {
                cout << "Product " << i + 1 << ": " << buyer.products[i].getName() << " with price " 
                    << (double)buyer.products[i] << "." << endl;
            }

            return out;
        }

        friend istream& operator>>(istream& in, Buyer& buyer) {
            cout << "Introduce the name of the buyer: ";
            fflush(stdin);
            gets(buyer.name);
            cout << "Introduce the budget: ";
            in >> buyer.budget;
            
            return in;            
        }

        ~Buyer() {
            delete[] products;
           // cout << "A buyer has been destroyed." << endl;
        }
};

class EmployedBuyer : public Buyer {
    private:
        int discountForShopping;
    
    public: 
        EmployedBuyer() : Buyer() {

        }

        EmployedBuyer(const char name[]) : Buyer(name) {

        }

        EmployedBuyer(
            const char name[],
            double budget,
            int maxNumberOfProducts,
            int discountForShopping
        ) : Buyer(name, budget, maxNumberOfProducts) {
            this->discountForShopping = discountForShopping;
        }

        EmployedBuyer(const EmployedBuyer& buyer) : Buyer(buyer) {
            discountForShopping = buyer.discountForShopping;
        }

        EmployedBuyer& operator=(const EmployedBuyer& employedBuyer) {
            this->performCopying(employedBuyer);
            discountForShopping = employedBuyer.discountForShopping;
            return *this;
        }

        EmployedBuyer& operator--() {
            this->removeLastProduct();
            return *this;            
        }

        EmployedBuyer operator--(int) {
            EmployedBuyer buyer = *this;
            if(numberOfProducts == 0) {
                return buyer;
            }

            --(*this);
            return buyer;
        }

        int getDiscountForShopping() {
            return discountForShopping;
        }

        // Overriding
        virtual double calculateTotalPrice() {
            double totalPriceWithoutDiscount = Buyer::calculateTotalPrice();
            return totalPriceWithoutDiscount * (1.00 - (double)discountForShopping / 100.0);
        }

        friend ostream &operator<<(ostream& out, const EmployedBuyer& employedBuyer) {
            out << "Employed buyer's name: " << employedBuyer.name << endl;
            out << "Employed buyer's budget: " << employedBuyer.budget << endl;
            out << "Employed buyer's discout for shopping: " << employedBuyer.discountForShopping << "%." << endl;
            out << "Number of products: " << employedBuyer.numberOfProducts << endl;
            if(employedBuyer.numberOfProducts != 0) {
                out << "The products are: " << endl;
            }
            
            for(int i = 0; i < employedBuyer.numberOfProducts; ++i) {
                cout << "Product " << i + 1 << ": " << employedBuyer.products[i].getName() << " with price " 
                    << (double)employedBuyer.products[i] << "." << endl;
            }

            return out;
        }

        friend istream& operator>>(istream& in, EmployedBuyer& employedBuyer) {
            cout << "Introduce the name of the employed buyer: ";
            fflush(stdin);
            gets(employedBuyer.name);
            cout << "Introduce the budget: ";
            in >> employedBuyer.budget;
            cout << "Introduce the discount for shopping: ";
            in >> employedBuyer.discountForShopping;
            
            return in;            
        }
};

class CashRegister {
    private:
        char name[30];
        bool isActive;
        int cashRegisterCapacity;
        int currentServedBuyers;
        Buyer *buyers;

        void performCopying(const CashRegister& cashRegister) {
            strcpy(name, cashRegister.name);
            isActive = cashRegister.isActive;
            currentServedBuyers = cashRegister.currentServedBuyers;
            cashRegisterCapacity = cashRegister.cashRegisterCapacity;
            buyers = new Buyer[cashRegisterCapacity];
            if (buyers == NULL) {
                cout << "Memory allocation error. buyers was not properly initialized." << endl;
                exit(1);
            }

            for (int i = 0; i < currentServedBuyers; ++i) {
                buyers[i] = cashRegister.buyers[i];
            }
        }

    public:
        CashRegister() {   
            currentServedBuyers = 0;
            cashRegisterCapacity = 5;
            buyers = new Buyer[cashRegisterCapacity];
            if (buyers == NULL) {
                cout << "Memory allocation error. buyers was not properly initialized." << endl;
                exit(1);
            }
        }

        CashRegister(const char name[]) {
            strcpy(this->name, name);
            isActive = 1;
            currentServedBuyers = 0;
            cashRegisterCapacity = 5;
            buyers = new Buyer[cashRegisterCapacity];
            if (buyers == NULL) {
                cout << "Memory allocation error. buyers was not properly initialized." << endl;
                exit(1);
            }
        }

        CashRegister(
            const char name[],
            int cashRegisterCapacity
        ) {
            strcpy(this->name, name);
            isActive = 1;
            currentServedBuyers = 0;
            this->cashRegisterCapacity = cashRegisterCapacity;
            buyers = new Buyer[cashRegisterCapacity];
            if (buyers == NULL) {
                cout << "Memory allocation error. buyers was not properly initialized." << endl;
                exit(1);
            }
        }

        CashRegister(const CashRegister& cashRegister) {
            performCopying(cashRegister);
        }

        CashRegister& operator=(const CashRegister& cashRegister) {
            performCopying(cashRegister);
            return *this;
        }

        void setIsActive(bool isActive) {
            if (currentServedBuyers == 0) {
                this->isActive = isActive;
            }
        }

        void setCashRegisterCapacity(int cashRegisterCapacity) {
            if(cashRegisterCapacity < this->cashRegisterCapacity) {
                if (currentServedBuyers == 0) {
                    this->cashRegisterCapacity = cashRegisterCapacity;
                    buyers = new Buyer[cashRegisterCapacity];
                    if (buyers == NULL) {
                        cout << "Memory allocation error. buyers was not properly initialized." << endl;
                        exit(1);
                    }
                }
            } else {
                Buyer *temp = buyers;
                buyers = new Buyer[cashRegisterCapacity];
                this->cashRegisterCapacity = cashRegisterCapacity;
                if (buyers == NULL) {
                    cout << "Memory allocation error. buyers was not properly initialized." << endl;
                    exit(1);
                }

                for (int i = 0; i < currentServedBuyers; ++i) {
                    buyers[i] = temp[i];
                }

                delete[] temp;
            }
        }
      
        void setName(const char name[]) {
            if (strlen(name) > 0) {
                strcpy(this->name, name);
            }
        }

        void addBuyer(Buyer& buyer) {
            if (currentServedBuyers == cashRegisterCapacity) {
                cout << "No more available places.";
                exit(1);
            }

            buyers[currentServedBuyers] = buyer;
            ++currentServedBuyers;
        }

        void removeBuyer() {
            if (currentServedBuyers == 0) {
                return;
            }
            cout << buyers[0] << endl;
            buyers[0].pay();
            for (int i = 1; i < currentServedBuyers; ++i) {
                buyers[i - 1] = buyers[i];
            }
            
            --currentServedBuyers;
        }

        int getCurrentServedBuyers() {
            return currentServedBuyers;
        }

        operator int(){
            return currentServedBuyers;
        }
        
        Buyer operator[](int index){
            if (index >= currentServedBuyers) {
                cout << "Index is out of bound." << endl;
                exit(1);
            }

            return buyers[index];
        }

        bool operator==(CashRegister& cashRegister){
            if (cashRegisterCapacity == cashRegister.cashRegisterCapacity)
                return true;
            else return false;
        }

        bool operator>(const CashRegister& cashRegister){
            return (strcmp(name, cashRegister.name) > 0);
        }

        bool operator!(){

            if (currentServedBuyers == 0) {
                return 0;
            }
            
            currentServedBuyers = 0;
            delete[] buyers;
            buyers = NULL;
            
            if(buyers != NULL){
                cout << "The buyers could not be deleated." <<endl;
                return 0; 
            }
          
            cout << "The buyers deleted."<<endl;
            return 1;

        }

        CashRegister& operator+(int capacity){
            this->cashRegisterCapacity += capacity;
            return *this;
        }

        CashRegister& operator+=(int capacity){
            this->cashRegisterCapacity += capacity;
            return *this;
        }
        
        CashRegister& operator--(){
            removeBuyer();
            return *this;
        }

        CashRegister operator--(int){
            CashRegister cashRegister = *this;
            --(*this);
            return cashRegister;
        }

        char *getName() {
            return name;
        }

        bool getIsActive() {
            return isActive;
        }

        int getCashRegisterCapacity() {
            return cashRegisterCapacity;
        }

        friend ostream &operator<<(ostream& out, const CashRegister& cashRegister) {
            if (cashRegister.isActive) {
                out << "The " << cashRegister.name << " is active and has a capacity of " << cashRegister.cashRegisterCapacity << " buyers." 
                    << " Currently, there are " << cashRegister.currentServedBuyers << " buyers being served." << endl;
            } else {
                out << "The " << cashRegister.name << " is inactive and has a capacity of " << cashRegister.cashRegisterCapacity << " buyers." 
                    << " Currently, there are " << cashRegister.currentServedBuyers << " buyers being served." << endl;
            }

            for (int i = 0; i < cashRegister.currentServedBuyers; ++i) {
                cout << cashRegister.buyers[i] << endl;
            }

            return out;
        }

         friend istream& operator>>(istream& in, CashRegister& cashRegister) {
            cout << "Is the cash register active? ";
            in >> cashRegister.isActive;
            cout << "Introduce the name of the cash register: ";
            fflush(stdin);
            gets(cashRegister.name);
            cout << "Introduce the capacity of the cash register: ";
            in >> cashRegister.cashRegisterCapacity; 
            
            return in;            
        }

        ~CashRegister() {
            delete[] buyers;
        }
};

class Provider {
    private:
        char name[30];
        Product *products = NULL;
        int numberOfProducts;
        int capacity;

        void performCopying(const Provider& provider) {
            strcpy(this->name, provider.name);
            this->capacity = provider.capacity;
            this->numberOfProducts = provider.numberOfProducts;
            products = new Product[this->capacity];
            if (products == NULL) {
                cout << "Memory allocation error. products in Provider class was not properly allocated.";
                exit(1);
            }

            for (int i = 0; i < this->numberOfProducts; ++i) {
                this->products[i] = provider.products[i];
            }
        }

        void resizeProducts(int capacity) {
            Product *temp = products;
            products = new Product[capacity];
            if (products == NULL) {
                cout << "Memory allocation error. products in Provider class was not properly allocated.";
                exit(1);
            }

            for (int i = 0; i < numberOfProducts; ++i) {
                products[i] = temp[i];
            }

            this->capacity = capacity;
        }
    
    public:
        Provider() {
            capacity = 10;
            products = new Product[capacity];
            numberOfProducts = 0;
            if (products == NULL) {
                cout << "Memory allocation error. products in Provider class was not properly allocated.";
                exit(1);
            }
        }

        Provider(
            const char name[]
        ) {
            strcpy(this->name, name);
            capacity = 10;
            products = new Product[capacity];
            numberOfProducts = 0;
            if (products == NULL) {
                cout << "Memory allocation error. products in Provider class was not properly allocated.";
                exit(1);
            }
        }

        Provider (
            const char name[],
            int capacity
        ) {
            strcpy(this->name, name);
            this->capacity = capacity;
            numberOfProducts = 0;
            products = new Product[this->capacity];
            if (products == NULL) {
                cout << "Memory allocation error. products in Provider class was not properly allocated.";
                exit(1);
            }
        }

        Provider(
            const Provider& provider
        ) {
            performCopying(provider);
        }

        Provider& operator=(const Provider& provider) {
            performCopying(provider);
            return *this;
        }

        Product& createProduct(
            const char name[],
            double price
        ) {
            for (int i = 0; i < numberOfProducts; ++i) {
                if (strcmp(products[i].getName(), name) == 0 && products[i].getPrice() == price) {
                    cout << "This product already exists." << endl;
                    return products[i];
                }
            }

            Product product(name, price);
            if(numberOfProducts == capacity) {
                resizeProducts(capacity * 2);
            }

            products[numberOfProducts] = product;
            ++numberOfProducts;

            return products[numberOfProducts - 1];
        }

        int getNumberOfProducts() {
            return numberOfProducts;
        }

        Provider& operator++(){
            resizeProducts(capacity * 2);
            return *this;
        }

        Provider operator++(int){
            Provider provider = *this;
            ++(*this);
            return provider;
        }

        bool operator==(const Provider& provider) {
            if (strcmp(provider.name, this->name) != 0 || provider.numberOfProducts != this->numberOfProducts) {
                return false;
            }

            for (int i = 0; i < this->numberOfProducts; ++i) {
                if (this->products[i] != provider.products[i]) {
                    return false;
                }
            }

            return true;
        }

        void setName(const char name[]) {
            if (strlen(name) == 0) {
                cout << "You have given an empty name for Provider." << endl;
                exit(1);
            }

            strcpy(this->name, name);
        }

        void setCapacity(int capacity) {
            resizeProducts(capacity);
        }

        char *getName() {
            return this->name;
        }

        int getCapacity() {
            return this->capacity;
        }

        bool operator>(const Provider& provider) {
            return this->numberOfProducts > provider.numberOfProducts;
        }

        bool operator<(const Provider& provider) {
            return this->numberOfProducts < provider.numberOfProducts;
        }

        Product *getProducts() {
            return this->products;
        }

        Product operator[](int index) {
            if (index >= numberOfProducts) {
                cout << "Index is out of bound." << endl;
                exit(1);
            }

            return products[index];
        }

         Provider& operator+(int cap){
            this->capacity += capacity;
            return *this;
        }
        operator int() {
            return capacity;
        }

        void operator!() {
            delete[] products;
            products = NULL;
            capacity = 10;
            products = new Product[capacity];
            numberOfProducts = 0;
            if (products == NULL) {
                cout << "Memory allocation error. products in Provider class was not properly allocated.";
                exit(1);
            }
        }

        void removeProduct(Product product) {
            for (int i = 0; i < numberOfProducts; ++i) {
                if (products[i] == product) {
                    for (int j = i; j < numberOfProducts - 1; ++j) {
                        products[i] = products[i + 1];
                    }
                    --numberOfProducts;
                }
            }
        }

        friend ostream &operator<<(ostream& out, const Provider& provider) {
            out << "Provider's name: " << provider.name << endl;
            out << "Provider's products: " << provider.numberOfProducts << endl;
            out << "Provider's capacity: " << provider.capacity << endl;

            for (int i = 0; i < provider.numberOfProducts; ++i) {
                out << provider.products[i].getName() << " " << provider.products[i].getPrice() << endl;
            }

            return out;
        }

        friend istream& operator>>(istream& in, Provider& provider) {
            cout << "Enter the provider's name: ";
            fflush(stdin);
            gets(provider.name);
            cout << "Introduce the provider's capacity: ";
            in >> provider.capacity;
            provider.resizeProducts(provider.capacity);
            
            return in;            
        }

        ~Provider() {
            delete[] products;
        }
};

class Shop {
    private:
        char name[30];
        int numberOfCashRegisters;
        int maxNumberOfCashRegisters;
        CashRegister *cashRegisters = NULL;
        int numberOfProducts;
        int maxNumberOfProducts;
        Product *products = NULL;
        int numberOfBuyers;
        int maxNumberOfBuyers;
        Buyer *buyers = NULL;
        static int totalNumberOfBuyers;

    public:
        Shop() {
            numberOfCashRegisters = 0;
            numberOfProducts = 0;
            numberOfBuyers = 0;
        }

        Shop(
            const char name[],
            int maxNumberOfCashRegisters, 
            int maxNumberOfProducts,
            int maxNumberOfBuyers
        ) 
        {
            strcpy(this->name, name);
            numberOfCashRegisters = 0;
            numberOfProducts = 0;
            numberOfBuyers = 0;
            this->maxNumberOfBuyers = maxNumberOfBuyers;
            this->maxNumberOfProducts = maxNumberOfProducts;
            this->maxNumberOfCashRegisters = maxNumberOfCashRegisters;
            cashRegisters = new CashRegister[maxNumberOfCashRegisters];
            products = new Product[maxNumberOfProducts];
            buyers = new Buyer[maxNumberOfBuyers];
            if (cashRegisters == NULL) {
                cout << "cashRegisters in Shop class could not be instantiated.";
                exit(1);
            }

            if (products == NULL) {
                cout << "products in Shop class could not be instantiated.";
                exit(1);
            }

            if (buyers == NULL) {
                cout << "buyers in Shop class could not be instantiated.";
                exit(1);
            }
        }

        Shop& operator=(const Shop& shop) {
            strcpy(this->name, shop.name);
            this->maxNumberOfBuyers = shop.maxNumberOfBuyers;
            this->maxNumberOfProducts = shop.maxNumberOfProducts;
            this->maxNumberOfCashRegisters = shop.maxNumberOfCashRegisters;
            this->cashRegisters = new CashRegister[this->maxNumberOfCashRegisters];
            this->buyers = new Buyer[this->maxNumberOfBuyers];
            this->products = new Product[this->maxNumberOfProducts];
            if (cashRegisters == NULL) {
                cout << "cashRegisters in Shop class could not be instantiated.";
                exit(1);
            }

            if (products == NULL) {
                cout << "products in Shop class could not be instantiated.";
                exit(1);
            }

            if (buyers == NULL) {
                cout << "buyers in Shop class could not be instantiated.";
                exit(1);
            }

            for (int i = 0; i < shop.numberOfCashRegisters; ++i) {
                this->addCashRegister(shop.cashRegisters[i]);
            }

            for(int i = 0; i < shop.numberOfBuyers; ++i) {
                this->addBuyer(shop.buyers[i]);
            }

            for(int i = 0; i < shop.numberOfProducts; ++i) {
                this->addProduct(shop.products[i]);
            }

            return *this;
        }

        void changeCashRegisterActivity(
            int indexOfCashRegister,
            bool activity
        ) {
            for (int i = 0; i < numberOfCashRegisters; ++i) {
                if (i == indexOfCashRegister) {
                    cashRegisters[i].setIsActive(activity);
                    break;
                }
            }
        }

        void setPromotionsToProduct(
            int promotions[],
            int length,
            const char name[]
        ) {
            for (int i = 0; i < numberOfProducts; ++i) {
                if (strcmp(products[i].getName(), name) == 0) {
                    products[i].setPromotions(promotions, length);
                    break;
                }
            }
        }

        void goToCashRegister(Buyer& buyer) {
            int minCapacity = cashRegisters[0].getCurrentServedBuyers();
            int minIndex = 0;
            cout << numberOfCashRegisters << endl;
            int k = 0;
            for (int i = 0; i < numberOfBuyers; ++i) {
                if (strcmp(buyers[i].getName(), buyer.getName()) == 0) {
                    k = i;
                    break;
                }
            }

            for (int i = 1; i < numberOfCashRegisters; ++i) {
                if (cashRegisters[i].getIsActive() == 1 && cashRegisters[i].getCurrentServedBuyers() < cashRegisters[i].getCashRegisterCapacity()) {
                    if (cashRegisters[i].getCurrentServedBuyers() < minCapacity) {
                        minCapacity = cashRegisters[i].getCurrentServedBuyers();
                        minIndex = i;
                    }
                }
            }

            if (minIndex == 0) {
                if (cashRegisters[minIndex].getCashRegisterCapacity() == minCapacity) {
                    cout << "We are sorry, we can not serve you at this moment. Please, wait." << endl;
                    return;
                } else {
                    cashRegisters[minIndex].addBuyer(buyers[k]);
                }
            } else {
                cashRegisters[minIndex].addBuyer(buyers[k]);
            }
        }

        void pay(
            int cashRegisterIndex
        ) {
            for (int i = 0; i < numberOfCashRegisters; ++i) {
                if (i == cashRegisterIndex) {
                    cashRegisters[i].removeBuyer();
                    
                }
            }
        }

        void placeProduct(
            const char name[],
            Product& product,
            int day
        ) {
            for (int i = 0; i < numberOfBuyers; ++i) {
                if (strcmp(buyers[i].getName(), name) == 0) {
                    buyers[i].addProduct(product, day);
                    break;
                }
            }
        }
        
        void addBuyer(const Buyer& buyer) {
            if (numberOfBuyers == maxNumberOfBuyers) {
                return;
            }

            Shop::totalNumberOfBuyers++;
            buyers[numberOfBuyers] = buyer;
            ++numberOfBuyers;
        }

        void addCashRegister(const CashRegister& cashRegister) {
            if (numberOfCashRegisters == maxNumberOfCashRegisters) {
                return;
            }

            cashRegisters[numberOfCashRegisters] = cashRegister;
            ++numberOfCashRegisters;
            
        }

        char *getName() {
            return name;
        }

        int getNumberOfBuyers() {
            return numberOfBuyers;
        }

        int getMaxNumberOfBuyers() {
            return maxNumberOfBuyers;
        }

        int getNumberOfCashRegisters() {
            return numberOfCashRegisters;
        }

        int getMaxNumberOfCashRegisters() {
            return maxNumberOfCashRegisters;
        }

        int getNumberOfProducts() {
            return numberOfProducts;
        }

        int getMaxNumberOfProducts() {
            return maxNumberOfProducts;
        }

        Product *getProducts() {
            return products;
        }

        Buyer *getBuyers() {
            return buyers;
        }

        CashRegister *getCashRegisters() {
            return cashRegisters;
        }

        void addProduct(const Product& product) {
            if (numberOfProducts == maxNumberOfProducts) {
                return;
            }

            products[numberOfProducts] = product;
            ++numberOfProducts;
        }

        static int getTotalNumberOfBuyers() {
            return Shop::totalNumberOfBuyers;
        }

        friend ostream &operator<<(ostream& out, const Shop& shop) {
            out << "Shop's name: " << shop.name << endl;
            out << "Number of cash registers: " << shop.numberOfCashRegisters << endl;
            for (int i = 0; i < shop.numberOfCashRegisters; ++i) {
                out << shop.cashRegisters[i].getName();
                out << " is serving " << shop.cashRegisters[i].getCurrentServedBuyers() << " buyers." << endl;
            }
            out << "Number of products: " << shop.numberOfProducts << ". The products are: " << endl;
            for (int i = 0; i < shop.numberOfProducts; ++i) {
                out << shop.products[i].getName() << endl;
            }
            out << "Number of buyers: " << shop.numberOfBuyers << endl;
            for (int i = 0; i < shop.numberOfBuyers; ++i) {
                out << shop.buyers[i].getName();
                out << " has " << shop.buyers[i].getNumberOfProducts() << " products." << endl;
            }

            return out;
        }
        
         friend istream& operator>>(istream& in, Shop& shop) {
            cout << "Enter the shop's name: ";
            fflush(stdin);
            gets(shop.name);
            cout <<"Introduce the maximum nr of cash registers: ";
            in >> shop.maxNumberOfCashRegisters;
            cout <<"Introduce the maximum nr of products: ";
            in >> shop.maxNumberOfProducts;
            cout <<"Introduce the maximum nr of buyers: ";
            in >> shop.maxNumberOfBuyers;

            shop.cashRegisters = new CashRegister[shop.maxNumberOfCashRegisters];
            shop.products = new Product[shop.maxNumberOfProducts];
            shop.buyers = new Buyer[shop.maxNumberOfBuyers];
            if (shop.cashRegisters == NULL) {
                cout << "cashRegisters in Shop class could not be instantiated.";
                exit(1);
            }

            if (shop.products == NULL) {
                cout << "products in Shop class could not be instantiated.";
                exit(1);
            }

            if (shop.buyers == NULL) {
                cout << "buyers in Shop class could not be instantiated.";
                exit(1);
            }
               
            return in;            
        }

        ~Shop() {
            delete[] cashRegisters;
            delete[] products;
            delete[] buyers;
        }
};

int Shop::totalNumberOfBuyers = 0;

class TextFileProcessor {
    
    public:
        TextFileProcessor() {}

        void readProductFromFile(
            ifstream& file,
            Product products[],
            int *numberOfProducts
        ) {
            string line;
            getline(file, line);
            while (getline(file, line)) {
                // Stream - înfășurător pentru String
                stringstream lineStream(line);
                // Variabile unde vor fi stocate câmpurile separate prin virgulă
                // Funcția getline va citi din stream-ul declarat precedent
                // Va stoca în variabile item ceea ce citește
                // Va citi până găsește o virgulă, va stoca în item ce a citit
                // După care continuă citirea până citește absolut totul din stream
                string items[9];
                for (int i = 0; i < 9; ++i) {
                    getline(lineStream, items[i], ',');
                }

                int promo[] = {
                    stoi(items[2]),
                    stoi(items[3]),
                    stoi(items[4]),
                    stoi(items[5]),
                    stoi(items[6]),
                    stoi(items[7]),
                    stoi(items[8])
                };
                char productName[items[0].length() + 1];
                strcpy(productName, items[0].c_str());
                Product product(productName, stod(items[1]), promo);
                products[*numberOfProducts] = product;
                (*numberOfProducts)++;
            }
        }

        void readBuyerFromFile(
            ifstream& file,
            Buyer buyers[],
            int *numberOfBuyers
        ) {
            string line;
            getline(file, line);
            while (getline(file, line)) {
                stringstream lineStream(line);
                string items[4];
                for (int i = 0; i < 4; ++i) {
                    getline(lineStream, items[i], ',');
                }

                if (stoi(items[3]) < 0) {
                    char buyerName[items[0].length() + 1];
                    strcpy(buyerName, items[0].c_str());
                    double budget = stod(items[1]);
                    int numberOfProducts = stoi(items[2]);
                    Buyer buyer(buyerName, budget, numberOfProducts);
                    buyers[*numberOfBuyers] = buyer;
                    (*numberOfBuyers)++;
                }
            }
        }

        void readEmployedBuyerFromFile(
            ifstream& file,
            EmployedBuyer buyers[],
            int *numberOfBuyers
        ) {
            string line;
            getline(file, line);
            while (getline(file, line)) {
                stringstream lineStream(line);
                string items[4];
                for (int i = 0; i < 4; ++i) {
                    getline(lineStream, items[i], ',');
                }

                double discount = stoi(items[3]);
                if (discount >= 0) {
                    char buyerName[items[0].length() + 1];
                    strcpy(buyerName, items[0].c_str());
                    double budget = stod(items[1]);
                    int numberOfProducts = stoi(items[2]);
                    EmployedBuyer employedBuyer(buyerName, budget, numberOfProducts, discount);
                    buyers[*numberOfBuyers] = employedBuyer;
                    (*numberOfBuyers)++;
                }
            }
        }

        void readProviderFromFile(
            ifstream& file,
            Provider providers[],
            int *numberOfProviders
        ) {
            string line;
            getline(file, line);
            while (getline(file, line)) {
                stringstream lineStream(line);
                string items[2];
                for (int i = 0; i < 2; ++i) {
                    getline(lineStream, items[i], ',');
                }

                char providerName[items[0].length() + 1];
                strcpy(providerName, items[0].c_str());
                int capacity = stoi(items[1]);
                Provider provider(providerName, capacity);
                providers[*numberOfProviders] = provider;
                (*numberOfProviders)++;
            }
        }

        void readCashRegisterFromFile(
            ifstream& file,
            CashRegister cashRegisters[],
            int *numberOfCashRegisters
        ) {
            string line;
            getline(file, line);
            while (getline(file, line)) {
                stringstream lineStream(line);
                string items[2];
                for (int i = 0; i < 2; ++i) {
                    getline(lineStream, items[i], ',');
                }

                char cashRegisterName[items[0].length() + 1];
                strcpy(cashRegisterName, items[0].c_str());
                int capacity = stoi(items[1]);
                CashRegister cashRegister(cashRegisterName, capacity);
                cashRegisters[*numberOfCashRegisters] = cashRegister;
                (*numberOfCashRegisters)++;
            }
        }
};

class BinaryFileWriter {
    public:
        virtual void writeToFile(ofstream& binaryFile, ifstream& inputFile) = 0;
};

class ProviderBinaryFileWriter : public BinaryFileWriter {
    public:
        virtual void writeToFile(ofstream& binaryFile, ifstream& inputFile) {
            TextFileProcessor textFileProcessor;
            int numberOfProviders = 0;
            Provider providers[100];
            textFileProcessor.readProviderFromFile(inputFile, providers, &numberOfProviders);
            for (int i = 0; i < numberOfProviders; ++i) {
                binaryFile.write(providers[i].getName(), sizeof(char) * strlen(providers[i].getName()));
                int x = providers[i].getCapacity();
                binaryFile.write((char*)&x, sizeof(int));
            }
        }
};

class ProductBinaryFileWriter : public BinaryFileWriter {
    public:
        virtual void writeToFile(ofstream& binaryFile, ifstream& inputFile) {
            TextFileProcessor textFileProcessor;
            int numberOfProducts = 0;
            Product products[100];
            textFileProcessor.readProductFromFile(inputFile, products, &numberOfProducts);
            for (int i = 0; i < numberOfProducts; ++i) {
                binaryFile.write(products[i].getName(), sizeof(char) * strlen(products[i].getName()));
                double x = products[i].getPrice();
                binaryFile.write((char*)&x, sizeof(double));
                binaryFile.write((char *)products[i].getPromotions(), sizeof(int) * 7);
            }
        }
};

class BuyerBinaryFileWriter : public BinaryFileWriter {
    public:
        virtual void writeToFile(ofstream& binaryFile, ifstream& inputFile) {
            TextFileProcessor textFileProcessor;
            int numberOfBuyers = 0;
            Buyer buyers[100];
            textFileProcessor.readBuyerFromFile(inputFile, buyers, &numberOfBuyers);
            for (int i = 0; i < numberOfBuyers; ++i) {
                binaryFile.write(buyers[i].getName(), sizeof(char) * strlen(buyers[i].getName()));
                double x = buyers[i].getBudget();
                binaryFile.write((char*)&x, sizeof(double));
                int y = buyers[i].getMaxNumberOfProducts();
                binaryFile.write((char *)&y, sizeof(int));
            }
        }
};

class EmployedBuyerBinaryFileWriter : public BinaryFileWriter {
    public:
        virtual void writeToFile(ofstream& binaryFile, ifstream& inputFile) {
            TextFileProcessor textFileProcessor;
            int numberOfEmployedBuyers = 0;
            EmployedBuyer employedBuyers[100];
            textFileProcessor.readEmployedBuyerFromFile(inputFile, employedBuyers, &numberOfEmployedBuyers);
            for (int i = 0; i < numberOfEmployedBuyers; ++i) {
                binaryFile.write(employedBuyers[i].getName(), sizeof(char) * strlen(employedBuyers[i].getName()));
                double x = employedBuyers[i].getBudget();
                binaryFile.write((char*)&x, sizeof(double));
                int y = employedBuyers[i].getMaxNumberOfProducts();
                binaryFile.write((char *)&y, sizeof(int));
                int z = employedBuyers[i].getDiscountForShopping();
                binaryFile.write((char *)&z, sizeof(int));
            }
        }
};

class CashRegisterBinaryFileWriter : public BinaryFileWriter {
    public:
        virtual void writeToFile(ofstream& binaryFile, ifstream& inputFile) {
            TextFileProcessor textFileProcessor;
            int numberOfCashRegisters = 0;
            CashRegister cashRegisters[100];
            textFileProcessor.readCashRegisterFromFile(inputFile, cashRegisters, &numberOfCashRegisters);
            for (int i = 0; i < numberOfCashRegisters; ++i) {
                binaryFile.write(cashRegisters[i].getName(), sizeof(char) * strlen(cashRegisters[i].getName()));
                int x = cashRegisters[i].getCashRegisterCapacity();
                binaryFile.write((char*)&x, sizeof(int));
                int y = cashRegisters[i].getIsActive();
                binaryFile.write((char*)&y, sizeof(int));
            }
        }
};

class ReportGenerator {
    public:
        virtual void generateReport(ofstream& file) = 0;
};

class ShopReportGenerator : public ReportGenerator{
    private:
        Shop shop;
    
    public:
        ShopReportGenerator(Shop shop) {
            this->shop = shop;
        }

        void setShop(Shop shop) {
            this->shop = shop;
        }

        virtual void generateReport(ofstream& file) {
            Product *products = shop.getProducts();
            Buyer *buyers = shop.getBuyers();
            CashRegister *cashRegisters = shop.getCashRegisters();

            cout << shop << endl;

            file << "Report for shop " << shop.getName() << endl;
            file << "Number of cash registers: " << shop.getNumberOfCashRegisters() << endl;
            for (int i = 0; i < shop.getNumberOfCashRegisters(); ++i) {
                file << cashRegisters[i].getName();
                file << " is serving " << cashRegisters[i].getCurrentServedBuyers() << " buyers." << endl;
            }
            file << "Number of products: " << shop.getNumberOfProducts() << ". The products are: " << endl;
            for (int i = 0; i < shop.getNumberOfProducts(); ++i) {
                file << products[i].getName() << endl;
            }
            file << "Number of buyers: " << shop.getNumberOfBuyers() << endl;
            for (int i = 0; i < shop.getNumberOfBuyers(); ++i) {
                file << buyers[i].getName();
                file << " has " << buyers[i].getNumberOfProducts() << " products." << endl;
            }
        }
};

class ProviderReportGenerator : public ReportGenerator {
    private:
        Provider provider;
    
    public:
        ProviderReportGenerator(Provider provider) {
            this->provider = provider;
        }

        virtual void generateReport(ofstream& file)  {
            Product *products = provider.getProducts();

            cout << provider << endl;

            file << "Report for provider " << provider.getName() << endl;
            file << "Number of products: " << provider.getNumberOfProducts() << ". The products are: " << endl;
            for (int i = 0; i < provider.getNumberOfProducts(); ++i) {
                file << products[i].getName() << " with price equal to " << products[i].getPrice() << "RON." << endl;
            }
        }
};

class ProductReportGenerator : public ReportGenerator {
    private:
        vector<Product> products;
    
    public:
        ProductReportGenerator(Product products[], int numberOfProducts) {
            for (int i = 0; i < numberOfProducts; ++i) {
                // push_back() - adauga un element la sfarsitul listei
                // Daca avem vector cu elementele p1 - p2 - p3, atunci, dupa ce este apelat push_back(p4),
                // vom avem p1 - p2 - p3 - p4
                this->products.push_back(products[i]);
            }
        }

        void generateReport(ofstream& file) {
            file << "Report for all the products read from .csv files." << endl;
            file << "The products are: " << endl;
            for (int i = 0; i < products.size(); ++i) {
                cout << products[i] << endl;
                file << products[i].getName() << " has a price of " << products[i].getPrice() << "RON." << endl;
                int *promotions = products[i].getPromotions();
                file << "The promotions for this product are as follows: " << endl;
                file << "   MONDAY: " << promotions[0] << "%;" << endl;
                file << "  TUESDAY: " << promotions[1] << "%;" << endl;
                file << "WEDNESDAY: " << promotions[2] << "%;" << endl;
                file << " THURSDAY: " << promotions[3] << "%;" << endl;
                file << "   FRIDAY: " << promotions[4] << "%;" << endl;
                file << " SATURDAY: " << promotions[5] << "%;" << endl;
                file << "   SUNDAY: " << promotions[6] << "%." << endl;
                file << endl;
            }
        }
};

int main(int argc, char** argv) {
    // argv[1] - the file containing the Product attributes
    ifstream productFile(argv[1]);
    ifstream buyerFile(argv[2]);
    ifstream employedBuyerFile(argv[2]);
    ifstream providerFile(argv[3]);
    ifstream cashRegisterFile(argv[4]);
    Product products[100];
    Buyer buyers[100];
    EmployedBuyer employedBuyers[100];
    Provider providers[100];
    CashRegister cashRegisters[100];
    int numberOfProducts = 0;
    int numberOfBuyers = 0;
    int numberOfEmployedBuyers = 0;
    int numberOfProviders = 0;
    int numberOfCashRegisters = 0;
    TextFileProcessor textFileProcessor;
    textFileProcessor.readProductFromFile(productFile, products, &numberOfProducts);
    textFileProcessor.readBuyerFromFile(buyerFile, buyers, &numberOfBuyers);
    textFileProcessor.readEmployedBuyerFromFile(employedBuyerFile, employedBuyers, &numberOfEmployedBuyers);
    textFileProcessor.readProviderFromFile(providerFile, providers, &numberOfProviders);
    textFileProcessor.readCashRegisterFromFile(cashRegisterFile, cashRegisters, &numberOfCashRegisters);
    ofstream binaryFile;
    binaryFile.open("data.dat", ios::binary | ios::out);
    productFile.clear();
    productFile.seekg(0);
    buyerFile.clear();
    buyerFile.seekg(0);
    employedBuyerFile.clear();
    employedBuyerFile.seekg(0);
    providerFile.clear();
    providerFile.seekg(0);
    cashRegisterFile.clear();
    cashRegisterFile.seekg(0);
    BinaryFileWriter *providerBinaryFileWriter = new ProviderBinaryFileWriter;
    BinaryFileWriter *productsBinaryFileWriter = new ProductBinaryFileWriter;
    BinaryFileWriter *buyerBinaryFileWriter = new BuyerBinaryFileWriter;
    BinaryFileWriter *employedBuyerBinaryFileWriter = new EmployedBuyerBinaryFileWriter;
    BinaryFileWriter *cashRegisterBinaryFileWriter = new CashRegisterBinaryFileWriter;
    providerBinaryFileWriter->writeToFile(binaryFile, providerFile);
    productsBinaryFileWriter->writeToFile(binaryFile, productFile);
    buyerBinaryFileWriter->writeToFile(binaryFile, buyerFile);
    employedBuyerBinaryFileWriter->writeToFile(binaryFile, employedBuyerFile);
    cashRegisterBinaryFileWriter->writeToFile(binaryFile, cashRegisterFile);
    binaryFile.close();
    productFile.close();
    buyerFile.close();
    employedBuyerFile.close();
    providerFile.close();
    cashRegisterFile.close();

    for (int i = 0; i < numberOfProducts; ++i) {
        cout << products[i] << endl;
    }

    for (int i = 0; i < numberOfBuyers; ++i) {
        cout << buyers[i] << endl;
    }

    for (int i = 0; i < numberOfEmployedBuyers; ++i) {
        cout << employedBuyers[i] << endl;
    }

    for (int i = 0; i < numberOfProviders; ++i) {
        cout << providers[i] << endl;
    }

    for (int i = 0; i < numberOfCashRegisters; ++i) {
        cout << cashRegisters[i] << endl;
    }

    int promo[] = { 0, 5, 0, 10, 0, 0, 0};
    Product  product1 ("Oreo", 7, promo);
    cout << product1 << endl;
    Product product2 = product1;
    cout << product2 << endl;

    int promo2[] = { 0, 5, 0, 20, 0, 10, 0};
    product2.setName("Milka");
    product2.setPrice(10);
    product2.setPromotions(promo2, 7);
    cout << product2 << endl;

    Product product3(product2);
    product3 = product3 + 4.00;
    product3.setName("Nutella");
    cout << product3 << endl;

    Buyer buyer1("Madalina Demian", 200.00, 10);
    buyer1.addProduct(product1, 1);
    buyer1.addProduct(product2, 1);
    buyer1.addProduct(product3, 1);
    cout << buyer1 << endl;

    cout << product1.getName() << endl;
    cout << buyer1.getBudget() << endl;
    cout << buyer1.getNumberOfProducts() << endl;
    Product *listOfProducts = buyer1.getProducts();
        for(int i = 0; i < buyer1.getNumberOfProducts(); ++i) {
            cout << listOfProducts[i].getName() << endl;
          }
          
    cout << !buyer1 << endl;
    cout << buyer1 << endl;
    
    buyer1.addProduct(product1, 1);
    buyer1.addProduct(product2, 1);
    buyer1.addProduct(product3, 1);
    //buyer1.pay();
    cout << buyer1 << endl;

    cout << "New budget: " << buyer1.getBudget() + 100 <<endl;
    
    Buyer buyer2("Ana Toma", 544.8, 10);
    cout << buyer2 << endl;
    buyer2.addProduct(product1, 1);
    buyer2.addProduct(product2, 1);
    cout << (double)buyer2 << endl;
   
    EmployedBuyer employee1("Andrei Anuta", 1000.00, 10, 15);
    
    int promo3[] = {10, 0, 0, 10, 5, 0, 0};
    Product product5("Mars", 20, promo3);
    int promo4[] = {0, 0, 5, 0, 0, 0, 0};
    Product product6("Choco duo", 5, promo4);

    employee1.addProduct(product5, 0);
    employee1.addProduct(product6, 1);
    cout << employee1 << endl;
    //employee1.pay();
    //cout << employee1 << endl;

    EmployedBuyer employee2(employee1);
    employee2.setName("Ion Vlaicu");
    cout << employee2;
    
    EmployedBuyer employee3 = --employee1;
    employee3.setName("Marin Tudor");
    cout << employee1 << endl;
    cout << employee3 << endl;
    
    cout << employee3[0] << endl;

    employee3.setBudget(400.50);
    cout << employee3 << endl;
    employee3.pay();

    Product product7("Snickers", 6.5, promo3);
    cout << product7.hasDiscount() << endl;
    cout << product7.calculatePrice(1) << endl;
    cout << endl;

    CashRegister cashRegister1("Cash register 1", 4);
    cashRegister1.addBuyer(buyer1);
    cout << cashRegister1 << endl;
    
    CashRegister cashRegister2(cashRegister1);
    cashRegister2.setName("Cash register 2");
    cashRegister2.addBuyer(employee1);
    cashRegister2.addBuyer(buyer2);
    cout << cashRegister2 << endl;

    // !cashRegister1;
    // cout << cashRegister1 << endl;
  
    if(cashRegister1 == cashRegister2)
       cout << "The cash register 1 has the same capacity as the cash register 2." << endl;
    else cout << "The cash register 1 has NOT the same capacity as the cash register 2." << endl;
    cout << endl;

    cout<<cashRegister2[1];
    cout << endl;

    if (cashRegister1 > cashRegister2)
        cout << "The cash register 1 has the same name than the cash register 2." << endl;
    else cout << "The cash register 1 has NOT the same name than the cash register 2." << endl;
    cout << endl;

    cashRegister2 += 2;
    cout << cashRegister2 << endl;

    // !cashRegister2;
    // cout << cashRegister2 << endl;

    cout << cashRegister2[1] << endl;

    cout << (int)cashRegister1 << endl << endl;

    Provider provider1("Chocolate SRL");
    Product product8(provider1.createProduct("Bounty", 2.99));
    Product product9(provider1.createProduct("Twix", 2.50));
    Product product10(provider1.createProduct("Mars", 2.10));
    provider1.removeProduct(product9);

    cout << provider1 << endl;
    product10.setPromotions(promo3, 7);
    cout << product10 << endl;

    Buyer buyer4("Henri Coanda", 10, 10);
    buyer4.setBudget(100.00);
    buyer4.addProduct(product8, 3);
    buyer4.addProduct(product9, 3);
    buyer4.pay();
    cout << endl;

    Buyer buyer5("Aurel Vlaicu", 200.00, 10);
    buyer5.addProduct(product9, 2);
    buyer5.addProduct(product10, 2);
    buyer5.addProduct(product3, 2);
    --buyer5;
    buyer5.pay();
    cout << endl;

    // !provider1;
    // cout << provider1 << endl;

    cout << provider1[1] << endl;

    Provider provider2(provider1);
    provider2.setName("Nefis");
    provider2.removeProduct(product8);
    provider2.removeProduct(product9);
    Product product11(provider2.createProduct("Biscuiti", 15));
 
    cout << provider2 << endl;

    if(provider1 == provider2)
        cout << "It is the same provider." << endl;
    else cout << "There are 2 different providers." << endl;

    if(provider1 < provider2)
        cout << "The provider 1 has less products than the second provider." << endl;
    else cout << "The provider 1 has more products than the second provider." << endl;
     
    cout  << provider1[1] << endl;

    // !provider1;
    // cout << provider1 << endl;

    Provider provider3;
    provider3.setName("Bucuria");
    // cin >> provider3;
    cout << provider3 << endl;

    cout << product11 << endl;
    product11.setPromotions(promo, 7);
    cout << product11 << endl;
    cout << provider2 << endl;
    char *shopName = "Nr.1";
    Shop shop(shopName, 5, 100, 15);
    CashRegister _cashRegister1("CashRegister1", 4);
    CashRegister _cashRegister2("CashRegister2", 5);
    Buyer _buyer1("Madalina Demian", 200.00, 10);
    Buyer _buyer2("Ana Toma", 300.00, 10);
    Product _product1 ("Oreo", 7, promo);
    Product _product2 ("Twix", 10, promo);
    Product _product3 ("Milka", 8, promo);
    shop.addCashRegister(_cashRegister1);
    shop.addCashRegister(_cashRegister2);
    shop.addProduct(_product1);
    shop.addProduct(_product2);
    shop.addProduct(_product3);
    shop.addBuyer(_buyer1);
    shop.addBuyer(_buyer2);
    shop.placeProduct("Madalina Demian", _product1, 2);
    shop.placeProduct("Madalina Demian", _product2, 2);
    shop.placeProduct("Ana Toma", _product3, 2);
    shop.goToCashRegister(_buyer1);
    shop.goToCashRegister(_buyer2);

    ofstream shopRefortFile("shop-report.rpt");
    ReportGenerator *shopReportGenerator = new ShopReportGenerator(shop);
    shopReportGenerator->generateReport(shopRefortFile);
    shopRefortFile.close();

    ofstream providerReportFile("provider-report.rpt");
    ReportGenerator *providerReportGenerator = new ProviderReportGenerator(provider1);
    ReportGenerator *providerReportGenerator2 = new ProviderReportGenerator(provider2);
    providerReportGenerator->generateReport(providerReportFile);
    providerReportGenerator2->generateReport(providerReportFile);
    providerReportFile.close();

    ofstream productsReportFile("products-generator.rpt");
    ReportGenerator *productsReportGenerator = new ProductReportGenerator(products, numberOfProducts);
    productsReportGenerator->generateReport(productsReportFile);
    productsReportFile.close();
    // shop.pay(0);
    // shop.pay(1);
    // Shop shopp;
    // cin >> shopp;
    // cout<<shopp << endl;
}