// ALICE IN BORDERLAND - ASSIGNMENT #5

/* 
 Title: THREE_OF_CLUBS
 Description: A simple Text Adventure game.. that remains as a beta release.
 
 Date: 15-Jun 2023
 Author: Jamie Lepard
 Version: 0.8
 Copyright: 2023 Jamie Lepard
*/

/*
 DOCUMENTATION
 
 Program Purpose:
 	A game that attempts to recreate the Three of Clubs puzzle from the Netflix program "Aice in Borderland"

 Compile: g++ -o ThreeOfClubsv0.8.cpp
 Execution: ThreeOfClubsv0.8.exe
 
  
 Classes: 
    Item
    Inventory
    Room
    Player
    Control
    Action
    Game

*/

/* TEST PLAN
    1. 
    2. 
    3.  
  
*/


#include <array>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <tuple>
#include <unordered_map>
// These are for the stopwatch and room timers
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;

//FORWARD DECLARE PLAYER Game and Room
class Player;
class Game;
class Room;



// ITEM Class declaration
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

    void getItemDescription(Player& player, Room& room) {
        // Perform the look behavior for this item
        std::cout << description_ << std::endl;
        // Additional logic specific to the item's behavior
    }
};

// INVENTORY Class declaration
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

// ROOM Class declaration
class Room 
    {
    private: 
    unordered_map<string, Item> roomItems_;
    string description_;
    bool explored_ = false; //false=NO, true=cleared, 
    bool isLight_ ; //when entering the room is it lit? 
    int roomTimer_= 45; // the amount of time the player is provided in the room before death. 
    bool timerRunning = false;
    std::chrono::system_clock::time_point startTime;

    map<string, Room*> doors_;
    Player* player_;
           
    public:
        
    // Constructors DEFINITION (removed the Declaration for later)
    Room(){}; //default constructor needed for <Array>
    Room(string description, Player* p, int roomTimer): description_(description), player_(p), roomTimer_(roomTimer) {};; 
   
    // Function to ADD the Door to the Room. This is one part where the MAP STL is required
    void addDoor(string direction, Room* room) {
        doors_[direction] = room;
        }

    // Function to call the Door (exit) for the *current* room. If no door object for the current room, returns nullptr (nothing). If valid, update the Room (doors_[direction])
    Room* getDoor(string direction) {
        if (doors_.find(direction) == doors_.end()) {
            // This direction is not in the map, return nullptr
            return nullptr;
            } 
            else {
            // This direction is in the map, return the Room*
            return doors_[direction];
            }
        }

    // If Room has NOT been explored, then provide the description
    // Also the player can call this function with a "Look Room" 
    string getDescription() {
        if (!explored_) {
            explored_ = true; //if the room has NOT been explored, the function will enter here, then change to status to TRUE. 
            return description_; //once set to True, it will give the room description (only once!)
            } 
            else {
            return "";
            }
        }
    string lookRoom() {
        return description_; 
        }
    
    // Called when the player first enters a room (different that remaining timer which is called when player "look timer")
    int getRoomTimer() {
        return roomTimer_;
        } 
    
    // Reset Rooms: 
    void resetExplored() {
        explored_ = false;
        }

    void resetRoomTimer() {
        roomTimer_;
        }    


    // ROOM ITEMS SECTION //
    
    // addRoomItem Function adds a previously created item to a particular room.
    // Used in 2 cases: 
        // 1. Game creation and instantiating room objects. 
        // 2. Player DROPs an item and effectively adds it back to the room. 
    void addRoomItem(const string& itemName, const Item& item) {
        roomItems_[itemName] = item;
        }
   
    // Taking an item need to convert from STRING to ITEM
    // Then remove the item from the roomInventory.
    // Same function required in player
    Item takeRoomItem(const string& itemName) {
        Item takenItem = roomItems_[itemName];
        roomItems_.erase(itemName);
        return takenItem;
        }
    
    void listRoomInventory() {
        //DEBUG cout << "The room contains the following items: "<< endl;
            for (const auto& pair : roomItems_){
            cout << pair.first << endl;
            }
        }
       
    // For later - can combine this with 
    bool hasItem(const string& itemName) {
        return roomItems_.find(itemName) != roomItems_.end();
    }

    // ROOM TIMER SECTION -------------------------------
    void enterRoom() {
        startTime = std::chrono::system_clock::now();
        //DEBUG: room timer starting
        //cout << "started room timer" << endl;
        //cout << startTime() << endl;
        }
    
    void exitRoom() {
        auto endTime = std::chrono::system_clock::now();
        std::chrono::duration<int> elapsed = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        roomTimer_ -= elapsed.count();
        //cout << endTime()  <<  endl;
         }
    
    int getRemainingTimeInRoom() const {
        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
        return roomTimer_ - elapsed.count();
        }
    };

// PLAYER Class declation
class Player {
    private: 
    bool isAlive = true; //you start the game in an Alive=true state. Will use this parameter for a "wile is alive..."

    public:
    Room* currentRoom;
    Player (){};
    Player(Room* startRoom, bool isAlive) : currentRoom(startRoom),isAlive(true) {};
    ~Player() {
        // DEBUG: cout << "Player destructor called" << endl;
        };
    Inventory playerInventory;
    
    // LIVING STATUS SECTION -----
    bool getAliveState() {
        return isAlive;
        }
    
    void setAliveState(bool alive) {
        isAlive = alive;
        }   

    // INVENTORY MANAGEMENT SECTION
    // This might not be required anymore (had it wehn was an array)
    size_t getInventorySize() const {
        return 5;
        }
        
    void takeItem(Item& item) {
        playerInventory.addItem(item);
        }
    
    void dropItem(Item& item) {
        playerInventory.subtrItem(item);
        } 
    
    void lookInventory() {
        playerInventory.listInventory();
        }
    
