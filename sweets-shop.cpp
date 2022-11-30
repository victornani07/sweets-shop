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
    
    public:
        Product(int id) : id(id) { }

        Product(
            int id,
            const char *name,
            double price,
            int promotions[]
        ) :id(id) {
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

        ~Product() {
            delete[] name;
            cout << "A product has been destroyed." << endl << endl;
        }
};


int main(void) {
    int promotionsProduct1[] = {0, 10, 5, 10, 0, 0, 0};
    Product product1(1, "Milka Oreo", 4.99, promotionsProduct1);
    cout << product1 << endl;

    int promotionsProduct2[] = {0, 5, 0, 12, 0, 0, 0};
    Product product2(2);
    product2.setPrice(9.99);
    product2.setName("Nutella");
    product2.setPromotions(promotionsProduct2, 7);
    cout << product2 << endl;

    Product product3(product2);
    product3 = product3 + 5.00;
    cout << product3 << endl;

    Product product4(4);
    product4 = product1;
    product4 += 5.00;
    cout << product4 << endl;


    // int *promo = product1.getPromotions();
    // for(int i = 0; i < 7; ++i) {
    //     cout << promo[i] << " ";
    // }

    // int promotionsProduct2[] = {0, -10, 5, 10, 0, 0, 0};
    // product1.setPromotions(promotionsProduct2, 7);
    
}