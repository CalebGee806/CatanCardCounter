/**
  ___    __   ____   __    _  _
 / __)  /__\ (_  _) /__\  ( \( )
( (__  /(__)\  )(  /(__)\  )  (
 \___)(__)(__)(__)(__)(__)(_)\_)

  ___    __    ____  ____
 / __)  /__\  (  _ \(  _ \
( (__  /(__)\  )   / )(_) )
 \___)(__)(__)(_)\_)(____/

  ___  _____  __  __  _  _  ____  ____  ____
 / __)(  _  )(  )(  )( \( )(_  _)( ___)(  _ \
( (__  )(_)(  )(__)(  )  (   )(   )__)  )   /
 \___)(_____)(______)(_)\_) (__) (____)(_)\_)

 */
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct City {
    vector<int> location;
    City();
};
City::City() : location(3) {}

struct Settlement {
    vector<int> location;
    Settlement();
};
Settlement::Settlement() : location(3) {}

struct Player {
    vector<int> Cards;
    vector<City> Cities;
    vector<Settlement> Settlements;
    Player();
};
Player::Player() : Cards(7) {}

struct Tile {
    int resourceType;
    int prosperity;
    Tile();
};
Tile::Tile() : resourceType(0), prosperity(0) {}

struct Board {
    vector<Tile> Tiles;
    Board();
};
Board::Board() : Tiles(20) {}

void printCardFunction(vector<Player> const &p) {
    size_t i;
    for (i = 0; i < p.size(); i++) {
        cout << "Player " << i + 1 << ":\n ";
        cout << "wheat: " << p[i].Cards[1] << endl;
        cout << "sheep: " << p[i].Cards[2] << endl;
        cout << "logs: " << p[i].Cards[3] << endl;
        cout << "ore: " << p[i].Cards[4] << endl;
        cout << "brick: " << p[i].Cards[5] << endl;
    }
}
void printBuildings(vector<Player> const &p){
    size_t i;
    for(i=0; i < p.size(); i++){
        cout << "Player " << i + 1 << ":\n ";
        cout << "Settlements: " << p[i].Settlements.size() << endl;
        cout << "Cities: " << p[i].Cities.size() << endl;
    }
}

vector<int> parseIntCSV(const string &csvInput, char delimiter) {
    vector<int> result;
    istringstream ss(csvInput);
    string token;

    while (getline(ss, token, delimiter)) {
        result.push_back(stoi(token)); // Convert string to integer and add to vector
    }

    return result;
}

vector<Player> placePlayerPieces(int numPlayers) {
    vector<Player> PlayerVector;
    PlayerVector.resize(numPlayers);

    size_t i;
    for (i = 0; i < numPlayers; i++) {
        cout << "Enter a comma-separated list of Player " << i + 1 << "'s first settlement tile touching: (if water, type 19) ";
        string input;
        char comma = ',';
        getline(cin, input);
        vector<int> csvIntegers = parseIntCSV(input, comma);

        Settlement settlement;
        settlement.location = {csvIntegers[0], csvIntegers[1], csvIntegers[2]};
        PlayerVector[i].Settlements.push_back(settlement);

        cout << "Enter a comma-separated list of Player " << i + 1 << "'s second settlement tile touching: (if water, type 19)";
        string SecondInput;
        getline(cin, SecondInput);
        vector<int> SecondcsvIntegers = parseIntCSV(SecondInput, comma);

        settlement.location = {SecondcsvIntegers[0], SecondcsvIntegers[1], SecondcsvIntegers[2]};
        PlayerVector[i].Settlements.push_back(settlement);
    }
    return PlayerVector;
}

void distributeInitialCards(Board const &b, vector<Player> &p) {
    size_t i;
    for (i = 0; i < p.size(); i++) {
        size_t j;
        for (j = 0; j < 2; j++) {
            size_t k;
            for (k = 0; k < 3; k++) {
                int tileToAdd = p[i].Settlements[j].location[k];
                p[i].Cards[b.Tiles[tileToAdd].resourceType] += 1;  // Use resourceType as an index
            }
        }
    }
}

vector<Player> setUpBoard(int numPlayers, Board &Gameboard) {
    cout << "1=wheat\n"
         << "2=sheep\n"
         << "3=logs\n"
         << "4=ore\n"
         << "5=bricks\n"
         << "6=desert\n";

    cout << "Enter a comma-separated list of the resource type of the tiles in order of a snake: ";
    string input;
    char comma = ',';
    getline(cin, input);
    vector<int> csvIntegers = parseIntCSV(input, comma);

    size_t i;
    for (i = 0; i < 19; i++) {
        Gameboard.Tiles[i].resourceType = csvIntegers[i];
    }

    cout << "Enter a comma-separated list of the prosperity of the tiles in order of a snake: ";
    string SecondInput;
    getline(cin, SecondInput);
    vector<int> SecondCSVintegers = parseIntCSV(SecondInput, comma);
    size_t j;
    for (j = 0; j < 19; j++) {
        Gameboard.Tiles[j].prosperity = SecondCSVintegers[j];
    }
    Gameboard.Tiles[19].resourceType = 7;
    Gameboard.Tiles[19].prosperity = 0;

    vector<Player> PlayerVector = placePlayerPieces(numPlayers);
    distributeInitialCards(Gameboard, PlayerVector);
    return PlayerVector;
}