    // Does the player have the item in inventory? Used for dropAction
    // Move this up to the Inventory class, cause its an inventory thing, not a player thing.
    bool hasItem(const string& itemName) {
        bool itemExists = playerInventory.inInventory(itemName);
        cout << "Item '" << itemName << "' in inventory: " << (itemExists ? "true" : "false") << endl;
        return itemExists;
        }
    };

// GLOBAL gameItems map -probably poor form :()
unordered_map<string, Item> gameItems={
        {"PHONE", Item("PHONE", 1, "A basic smartphone.", true, false)},
        {"BOOK", Item("BOOK", 2, "An old and dusty book.", true, false)},
        {"WATCH", Item("WATCH", 1, "A small pocketwatch. Mysterious as it has no obvious buttons", true, false)},
        {"TABLE", Item("TABLE", 200, "A small plain table", false, false)},
        {"MATCH", Item("MATCH", 1, "A book of matches", true, true)},
        {"PAPER", Item("PAPER", 2, "A stack of approximately 20 sheets of paper", true, false)},
        {"ROPE", Item("ROPE", 2, "a length of cord about 15 long. Looks weak", true, false)},
        {"KEYS", Item("KEYS", 1, "Set of car keys, a house key and a Mysterious key", true, true)},
        {"FLAMINGO", Item("FLAMINGO", 1, "Its a pink flamingo.\nApparently used to hit small round objects", true, true)},
        {"HATTER", Item("HATTER", 1, "A Strange person who keeps moving around the room, making short, personal remarks, asking unanswerable riddles.", true, true)},
        {"CAT", Item("CAT", 1, "A cat of English-origin sporting a feindish grin.", true, true)}
        };

// ACTION Class declaration
class Action {
    public: 
    // This is our PURE VIRTUAL VOID function - there is no default
    virtual void execute_action(Player& player, Room& room, string direction = "") =0;
    };
class moveAction: public Action {
    private: 
    
    public:
    moveAction() {}
    void execute_action(Player& player, Room& room, string direction) override  {
           //DEBUG:  cout << "do action\n";
        Room* nextRoom = room.getDoor(direction);
        if (nextRoom != nullptr) {
            player.currentRoom->exitRoom();
            player.currentRoom = nextRoom;
            player.currentRoom->enterRoom();
            //Debug: cout << "call start timer\n";           
            //DEBUG: cout << "call stop timer\n";
            } 
        else {
            std::cout << "There is no door in that direction.\n";
        }
        }
    };
class takeAction: public Action {
    private:
    public: 
    takeAction () {}
    void execute_action(Player& player, Room& room, string object) override{
        // Is the object IN this room?
        if(room.hasItem(object)){
            //Remove the item from the room's inventory
            Item item = room.takeRoomItem(object);
            
            //Add the same item to the player inventory.
            player.takeItem(item);
            cout << "you took the " << object << endl;
            } 
        else{
            cout << "THERE IS NO SUCH ITEM IN THE ROOM" << endl;
            }
        }
};
class dropAction: public Action {
    private:
    public: 
    dropAction () {}
    void execute_action(Player& player, Room& room, string object) override {
        bool itemExists = player.hasItem(object);
            if (itemExists) {   
                cout << "YOU DROPPED THE " << object << endl;
                // Remove the item from the Players's inventory
                Item droppedItem = player.playerInventory.takeItemInv(object);
                
            // RETURN ITEM TO ROOM INVENTOYR DUE TO GAME SCOPE
                // Add the same item to the ROOM inventory.
                room.addRoomItem(object, droppedItem);  
                }  
                 
            else {
                cout << "THERE IS NO SUCH ITEM IN YOUR INVENTORY" << endl;
                cout << endl;
                }  
        }  
};
class lookAction: public Action{
    private:
    
    public: 
    lookAction (){}
    void execute_action(Player& player, Room& room, string object) override{
        //DEBUG: cout << "you called the Look Action on:"<< object << endl;
    
    // Item Description function. 
    // SHOULD BE constrained to items in the CURRENT ROOM or in INVENTORY!~
        if(object == "INVENTORY" || object == "BAG" || object == "POCKET" ) {
                    // Show items in the inventory... 
                    player.lookInventory();
                    cout << endl;
        }
        if(object == "WATCH"){
            if(player.hasItem("WATCH")){
                cout << "THERE ARE: " << player.currentRoom->getRemainingTimeInRoom()  << " SECONDS REMAINING ON THE TIMER"<< endl;
                cout << endl;
                }
        }   
        if(object == "ROOM" ) {
            cout << player.currentRoom->lookRoom() << endl;
            cout << endl;
            //Separated the Room inventory from the Look Table
            cout << "YOU SEE THE FOLLOWING ITEMS IN THE ROOM:" << endl;
            room.listRoomInventory();
            cout << endl; 
        }
        else {
            auto it = gameItems.find(object);
            if (it != gameItems.end()) {
                it->second.getItemDescription(player, room);
                //DEBUG: cout << "Yes, this item exists in the game" << endl;

                if (player.currentRoom->hasItem(object)) {
                //DEBUG:    cout << "The item is also in this room" << endl;
                }
                else {
                //DEBUG:    cout << "The item is NOT in this room" << endl;
                }
                }
                else {
                    cout << "THERE IS NO ITEM WITH THAT NAME IN THE GAME" << endl;
                }
        }    
    }
};
class useAction: public Action{
    private:
    public: 
    useAction (){}
    void execute_action(Player& player, Room& room, string object) override{
        cout << "you called the USE Action"<< endl;
        }
    };
class talkAction: public Action{
    private:
    public: 
    talkAction (){}
    void execute_action(Player& player, Room& room, string object) override{
        cout << "you called the TALK Action"<< endl;
        cout << "'TALK' actions will come in a future release"<< endl;
        }
    };
