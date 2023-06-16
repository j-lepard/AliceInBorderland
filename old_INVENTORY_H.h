/* INVENTORY HEADER used in THREE OF CLUBS*/

#ifndef INVENTORY_H
#define INVENTORY_H
#include <iostream>
#include <map>
#include <unordered_map>
using namespace std;

// Function prototype
void addItem(const Item& item);
Item takeItemInv(const string& itemName);
void subtrItem(const Item& item);
int size();
void listInventory();
bool inInventory (const string& itemName);
void clear();

// Class declaration
class Inventory
    {
    private:
    // InventoryItems as an UNORDERED_MAP
    unordered_map<string, Item> inventoryItems_;
    int itemCount;
    
public: 
    Inventory() : itemCount(0) {}

     // Pickup item and Add to Inventory
     // max inventory is 5, so if item count < 5, ok to add, else, cout "error"
    
    void clear() 
        {
        inventoryItems_.clear();
        }
    
    void addItem(const Item& item) 
        {
        if (inventoryItems_.size() < 5) 
            {
            inventoryItems_[item.getName()] = item;
            } 
        else 
            {
            cout << "You have maxed your inventory" << endl;
            }
        }
    
    // Taking an item need to convert from STRING to ITEM
    // Then remove the item from the Inventory.
    // Same function required in Room
    Item takeItemInv(const string& itemName) 
        {
        Item takenItem = inventoryItems_[itemName];
        inventoryItems_.erase(itemName);
        return takenItem;
        }


    // Drop item from inventory
    void subtrItem(const Item& item) 
        {
        auto iter = inventoryItems_.find(item.getName());
            if (iter != inventoryItems_.end()) 
                {
                inventoryItems_.erase(iter);
                }
        }

    // How many items are in the current inventory (count)
    int size() const 
        {
        return itemCount;
        }
  
    const Item& operator[](int i) const 
        {
        if (i < 0 || i >= inventoryItems_.size()) 
            {
            throw std::out_of_range("Invalid index");
            }
    
        auto iter = inventoryItems_.begin();
        advance(iter, i);
        return iter->second;
        }
    void listInventory() 
        {
        cout << "The inventory contains the following items: "<< endl;
            for (const auto& pair : inventoryItems_) 
            {
            cout << pair.first << endl;
            }
        }
    
    // This is correct from a syntax perspective. 
    bool inInventory (const string& itemName) 
        {
        return inventoryItems_.find(itemName) != inventoryItems_.end();
        }
    };

#endif // INVENTORY_H