vector<Player> initialization(Board &Gameboard) {
    int numPlayers;
    cout << "3 or 4 players?\n";
    cin >> numPlayers;
    cin.ignore(); // Clear the newline character from the input buffer
    vector<Player> PlayerVector = setUpBoard(numPlayers, Gameboard);
    return PlayerVector;
}

void resourceRoll(vector<Player>& p, Board b, int robberTile){
    cout << "What number was rolled? ";
    int numRolled;
    cin >> (numRolled);
    size_t i;
    for (i = 0; i < p.size(); i++) {
        size_t j;
        for (j = 0; j < p[i].Settlements.size(); j++) {
            size_t k;
            for (k = 0; k < 3; k++) {
                int tileToAdd = p[i].Settlements[j].location[k];
                if (numRolled == b.Tiles[tileToAdd].prosperity && tileToAdd != robberTile) {
                    p[i].Cards[b.Tiles[tileToAdd].resourceType] += 1;
                }
            }
        }
    }
    for (i = 0; i < p.size(); i++) {
        size_t j;
        for (j = 0; j < p[i].Cities.size(); j++) {
            size_t k;
            for (k = 0; k < 3; k++) {
                int tileToAdd = p[i].Cities[j].location[k];
                if (numRolled == b.Tiles[tileToAdd].prosperity && tileToAdd != robberTile) {
                    p[i].Cards[b.Tiles[tileToAdd].resourceType] += 2;
                }
            }
        }
    }
}

void tradeWithPersonOrHarbour(vector<Player>& p){
    cout << "Which player is trading?";
    int trader;
    cin >> (trader);
    cout << "Which player is recieving the trade?";
    int tradee;
    cin >> (tradee);
    cout << "what material is the trader recieving?";
    int TradermaterialRecieving;
    cin >> (TradermaterialRecieving);
    cout << "how many?";
    int traderQuantity;
    cin >> (traderQuantity);
    cout << "what material is the tradee recieving?";
    int TradeematerialRecieving;
    cin >> (TradeematerialRecieving);
    cout << "how many?";
    int tradeeQuantity;
    cin >> (tradeeQuantity);
    p[trader-1].Cards[TradermaterialRecieving] += traderQuantity;
    p[trader-1].Cards[TradeematerialRecieving] -= tradeeQuantity;
    p[tradee-1].Cards[TradeematerialRecieving] += tradeeQuantity;
    p[tradee-1].Cards[TradermaterialRecieving] -= traderQuantity;
}
void purchase(vector<Player>& p){
    string input;
    char comma = ',';
    vector<int> csvIntegers;
    City city;
    Settlement settlement;
    cout << "Which player purchased something?\n";
    int purchaser;
    cin >> (purchaser);
    cout << "What was purchased?";
    char purchased;
    cout << "D = development card\n" <<
    "R = road\n" <<
    "C = City\n" <<
    "S = Settlement\n";
    cin >> (purchased);
    switch (purchased) {
        case 'D':
            p[purchaser-1].Cards[1] += -1;
            p[purchaser-1].Cards[2] += -1;
            p[purchaser-1].Cards[4] += -1;

            break;
        case 'R':
            p[purchaser-1].Cards[3] += -1;
            p[purchaser-1].Cards[5] += -1;

            break;
        case 'C':
            p[purchaser-1].Cards[1] -= 2;
            p[purchaser-1].Cards[4] -= 3;
            cout << "Enter a comma-separated list of Player " << purchaser << "'s new City tile touching: (if water, type 19) ";
            cin.ignore();
            getline(cin, input);
            csvIntegers = parseIntCSV(input, comma);
            city.location = {csvIntegers[0], csvIntegers[1], csvIntegers[2]};
            p[purchaser-1].Cities.push_back(city);
            size_t i;
            for(i=0;i<p[purchaser-1].Settlements.size();i++){
                if(p[purchaser-1].Settlements[i].location[0] == csvIntegers[0] && p[purchaser-1].Settlements[i].location[1] == csvIntegers[1] && p[purchaser-1].Settlements[i].location[2] == csvIntegers[2]){
                    p[purchaser-1].Settlements.erase(p[purchaser-1].Settlements.begin() + i);
                }
            }

            break;
        case 'S':
            p[purchaser-1].Cards[1] += -1;
            p[purchaser-1].Cards[2] += -1;
            p[purchaser-1].Cards[3] += -1;
            p[purchaser-1].Cards[5] += -1;
            cout << "Enter a comma-separated list of Player " << purchaser << "'s new settlement tile touching: (if water, type 19) ";
            cin.ignore();
            getline(cin, input);
            csvIntegers = parseIntCSV(input, comma);
            settlement.location = {csvIntegers[0], csvIntegers[1], csvIntegers[2]};
            p[purchaser-1].Settlements.push_back(settlement);

            break;
        default:
         break;
    }

}