class openAction: public Action{
    private:
    public: 
    openAction (){}
    void execute_action(Player& player, Room& room, string object) override {
        cout << "You called the 'OPEN' Action"<< endl;
        cout << "Simply 'move' through the door"<< endl;
        cout << "'OPEN' actions will come in a future release"<< endl;
        }
    };   

// CONTROL Class declaration
class Control {
    private:
    map<string, Action*> verbMapping;
        
    public:
    // Control Mapping is the class that aligns a 'verb' input from user to a generic action class/function
    // "movement" words as mapped to the moveAction class
    // "take" words are mapped to the takeAction class
    // "drop" words are mapped to the dropAction class
    // "look" words are mapped to the lookAction class
    // "use" words are mapped to the useAction class
    // "talk" words are mapped to the talkAction class
    // "open" words are mapped to the openAction class

    Control() {
        verbMapping["GO"] = new moveAction();
        verbMapping["MOVE"] = new moveAction();
        verbMapping["AMBLE"] = new moveAction();
        verbMapping["WANDER"] = new moveAction();
        verbMapping["STROLL"] = new moveAction();
        verbMapping["SAUNTER"] = new moveAction();
        verbMapping["MEANDER"] = new moveAction();
        verbMapping["ROAM"] = new moveAction();
        verbMapping["MARCH"] = new moveAction();
        verbMapping["STRIDE"] = new moveAction();
        verbMapping["PADDLE"] = new moveAction();
        verbMapping["HIKE"] = new moveAction();
        verbMapping["TREAD"] = new moveAction();
        verbMapping["TRUDGE"] = new moveAction();
        verbMapping["PERAMBULATE"] = new moveAction();
        verbMapping["LUMBER"] = new moveAction();
        verbMapping["RAMBLE"] = new moveAction();
        verbMapping["STRUT"] = new moveAction();
        verbMapping["SLOG"] = new moveAction();
        verbMapping["PROMENADE"] = new moveAction();
        verbMapping["ROVE"] = new moveAction();
        verbMapping["TRAIPSE"] = new moveAction();
        verbMapping["RUN"] = new moveAction();
        verbMapping["GO"] = new moveAction();
        verbMapping["CRAWL"] = new moveAction();
        verbMapping["DASH"] = new moveAction();
        verbMapping["SPRINT"] = new moveAction();
        verbMapping["RACE"] = new moveAction();
        verbMapping["RUSH"] = new moveAction();
        verbMapping["SPEED"] = new moveAction();
        verbMapping["JOG"] = new moveAction();
        verbMapping["DART"] = new moveAction();
        verbMapping["BOLT"] = new moveAction();
        verbMapping["HUSTLE"] = new moveAction();
        verbMapping["SCAMPER"] = new moveAction();
        verbMapping["SCUTTLE"] = new moveAction();
        verbMapping["SCURRY"] = new moveAction();
        verbMapping["TROT"] = new moveAction();
        verbMapping["CHARGE"] = new moveAction();
        verbMapping["FLY"] = new moveAction();
        verbMapping["SHOOT"] = new moveAction();
        verbMapping["HIGHTAIL"] = new moveAction();
        verbMapping["SCORCH"] = new moveAction();
        verbMapping["WHIZZ"] = new moveAction();
        verbMapping["CREEP"] = new moveAction();
        verbMapping["INCH"] = new moveAction();
        verbMapping["SLITHER"] = new moveAction();
        verbMapping["DRAG"] = new moveAction();
        verbMapping["SLIDE"] = new moveAction();
        verbMapping["SHUFFLE"] = new moveAction();
        verbMapping["LIMP"] = new moveAction();
        verbMapping["LURCH"] = new moveAction();
        verbMapping["SLINK"] = new moveAction();
        verbMapping["SNEAK"] = new moveAction();
        verbMapping["STAGGER"] = new moveAction();
        verbMapping["TRUDGE"] = new moveAction();
        verbMapping["WIGGLE"] = new moveAction();
        verbMapping["SKULK"] = new moveAction();
        verbMapping["PLOD"] = new moveAction();
        verbMapping["WRIGGLE"] = new moveAction();
        verbMapping["GROVEL"] = new moveAction();
        verbMapping["SQUIRM"] = new moveAction();
        verbMapping["SIDLE"] = new moveAction();
        verbMapping["SCURRY"] = new moveAction();
        verbMapping["USE"] = new useAction(); 
        verbMapping["UTILIZE"] = new useAction();
        verbMapping["EMPLOY"] = new useAction();
        verbMapping["OPERATE"] = new useAction();
        verbMapping["HANDLE"] = new useAction();
        verbMapping["MANIPULATE"] = new useAction();
        verbMapping["HARNESS"] = new useAction();
        verbMapping["APPLY"] = new useAction();
        verbMapping["DEPLOY"] = new useAction();
        verbMapping["EXERT"] = new useAction();
        verbMapping["EXPLOIT"] = new useAction();
        verbMapping["WIELD"] = new useAction();
        verbMapping["ADMINISTER"] = new useAction();
        verbMapping["ENGAGE"] = new useAction();
        verbMapping["ADOPT"] = new useAction();
        verbMapping["IMPLEMENT"] = new useAction();
        verbMapping["CONTROL"] = new useAction();
        verbMapping["CONDUCT"] = new useAction();
        verbMapping["WORK"] = new useAction();
        verbMapping["GUIDE"] = new useAction();
        verbMapping["DIRECT"] = new useAction();
        verbMapping["EXERCISE"] = new useAction();
        verbMapping["INVOKE"] = new useAction();
        verbMapping["RUN"] = new useAction();
        verbMapping["DRIVE"] = new useAction();
        verbMapping["NAVIGATE"] = new useAction();
        verbMapping["MANEUVER"] = new useAction();
        verbMapping["MASTER"] = new useAction();
        verbMapping["PILOT"] = new useAction();
        verbMapping["EXECUTE"] = new useAction();
        verbMapping["ENACT"] = new useAction();
        verbMapping["MANAGE"] = new useAction();
        verbMapping["PLAY"] = new useAction();
        verbMapping["PRESS"] = new useAction();
        verbMapping["ACTIVATE"] = new useAction();
        verbMapping["COMMAND"] = new useAction();
        verbMapping["PRACTICE"] = new useAction();
        verbMapping["LEVERAGE"] = new useAction();
        verbMapping["OPERATE"] = new useAction();
        verbMapping["YIELD"] = new useAction();
        verbMapping["PRODUCE"] = new useAction();
        verbMapping["CRAFT"] = new useAction();
        verbMapping["EXTRACT"] = new useAction();
        verbMapping["DRAW"] = new useAction();
        verbMapping["DESIGN"] = new useAction();
        verbMapping["GENERATE"] = new useAction();
        verbMapping["CREATE"] = new useAction();
        verbMapping["DEVELOP"] = new useAction();
        verbMapping["BUILD"] = new useAction();
        verbMapping["CONSTRUCT"] = new useAction();
        verbMapping["ARRANGE"] = new useAction();
        verbMapping["FORMULATE"] = new useAction();
        verbMapping["MODEL"] = new useAction();
        verbMapping["CARVE"] = new useAction();
        verbMapping["SHAPE"] = new useAction();
        verbMapping["MOLD"] = new useAction();
        verbMapping["SCULPT"] = new useAction();
        verbMapping["COMPOSE"] = new useAction();
        verbMapping["ASSEMBLE"] = new useAction();
        verbMapping["EFFECT"] = new useAction();
        verbMapping["CAST"] = new useAction();
        verbMapping["TALK"] = new talkAction();
        verbMapping["CHAT"] = new talkAction();
        verbMapping["SPEAK"] = new talkAction();
        verbMapping["CONVERSE"] = new talkAction();
        verbMapping["DISCUSS"] = new talkAction();
        verbMapping["GAB"] = new talkAction();
        verbMapping["PRATTLE"] = new talkAction();
        verbMapping["BLABBER"] = new talkAction();
        verbMapping["YAK"] = new talkAction();
        verbMapping["JABBER"] = new talkAction();
        verbMapping["BANTER"] = new talkAction();
        verbMapping["RAMBLE"] = new talkAction();
        verbMapping["NATTER"] = new talkAction();
        verbMapping["PALAVER"] = new talkAction();
        verbMapping["RATTLE"] = new talkAction();
        verbMapping["UTTER"] = new talkAction();
        verbMapping["MURMUR"] = new talkAction();
        verbMapping["MUMBLE"] = new talkAction();
        verbMapping["PRATE"] = new talkAction();
        verbMapping["YATTER"] = new talkAction();
        verbMapping["BABBLE"] = new talkAction();
        verbMapping["BLATHER"] = new talkAction();
        verbMapping["CHATTER"] = new talkAction();
        verbMapping["YAP"] = new talkAction();
        verbMapping["CHIT-CHAT"] = new talkAction();
        verbMapping["WHISPER"] = new talkAction();
        verbMapping["GOSSIP"] = new talkAction();
        verbMapping["CONFAB"] = new talkAction();
        verbMapping["CONFER"] = new talkAction();
        verbMapping["CONSULT"] = new talkAction();
        verbMapping["ADVISE"] = new talkAction();
        verbMapping["PARLEY"] = new talkAction();
        verbMapping["POWWOW"] = new talkAction();
        verbMapping["REASON"] = new talkAction();
        verbMapping["LECTURE"] = new talkAction();
        verbMapping["ORATE"] = new talkAction();
        verbMapping["PREACH"] = new talkAction();
        verbMapping["SPEECHIFY"] = new talkAction();
        verbMapping["ARTICULATE"] = new talkAction();
        verbMapping["SPOUT"] = new talkAction();
        verbMapping["SOLILOQUIZE"] = new talkAction();
        verbMapping["PONTIFICATE"] = new talkAction();
        verbMapping["COMMUNICATE"] = new talkAction();
        verbMapping["ADDRESS"] = new talkAction();
        verbMapping["RAP"] = new talkAction();
        verbMapping["DECLAIM"] = new talkAction();
        verbMapping["EXCLAIM"] = new talkAction();
        verbMapping["PROCLAIM"] = new talkAction();
        verbMapping["VOCALIZE"] = new talkAction();
        verbMapping["VOICE"] = new talkAction();
        verbMapping["EXPRESS"] = new talkAction();
        verbMapping["STATE"] = new talkAction();
        verbMapping["VERBALIZE"] = new talkAction();
        verbMapping["ENUNCIATE"] = new talkAction();
        verbMapping["PRONOUNCE"] = new talkAction();
        verbMapping["VENT"] = new talkAction();
        verbMapping["TELL"] = new talkAction();
        verbMapping["SAY"] = new talkAction();
        verbMapping["DECLARE"] = new talkAction();
        verbMapping["ANNOUNCE"] = new talkAction();
        verbMapping["INFORM"] = new talkAction();
        verbMapping["OPEN"] = new openAction();
        verbMapping["UNSEAL"] = new openAction();
        verbMapping["UNLATCH"] = new openAction();
        verbMapping["UNCLOSE"] = new openAction();
        verbMapping["UNLOCK"] = new openAction();
        verbMapping["UNBOLT"] = new openAction();
        verbMapping["UNBAR"] = new openAction();
        verbMapping["UNFASTEN"] = new openAction();
        verbMapping["RELEASE"] = new openAction();
        verbMapping["ACCESS"] = new openAction();
        verbMapping["ADMIT"] = new openAction();
        verbMapping["EXPOSE"] = new openAction();
        verbMapping["REVEAL"] = new openAction();
        verbMapping["DISCLOSE"] = new openAction();
        verbMapping["UNVEIL"] = new openAction();
        verbMapping["UNCOVER"] = new openAction();
        verbMapping["UNWRAP"] = new openAction();
        verbMapping["SPREAD"] = new openAction();
        verbMapping["EXTEND"] = new openAction();
        verbMapping["PART"] = new openAction();
        verbMapping["SEPARATE"] = new openAction();
        verbMapping["UNZIP"] = new openAction();
        verbMapping["UNBUTTON"] = new openAction();
        verbMapping["UNHOOK"] = new openAction();
        verbMapping["UNLEASH"] = new openAction();
        verbMapping["UNLOOSE"] = new openAction();
        verbMapping["UNCHAIN"] = new openAction();
        verbMapping["FREE"] = new openAction();
        verbMapping["UNFURL"] = new openAction();
        verbMapping["UNROLL"] = new openAction();
        verbMapping["UNFOLD"] = new openAction();
        verbMapping["EXPAND"] = new openAction();
        verbMapping["SPREAD"] = new openAction();
        verbMapping["EXPOSE"] = new openAction();
        verbMapping["UNCAP"] = new openAction();
        verbMapping["UNPLUG"] = new openAction();
        verbMapping["UNCORK"] = new openAction();
        verbMapping["UNHINGE"] = new openAction();
        verbMapping["DISENTANGLE"] = new openAction();
        verbMapping["DISMANTLE"] = new openAction();
        verbMapping["UNBIND"] = new openAction();
        verbMapping["UNSHACKLE"] = new openAction();
        verbMapping["UNTIE"] = new openAction();
        verbMapping["LOOSEN"] = new openAction();
        verbMapping["UNTWINE"] = new openAction();
        verbMapping["UNCLASP"] = new openAction();
        verbMapping["UNSTRAP"] = new openAction();
        verbMapping["UNSTICK"] = new openAction();
        verbMapping["UNPICK"] = new openAction();
        verbMapping["DETACH"] = new openAction();
        verbMapping["LIBERATE"] = new openAction();
        verbMapping["DISENGAGE"] = new openAction();
        verbMapping["UNPIN"] = new openAction();
        verbMapping["UNCLIP"] = new openAction();
        verbMapping["UNSNARL"] = new openAction();
        verbMapping["UNRAVEL"] = new openAction();
        verbMapping["UNBRIDLE"] = new openAction();
        verbMapping["DISEMBARRASS"] = new openAction();
        verbMapping["DISENTHRALL"] = new openAction();
        verbMapping["UNSHUT"] = new openAction();
        verbMapping["CLEAR"] = new openAction();
        verbMapping["TAKE"] = new takeAction();
        verbMapping["GRAB"] = new takeAction();
        verbMapping["SNATCH"] = new takeAction();
        verbMapping["SEIZE"] = new takeAction();
        verbMapping["CAPTURE"] = new takeAction();
        verbMapping["ACQUIRE"] = new takeAction();
        verbMapping["OBTAIN"] = new takeAction();
        verbMapping["PROCURE"] = new takeAction();
        verbMapping["ATTAIN"] = new takeAction();
        verbMapping["GET"] = new takeAction();
        verbMapping["COLLECT"] = new takeAction();
        verbMapping["SECURE"] = new takeAction();
        verbMapping["GAIN"] = new takeAction();
        verbMapping["EARN"] = new takeAction();
        verbMapping["RECEIVE"] = new takeAction();
        verbMapping["ACCEPT"] = new takeAction();
        verbMapping["ADOPT"] = new takeAction();
        verbMapping["PICK"] = new takeAction();
        verbMapping["CHOOSE"] = new takeAction();
        verbMapping["SELECT"] = new takeAction();
        verbMapping["EXTRACT"] = new takeAction();
        verbMapping["GATHER"] = new takeAction();
        verbMapping["CLASP"] = new takeAction();
        verbMapping["GRASP"] = new takeAction();
        verbMapping["HOLD"] = new takeAction();
        verbMapping["CATCH"] = new takeAction();
        verbMapping["CLAIM"] = new takeAction();
        verbMapping["APPROPRIATE"] = new takeAction();
        verbMapping["NAB"] = new takeAction();
        verbMapping["BAG"] = new takeAction();
        verbMapping["POCKET"] = new takeAction();
        verbMapping["HOOK"] = new takeAction();
        verbMapping["NET"] = new takeAction();
        verbMapping["SCORE"] = new takeAction();
        verbMapping["ACCOMPLISH"] = new takeAction();
        verbMapping["ACHIEVE"] = new takeAction();
        verbMapping["WIN"] = new takeAction();
        verbMapping["BUY"] = new takeAction();
        verbMapping["PURCHASE"] = new takeAction();
        verbMapping["ASSUME"] = new takeAction();
        verbMapping["ABSORB"] = new takeAction();
        verbMapping["CONSUME"] = new takeAction();
        verbMapping["DIGEST"] = new takeAction();
        verbMapping["EMBRACE"] = new takeAction();
        verbMapping["ENGROSS"] = new takeAction();
        verbMapping["IMBIBE"] = new takeAction();
        verbMapping["INCORPORATE"] = new takeAction();
        verbMapping["POSSESS"] = new takeAction();
        verbMapping["OWN"] = new takeAction();
        verbMapping["COMMANDEER"] = new takeAction();
        verbMapping["DRAW"] = new takeAction();
        verbMapping["PULL"] = new takeAction();
        verbMapping["REAP"] = new takeAction();
        verbMapping["RETAIN"] = new takeAction();
        verbMapping["SUSTAIN"] = new takeAction();
        verbMapping["UPHOLD"] = new takeAction();
        verbMapping["INGEST"] = new takeAction();
        verbMapping["BEAR"] = new takeAction();
        verbMapping["CARRY"] = new takeAction();
        verbMapping["TRANSPORT"] = new takeAction();
        verbMapping["CONVEY"] = new takeAction();  
        verbMapping["LOOK"] = new lookAction();
        verbMapping["SEE"] = new lookAction();
        verbMapping["VIEW"] = new lookAction();
        verbMapping["WATCH"] = new lookAction();
        verbMapping["OBSERVE"] = new lookAction();
        verbMapping["GAZE"] = new lookAction();
        verbMapping["STARE"] = new lookAction();
        verbMapping["GLANCE"] = new lookAction();
        verbMapping["PEEK"] = new lookAction();
        verbMapping["PEEP"] = new lookAction();
        verbMapping["GLIMPSE"] = new lookAction();
        verbMapping["EYEBALL"] = new lookAction();
        verbMapping["SURVEY"] = new lookAction();
        verbMapping["SCRUTINIZE"] = new lookAction();
        verbMapping["INSPECT"] = new lookAction();
        verbMapping["EXAMINE"] = new lookAction();
        verbMapping["STUDY"] = new lookAction();
        verbMapping["SIGHT"] = new lookAction();
        verbMapping["SCAN"] = new lookAction();
        verbMapping["REGARD"] = new lookAction();
        verbMapping["EYE"] = new lookAction();
        verbMapping["BEHOLD"] = new lookAction();
        verbMapping["WITNESS"] = new lookAction();
        verbMapping["PERCEIVE"] = new lookAction();
        verbMapping["NOTE"] = new lookAction();
        verbMapping["NOTICE"] = new lookAction();
        verbMapping["SPY"] = new lookAction();
        verbMapping["CHECK"] = new lookAction();
        verbMapping["REVIEW"] = new lookAction();
        verbMapping["SCOPE"] = new lookAction();
        verbMapping["GAWK"] = new lookAction();
        verbMapping["OGLE"] = new lookAction();
        verbMapping["LEER"] = new lookAction();
        verbMapping["SEARCH"] = new lookAction();
        verbMapping["SEEK"] = new lookAction();
        verbMapping["SPOT"] = new lookAction();
        verbMapping["DISCOVER"] = new lookAction();
        verbMapping["DETECT"] = new lookAction();
        verbMapping["FIND"] = new lookAction();
        verbMapping["RECOGNIZE"] = new lookAction();
        verbMapping["ADMIRE"] = new lookAction();
        verbMapping["CONTEMPLATE"] = new lookAction();
        verbMapping["PONDER"] = new lookAction();
        verbMapping["CONSIDER"] = new lookAction();
        verbMapping["ASSESS"] = new lookAction();
        verbMapping["EVALUATE"] = new lookAction();
        verbMapping["APPRAISE"] = new lookAction();
        verbMapping["ESTIMATE"] = new lookAction();
        verbMapping["MEASURE"] = new lookAction();
        verbMapping["JUDGE"] = new lookAction();
        verbMapping["EXPLORE"] = new lookAction();
        verbMapping["INVESTIGATE"] = new lookAction();
        verbMapping["ANALYZE"] = new lookAction();
        verbMapping["ATTEND"] = new lookAction();
        verbMapping["MONITOR"] = new lookAction();
        verbMapping["FOLLOW"] = new lookAction();
        verbMapping["TRACK"] = new lookAction();
        verbMapping["SUPERVISE"] = new lookAction();
        verbMapping["OVERSEE"] = new lookAction();
        verbMapping["SURVEIL"] = new lookAction();
        verbMapping["SURVEY"] = new lookAction();
        verbMapping["DROP"] = new dropAction(); 
        verbMapping["DISPOSE"] = new dropAction();
    }

