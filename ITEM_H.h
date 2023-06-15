/* ITEM HEADER - USED in THREE OF CLUBS*/

#ifndef ITEM_H
#define ITEM_H
#include <iostream>
using namespace std;

// Function prototype
string getName() ;
string getDescription() ;

// Class declaration
class Item
    { 
    private:
    string name_;
    int weight_; //at some point the player should not be able to carry more and has to drop something. 
    string description_; 
    bool collectible_; // can the item be picked up? 
    bool combinable_; // can the item be combined with another? */
    

    public:
    Item(string name, int weight, string description, bool collectible, bool combinable) 
         : name_(name), weight_(weight), description_(description), collectible_(collectible), combinable_(combinable) {}
    //Item(string name) : name_(name) {}
    Item() : name_("") {}
    
    //Function to return the 'name' of the item.
    string getName() const 
        {
        return name_;
        }
    string getItemDescription() const 
        {
        return description_;
        }
    // Added - poss duplicat of getItemDescription
    void look(Player& player, Room& room) {
        // Perform the look behavior for this item
        std::cout << description_ << std::endl;
        // Additional logic specific to the item's behavior
    }
    
    
    // Function to get "all" the properties of an item (name, weight, etc)
    /* void getItemProperties()
        {
        cout<< getName() << endl;
        //cout<< getweight() << endl; ** TO BUILD
        //cout << getDescription() << endl;
        } */

    };


#endif // ITEM_H