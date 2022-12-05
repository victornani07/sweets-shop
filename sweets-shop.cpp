#include <iostream>
#include <string>
#include <string.h>

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
            double budget
        ) {
            strcpy(this->name, name);
            this->budget = budget;
            numberOfProducts = 0;
            maxNumberOfProducts = 10;
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
 
        double calculateTotalPrice() {
            double sum = 0;
            for(int i = 0; i < numberOfProducts; ++i) {
                sum += (double)products[i];
            }

            return sum;
        }

        void addProduct(Product product, int day) {
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
            cout << "The shopping took " << totalPrice << "RON. The buyer " << name << " has a budget of " 
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
            int discountForShopping
        ) : Buyer(name, budget) {
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
        double calculateTotalPrice() {
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

        void addBuyer(Buyer buyer) {
            if (currentServedBuyers == cashRegisterCapacity) {
                return;
            }

            buyers[currentServedBuyers] = buyer;
            ++currentServedBuyers;
        }

        void removeBuyer() {
            if (currentServedBuyers == 0) {
                return;
            }

            for (int i = 1; i < currentServedBuyers; ++i) {
                buyers[i - 1] = buyers[i];
            }
            
            --currentServedBuyers;
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

        bool getIsActive() {
            return isActive;
        }

        int getCashRegisterCapacity() {
            return cashRegisterCapacity;
        }

        int getCurrentServedBuyers() {
            return currentServedBuyers;
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
                out << provider.products[i] << endl;
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

int Product::instances = 0;

int main(void) {
    // Create some promotions
    int promotions1[] = {0, 10, 0, 10, 0, 10, 0};
    int promotions2[] = {0, 0, 0, 5, 5, 0, 0};

    // First Provider with Nefis name and all its products
    Provider nefisProvider("Nefis");
    Product product1(nefisProvider.createProduct("Bounty", 2.99));
    Product product2(nefisProvider.createProduct("Twix", 2.50));
    Product product3(nefisProvider.createProduct("Mars", 2.10));
    nefisProvider.removeProduct(product2);

    product2.setPromotions(promotions1, 7);

    Buyer buyer1("Henri Coanda");
    buyer1.setBudget(100.00);
    buyer1.addProduct(product1, 3);
    buyer1.addProduct(product2, 3);
    buyer1.pay();

    Buyer buyer2("Aurel Vlaicu", 200.00);
    buyer2.addProduct(product3, 2);
    buyer2.addProduct(product3, 2);
    buyer2.addProduct(product3, 2);
    --buyer2;
    buyer2.pay();

    EmployedBuyer employedBuyer1("Ady Mutu", 90.00, 10);
    employedBuyer1.addProduct(product1, 3);
    employedBuyer1.addProduct(product2, 3);
    employedBuyer1.addProduct(product3, 3);
    employedBuyer1.pay();
}