    ~Control() {
        // Delete actions to prevent memory leaks
        for(auto& pair : verbMapping) {
            delete pair.second;
        }
    }

     void handleInput(Player& player, Room& room, string verb, string object, string preposition = "", string indirect_object = "") {
        if (verbMapping.find(verb) != verbMapping.end()) 
            {
            verbMapping[verb]->execute_action(player, room, object);
            } 
        else 
            {
            std::cout << "That didnt work. Try something else" << std::endl;
            }
    }
};

// TEXT FILE READER Class Declaration: 

class TextFileReader{
    private:  
    string txtString [100]; 
    string contents_;
    
    public:
    
    // Default constructor
    TextFileReader (){};
    
    // Paramenter Contructor
    TextFileReader (string fileName)
        {
        fstream gameFile_;
        string Line_;
        int i = 0;
        while (true)
            {     
            //Open file. If file cant be opened  loop until correct name entered
            gameFile_.open(fileName, ios::in);         
            if (!gameFile_.is_open()){
                cout << "There was a problem opening the file. Please confirm spelling" << endl;
                }
            else
                break;  
            }

         // Now file is success opened, take name of file and put each line into array of string (up to 100)
        while (getline(gameFile_, Line_) && i<10){
            txtString[i]=Line_;
            i++;
            }   
        }
    // Display method to present the string at each index within the txtString array.    
    void display(){
        for(int i = 0; i < 10; i++){
            cout << txtString[i] << endl;
            }
        cout << endl;}
    };