void monopoly(vector<Player>& p){
    cout << "Which player is playing monopoly?";
    int monopolizer;
    cin >> (monopolizer);
    cout << "Which material are the monopolizing";
    int monopolyMaterial;
    cin >> (monopolyMaterial);
    size_t i;
    for(i=0, i < p.size(); i++;){
        if ((monopolizer-1) != i) {
            p[monopolizer - 1].Cards[monopolyMaterial] += p[i].Cards[monopolyMaterial];
            p[i].Cards[monopolyMaterial] = 0;
        }
    }
}

void yearOfPlenty(vector<Player>& p){
    cout << "Which player is playing Year of Plenty?";
    int PlentyPlayer;
    cin >> (PlentyPlayer);
    cout << "Material 1:";
    int materialOne;
    cin >> (materialOne);
    cout << "Material 2:";
    int materialTwo;
    cin >> (materialTwo);
    p[PlentyPlayer-1].Cards[materialOne] += 1;
    p[PlentyPlayer-1].Cards[materialTwo] += 1;
}
void tradeWithHarbour(vector<Player> p){
    cout << "Which player is trading?";
    int trader;
    cin >> (trader);
    cout << "What are they trading?";
    int tradingMaterial;
    cin >> (tradingMaterial);
    cout << "How many?";
    int MaterialCount;
    cin >> (MaterialCount);
    cout << "What material are they recieving?";
    int receivingMaterial;
    cin >> (receivingMaterial);
    p[trader-1].Cards[receivingMaterial] += 1;
    p[trader-1].Cards[tradingMaterial] -= MaterialCount;
}

void rolledASeven(vector<Player>& p, Board b, int robberTile){
    size_t i;
    string input;
    char comma = ',';
    vector<int> csvIntegers;
    for(i=0; i < p.size(); i++){
        if(p[i].Cards[1]+p[i].Cards[2]+p[i].Cards[3]+p[i].Cards[4]+p[i].Cards[5] > 7){
            cout << "Enter a Comma seperated File of the cards Player " << i+1 <<" has chosen to remove:";
            cin.ignore();
            getline(cin, input);
            csvIntegers = parseIntCSV(input, comma);
            size_t j;
            for(j=1;j < 6; j++){
                p[i].Cards[j] -= csvIntegers[j-1];
            }
        }
    }
    cout << "What vector is the robber on?";
    cin >> (robberTile);
    //TODO: IMPLEMENT STEALING FUNCTION
}
void knightCard(vector<Player>& p, Board b, int robberTile){
    cout << "What vector is the robber on?";
    cin >> (robberTile);
    //TODO: IMPLEMENT STEALING FUNCTION
}

void menu(vector<Player>& PlayerVector, const Board& Gameboard){
    int robberTile;
    robberTile = 0;
    cout << "Select an option:\n";
    cout << "Resource Roll: (A)\n" <<
            "Trade with Person: (B)\n" <<
            "Purchase: (C)\n" <<
            "Rolled a seven: (D)\n" <<
            "Knight Card: (E)\n" <<
            "Year of Plenty Development Card: (F)\n" <<
            "Monopoly Development Card: (G)\n" <<
            "Print all cards: (H)\n" <<
            "Print all settlements/cities: (I)\n" <<
            "Trade with Harbour: (J)";
    char select;
    cin >> (select);
    switch (select) {
        case 'A':
            resourceRoll(PlayerVector, Gameboard, robberTile);
            break;
        case 'B':
            tradeWithPersonOrHarbour(PlayerVector);
            break;
        case 'C':
            purchase(PlayerVector);
            break;
        case 'D':
            rolledASeven(PlayerVector, Gameboard, robberTile);
            break;
        case 'E':
            knightCard(PlayerVector, Gameboard, robberTile);
            break;
        case 'F':
            yearOfPlenty(PlayerVector);
            break;
        case 'G':
            monopoly(PlayerVector);
       case 'H':
           printCardFunction(PlayerVector);
           break;
        case 'I':
            printBuildings(PlayerVector);
            break;
        case 'J':
            tradeWithHarbour(PlayerVector);
        default:
            break;
    }

}
int main() {
    Board Gameboard;
    vector<Player> PlayerVector = initialization(Gameboard);
    printCardFunction(PlayerVector);
    while(true){
        menu(PlayerVector, Gameboard);
    }
}