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

        Product(const Product& product) :id(Product::instances++) {
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

        void setName(const char *name) {
            if(strlen(name) < 5) {
                cout << "The name length of the product must contain at least 5 characters." << endl;
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
            int counter1 = 0;
            int counter2 = 0;
            for(int i = 0; i < 7; ++i){
                if(promotions[i] != 0)
                    counter1++;
            }
            for(int i = 0; i < 7; ++i){
                if(product.promotions[i]!= 0)
                    counter2++;
            }

            return counter1 == counter2;
        }

        double calculateDiscount(int day) {
            int promotion = promotions[day];
            double discount = ((double)promotion / 100.0) * price;
            return price - discount;
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
    private:
        char name[30];
        double budget;
        int numberOfProducts;
        int maxNumberOfProducts;
        // HAS-A
        Product *products;

    public:
        Buyer(
            const char name[],
            double budget
        ) {
            strcpy(this->name, name);
            this->budget = budget;
            numberOfProducts = 0;
            maxNumberOfProducts = 1;
            products = new Product[maxNumberOfProducts];
            if(products == NULL) {
                cout << "Memory allocation error. products was not properly initialized.";
                exit(1);
            }
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
            double newProductPrice = product.calculateDiscount(day);
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

        ~Buyer() {
            delete[] products;
           // cout << "A buyer has been destroyed." << endl;
        }
};

int Product::instances = 0;

int main(void) {
    int promotionsProduct1[] = {0, 10, 5, 10, 0, 0, 0};
    Product product1("Milka Oreo", 4.99, promotionsProduct1);
    // cout << product1 << endl;

    int promotionsProduct2[] = {0, 10, 20, 0, 8, 0, 40};
    Product product2;
    product2.setPrice(9.99);
    product2.setName("Nutella");
    product2.setPromotions(promotionsProduct2, 7);
    // cout << product2 << endl;

    Product product3(product2);
    product3 = product3 + 5.00;
    // cout << product3 << endl;

    // Product product4;
    // product4 = product1;
    // product4 += 5.00;
    // cout << product4 << endl;

    // Product product5;
    // // cin >> product5;
    // // cout << product5 << endl;
    
    // cout << "The discount for the first product on WEDNESDAY is: " << product1[2] << "%" << endl;
    
    // cout << endl << "The new price for the third product: \n";
    // product4 = product3--;
    // cout << product3 << endl;
    // cout << product4 << endl;
    
    // cout << endl << "The new prices for the third and fourth product: \n";
    // product4 = --product3;
    // cout << product3 << endl;
    // cout << product4 << endl;
    
    // cout << endl << "The price for the first product: " << double(product1) << endl;

    // if(!product2)
    //   cout << endl << "There are disscounts for the second product" << endl;
    // else cout << endl << "There are NO disscounts for the second product" << endl;

    // if(product1 > product2)
    //     cout << endl << "The price of the first product is bigger than the price of the second product" << endl;
    // else cout << endl << "The price of the first product is NOT bigger than the price of the second product" << endl;
    
    
    // if(product1 == product2)
    //     cout << endl << "The first product has the same number of days of disscounts as the second product" << endl;
    // else cout << endl << "The first product has different number of days of disscounts " << endl;
    // int *promo = product1.getPromotions();
    // for(int i = 0; i < 7; ++i) {
    //     cout << promo[i] << " ";
    // }

    // int promotionsProduct2[] = {0, -10, 5, 10, 0, 0, 0};
    // product1.setPromotions(promotionsProduct2, 7);
    int day = 1;
    Buyer buyer("Madalina Demian", 200.23);
    buyer.addProduct(product1, day);
    buyer.addProduct(product2, day);
    buyer.addProduct(product3, day);
    cout << buyer << endl;
    //cout << !buyer << endl;
    cout << buyer << endl;
    buyer.pay();
    cout << buyer << endl;
    buyer.addProduct(product1, day);
    buyer.addProduct(product2, day);
    cout << buyer << endl;
    buyer.pay();

}