// GAME Class declaration
class Game //Purpose of the class is construct/initialize the various elements in the game. The rooms, doors, objects, and the player. 
{
    private:
        
    public:
    
    array<Room,13> rooms;
    Player Arisu;
    Control controlArisu;    
    Game() : Arisu(), controlArisu(){
        // Define the game board, 9 rooms, 2 to outside, 1 to win and 1 more as the 'ante room'.
        rooms[0]= Room("YOU IN A DULL ROOM, WITH NO WINDOWS.\nTHERE IS A SMALL TABLE.\nTHERE ARE TWO DOORS. THE DOOR AHEAD SAYS \"EAT ME\", TO THE RIGHT SAYS \"DRINK ME\"", &Arisu,120);
        rooms[1]= Room("YOU ENTERED THE NEXT ROOM\nYOU SEE A PERSON IN THE ROOM.\nTO YOUR RIGHT IS A DOOR THAT READS \"EAT ME\". STRAIGHT AHEAD THE DOOR READS \"DRINK ME\" ", &Arisu,55);
        rooms[2]= Room("YOU ESCAPED THE PREVIOUS ROOM\n FEW HAVE MADE IT EVEN THIS FAR.\nTHE ROOM IS TOTALLY DARK!!", &Arisu,60);
        rooms[3]= Room("YOU ENTERED THE FOURTH ROOM. THERE ARE TWO DOORS. THE DOOR TO THE RIGHT SAYS \"EAT ME\", THE DOOR AHEAD SAYS \"DRINK ME\"", &Arisu,55);
        rooms[4]= Room("YOU ENTERED THE ROOM....", &Arisu,0);
        rooms[5]= Room("YOU ENTERED THE ROOM....", &Arisu,0);
        rooms[6]= Room("YOU FEEL VERY CLOSE... THE DOOR TO THE RIGHT SAYS \"EAT ME\", THE DOOR TO THE LEFT SAYS \"DRINK ME\"", &Arisu,30);
        rooms[7]= Room("YOU ENTERED YET ANOTHER ROOM. THE DOOR TO THE RIGHT SAYS \"DRINK ME\", THE DOOR AHEAD SAYS \"EAT ME\"", &Arisu,40);
        rooms[8]= Room("YOU ENTERED A ROOM THAT LOOKS EXACTLY THE SAME AS ALL PREVIOUS.\nTHERE ARE TWO DOORS. THE DOOR AHEAD SAYS \"EAT ME\", THE DOOR TO THE LEFT SAYS \"DRINK ME\"", &Arisu,45);
        rooms[9]= Room("YOU ENTERED THE ROOM....", &Arisu,0);
        rooms[10]= Room("YOU ENTERED THE ROOM....", &Arisu,0);
        rooms[11]= Room("YOU ENTERED ROOM 3 AND WON!!", &Arisu,200);
        rooms[12]= Room("YOU ARE STANDING BEFORE A RED ELEVATOR DOOR.\nA RABBIT, STANDING ON HIS HIND LEGS, DISAPPEARS THROUGH THE DOOR\nGO STRAIGHT TO ENTER THE RABBIT HOLE", &Arisu,200);
       
       // Assign the Doors to each of the rooms below Arguments to addDoor are the direction and pointer to the new room.
       rooms[0].addDoor("RIGHT", &rooms[5]); //You die if you go this way 
       rooms[0].addDoor("STRAIGHT", &rooms[1]); 
       rooms[1].addDoor("RIGHT", &rooms[4]); //Dead
       rooms[1].addDoor("STRAIGHT", &rooms[2]);
       rooms[2].addDoor("RIGHT", &rooms[3]);
       rooms[2].addDoor("LEFT", &rooms[9]); //Dead
       rooms[3].addDoor("RIGHT", &rooms[4]); //Dead
       rooms[3].addDoor("STRAIGHT", &rooms[8]);
       rooms[8].addDoor("STRAIGHT", &rooms[10]); //Dead
       rooms[8].addDoor("RIGHT", &rooms[7]);
       rooms[7].addDoor("RIGHT", &rooms[4]); //dead
       rooms[7].addDoor("STRAIGHT", &rooms[6]);
       rooms[6].addDoor("LEFT", &rooms[11]);
       rooms[6].addDoor("RIGHT", &rooms[5]); //Dead
       rooms[12].addDoor("STRAIGHT", &rooms[0]); //START
    
        // Create the ITEMS of the game.
        
        
        //Add the Game Items to their starting locations in Room Inventory:
        //need to pass addItem both the name of the item and the actual Item object as it is an unordered_map
        rooms[0].addRoomItem("PHONE", gameItems["PHONE"]);
        rooms[0].addRoomItem("WATCH", gameItems["WATCH"]);
        rooms[0].addRoomItem("MATCH", gameItems["MATCH"]);
        rooms[2].addRoomItem("PAPER", gameItems["PAPER"]);
        rooms[0].addRoomItem("BOOK", gameItems["BOOK"]);
        rooms[0].addRoomItem("KEYS", gameItems["KEYS"]);
        rooms[0].addRoomItem("TABLE", gameItems["TABLE"]);
        rooms[3].addRoomItem("FLAMINGO", gameItems["FLAMINGO"]);
        rooms[7].addRoomItem("HATTER", gameItems["HATTER"]);
        rooms[8].addRoomItem("CAT", gameItems["CAT"]);
        

        // Create the player to the first room after rooms initialization
       Arisu = Player(&rooms[12],true);
       }
       
    string getCurrentRoomDescription(){
        return Arisu.currentRoom->getDescription();
        }
    // Function makes Arise living status available in the gameLoop
    bool ArisuAlive(){
            return Arisu.getAliveState();
        }
    
    // Critical function for Game - check the remaining time in the current room.
    void checkRemainingTimer(){
        // This function prevents the room timer from killing Arisu in the first room!!
        if (!Arisu.currentRoom->getRoomTimer()==200)
        {
        // Once Arisu moves into a room, the timer starts counting down.
        if(Arisu.currentRoom->getRemainingTimeInRoom() <= 0) {
            Arisu.setAliveState(false);
            std::cout << "A SOFT WHIRRING SOUND STARTS TO EMINATE FROM ABOVE.\nYOU LOOK UP AND THE LAST THING YOU SEE IS A BRILLIANT FLASH OF LIGHT"<< endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "A LASER HAS JUST PASSED THROUGH YOUR SKULL.\nYOU ARE DEAD."<< endl;
            return ;
            }   
        }
    }

    
    // This function is the main game loop. It is called from main() and runs until the player dies or wins.
    void gameLoop() {

        
            string roomDescription = Arisu.currentRoom->getDescription();
            if (!roomDescription.empty()){
                cout << roomDescription << endl;
            }
            
            // ----INPUT FROM USER COLLECTED, VALIDATED AND PARSED -----
            cout << endl;
            cout << "WHAT DO YOU WANT TO DO?\n*WARNING:* Do not take too long to decide.. "<< endl;
            

            while(true) {

                string string_;
                //DEBUG:: cout << "confirmed enter commandVector\n";
                std::getline(cin, string_);
                
                std::string word_;
                std::vector<string> commandVector;
                int length = string_.length(); //This gives the total phrase length
                
                //Convert the commanVector to Upper. Starting at index 0, until length is reached, then increase the index
                for (int index = 0; index < length; index++) {
                        string_[index] = toupper(string_[index]); //index 0 = "T" index 1 = "H" etc
                    }

                stringstream iss(string_);
                while (iss >> word_)
                    commandVector.push_back(word_);

                    // DEBUG:: Check the vector contents (for debug)
                    /* for (const string &word : commandVector)
                        cout << word << ' ' << endl;
                        cout << endl; */
            
                // CLS - Clear the screen before printing the next room description
                std::cout << "\033c"; //Clear the screen
                
                // CHECK INPUT IS OK LENGTH - IF checks to make sure that the command is sufficient length
                // Also intro a ternary operator "? with a : "
                if (commandVector.size() >= 2){
                    string verb = commandVector[0];
                    string object = commandVector[1];
                    string preposition = commandVector.size() > 2 ? commandVector[2] : "";
                    string indirect_object = commandVector.size() > 3 ? commandVector[3] : "";
                    controlArisu.handleInput(Arisu, *Arisu.currentRoom, verb, object, preposition, indirect_object);
                    //DEBUG: cout << "finish processing input\n";
                    }
                else{
                    if (commandVector[0] == "HELP"){
                        cout << "HELP? REALLY?\nYOU WENT DOWN THE RABBIT HOLE.\nYOU ARE ON YOUR OWN!!" << endl;
                        cout << endl;
                        }
                        else{
                        cout << "Please enter a valid command.\nIt needs to be at least 2 words.";
                        }
                }

                
                
                string roomDescription = Arisu.currentRoom->getDescription();
                cout << "print elevator description\n";
                if (!roomDescription.empty()){
                    //DEBUG:: cout << "room\n";
                    cout << roomDescription << endl;
                    if(Arisu.currentRoom->getRoomTimer()!=0){
                        cout << "[QUEEN OF HEARTS]: THE TIME LIMIT FOR THIS ROOM IS: " << Arisu.currentRoom->getRoomTimer() << " SECONDS" <<endl;
                        }}
                //DEBUG:: cout << "completed parsing user input" << endl;
                
                checkRemainingTimer();
                if (!Arisu.getAliveState()){
                    break;
                    }
                else {// Get input from the user
                    cout << endl;
                    cout << "-> WHAT DO YOU WANT TO DO?"<< endl; 
                    //std::cout << "\033c"; //Clear the screen
                    }                                       
            }
        }
    // GAME RESET FUNCTION ----- 
    void resetGame()
        {
        // Player is reset to starting position and Alive.
        Arisu = Player(&rooms[12],true);
        
        // Reset all rooms to unexplored 
        // Reset all room timers. 
        for (int i = 0; i < 13; i++){
            rooms[i].resetExplored();
            rooms[i].resetRoomTimer();
            cout << "Room " << i << " has been reset" << endl;
            }
        // Reset player inventory to empty
        Arisu.playerInventory.clear(); 
        
        // Reset room inventory --- future work to make this more scalable/consolidated.
        rooms[0].addRoomItem("PHONE", gameItems["PHONE"]);
        rooms[0].addRoomItem("TIMER", gameItems["TIMER"]);
        rooms[0].addRoomItem("MATCH", gameItems["MATCH"]);
        rooms[2].addRoomItem("PAPER", gameItems["PAPER"]);
        rooms[0].addRoomItem("BOOK", gameItems["BOOK"]);
        rooms[0].addRoomItem("KEYS", gameItems["KEYS"]);
        rooms[0].addRoomItem("TABLE", gameItems["TABLE"]);
        rooms[3].addRoomItem("FLAMINGO", gameItems["FLAMINGO"]);
        rooms[7].addRoomItem("HATTER", gameItems["HATTER"]);
        rooms[8].addRoomItem("CAT", gameItems["CAT"]);
        }
};

int main(){
    char playAgain = 'y';
    int gameAttempts_ = 0;
    Game game;
    //Import the game intro text:
    TextFileReader gameIntro("gameIntro.txt");
    gameIntro.display();
    
    //The play again loop - runs until the player dies or wins.
    while (playAgain == 'y' || playAgain == 'Y'){
        game.gameLoop();
        
        // Player death loop and game reset:
        cout << "[QUEEN OF HEARTS]: YOU FAILED! WISH TO TRY AGAIN??" << endl;
        cin >> playAgain;
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        if(playAgain == 'y' || playAgain == 'Y'){
            cout << "Game reset function" << endl;
            game.resetGame();
            gameAttempts_++;
        } 
     }

    // cout << "Your score was: " << gameAttempts_ << endl;
    cout << "Thank you for playing. You attempted the game: "<< gameAttempts_ << " times." << endl;
    cout << "Imagine if you had only 1 chance..." << endl;
    cout << "Good bye!" << endl;
     
   return 0;
}
