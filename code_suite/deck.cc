#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <deque>        
#include <queue> 
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>   
    // std::chrono::system_clock

class Player;
class Deck;
class Card;
class straightCard;
class Strategy;
class humanStrategy;
class computerStrategy;


class Card {
    public:
    int x;
    char num;
    char suite;
    virtual void printCard() = 0;
    void copy(Card &s) {
        this->x = s.x;
        this->num = s.num;
        this->suite = s.suite;
    };
    virtual ~Card() { };
};

class straightCard : public Card {
    public:
    int x;
    char num;
    char suite;
    straightCard(int i = 0, char suite = 'N');
    void printCard();
    void copy(straightCard &s) {
       this->x = s.x;
       this->num = s.num;
       this->suite = s.suite;
    }
    int numCard();
    ~straightCard() { };
};


void copyV(std::vector<straightCard> &s, std::vector<straightCard> &t) {
    for(auto n : t) {
        s.emplace_back(n);
    }
}



int straightCard::numCard() {
    return this->x;
}

straightCard::straightCard(int i, char suite ){
    if (i == 1) {
        this->num = 'A';
    }
    else if ( i < 10 && i >= 2 ) {
        this->num = '0' + i;
    }
    else if ( i == 10) {
        this->num = 'T';
    }
    else if ( i == 11) {
        this->num = 'J';
    }
    else if ( i == 12) {
        this->num = 'Q';
    }
    else if ( i == 13) {
        this->num = 'K';
    }
    else this->num = 0;
    this->x = i;
    this->suite = suite;
}

void straightCard::printCard() {
    std::cout << this->num << this->suite;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////



class Strategy {
    public:
    Strategy() {};
    virtual void move(Deck &deck, Player &player, std::vector<straightCard> validCards) = 0;
    ~Strategy() {};
};

class computerStrategy: public Strategy {
    public:
    computerStrategy() {};
    void move(Deck &deck, Player &player, std::vector<straightCard> validCards);
};

class humanStrategy: public Strategy {
    public:
    humanStrategy() {};
    void move(Deck &deck, Player &player, std::vector<straightCard> validCards);
};

class Player {
    public:
    std::vector<straightCard> hands;
    std::vector<straightCard> discards;
    std::shared_ptr<Strategy> strategy;
    int rank;
    int oldscore;
    bool ifComp;
    Player(std::vector<straightCard> dilivery, std::shared_ptr<Strategy> strategy, bool ifComp, int rank);
    void setStrategy(Strategy &strategy);
    void move(Deck &deck);
    void printDiscards();
    void printHands();
    int score();
    int newScore();
    void newGame(std::vector<straightCard> dilivery);
    friend class Deck;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Deck {
    public:
    int count;
    int seed;
    int now;
    bool roundEnd;
    bool gameEnd;
    std::vector<std::shared_ptr<Player>> player;
    std::vector<straightCard> full;
    std::vector<straightCard> shuffled;
    std::vector<int> order;
    std::vector<straightCard> club;
    std::vector<straightCard> diamond;
    std::vector<straightCard> heart;
    std::vector<straightCard> spade;
    std::shared_ptr<Strategy> hStrategy;
    std::shared_ptr<Strategy> cStrategy;
    Deck(int seed, std::shared_ptr<Strategy> hStrategy, std::shared_ptr<Strategy> cStrategy);
    void printDeck();
    void printFullDeck();
    void playerMove(int index);
    void newGame();
    void playCard(straightCard indexCard);
    void checkEnd();
    //void default_deck();
};

std::vector<straightCard> newCards() {
    std::vector<straightCard> temp;
    for (int i = 0; i < 4; i++) {
        for ( int j = 1; j <= 13; j++ ) {
            if (i == 0) {
            temp.emplace_back(straightCard(j, 'C'));
           }
           if (i == 1) {
            temp.emplace_back(straightCard(j, 'D'));
           }
           if (i == 2) {
            temp.emplace_back(straightCard(j, 'H'));
           }
           if (i == 3) {
            temp.emplace_back(straightCard(j, 'S'));
           }
        }
    }
    return temp;
}

void Deck::newGame() {
    //char tempIf = 0;
    //int r  = 0;
    int spade7 = 0;
    this->seed += 1;
    std::shared_ptr<Player> tempPlayer ;
    this->roundEnd = false;
    this->gameEnd = false;
    std::vector<std::shared_ptr<Player>> q;
    this->player = q;
    this->order.clear();
    this->shuffled.clear();
    this->club.clear();
    this->diamond.clear();
    this->heart.clear();
    this->spade.clear();
 //   this->default_deck();

    for (int i = 0; i < 52; i++) {
        this->order.emplace_back(i);
    }

    std::shuffle(this->order.begin(), this->order.end(), std::default_random_engine(seed));
    for (auto n : order) {
        this->shuffled.emplace_back(full[n]);
    }

    for (auto n : this->player) {
        if (n->rank == 1) {
            n->newGame({this->shuffled.begin(), this->shuffled.begin() + 12});
        }
        else if (n->rank == 2) {
            n->newGame({this->shuffled.begin()+13, this->shuffled.begin() + 25});
        }
        else if (n->rank == 3) {
            n->newGame({this->shuffled.begin()+26, this->shuffled.begin() + 38});
        }
        else if (n->rank == 4) {
            n->newGame({this->shuffled.begin() + 39, this->shuffled.begin() + 51});
        }
    }

    for(auto n : this->shuffled) {
        if (n.num == '7' && n.suite == 'S') break;
        else spade7++;
    }
    this->now = spade7;
}

void Deck::playCard(straightCard indexCard) {
    if (indexCard.suite == 'C') {
        if (this->club.size() == 0) {
            this->club.emplace_back(indexCard);
        }
        else if (indexCard.x == this->club[0].x - 1) {
            this->club.emplace(this->club.begin(), indexCard);
        }
        else {
            this->club.emplace_back(indexCard);
        }
    }
    else if (indexCard.suite == 'D') {
        if (this->diamond.size() == 0) {
            this->diamond.emplace_back(indexCard);
        }
        else if (indexCard.x == this->diamond[0].x - 1) {
            this->diamond.emplace(this->diamond.begin(), indexCard);
        }
        else {
            this->diamond.emplace_back(indexCard);
        }
    }
    else if (indexCard.suite == 'H') {
        if (this->heart.size() == 0) {
            this->heart.emplace_back(indexCard);
        }
        else if (indexCard.x == this->heart[0].x - 1) {
            this->heart.emplace(this->heart.begin(), indexCard);
        }
        else {
            this->heart.emplace_back(indexCard);
        }
    }
    else if (indexCard.suite == 'S') {
        if (this->spade.size() == 0) {
            this->spade.emplace_back(indexCard);
        }
        else if (indexCard.x == this->spade[0].x - 1) {
            this->spade.emplace(this->spade.begin(), indexCard);
        }
        else {
            this->spade.emplace_back(indexCard);
        }
    }
}
/*
void Deck::default_deck() {
    straightCard tempCard = straightCard(0, 'N');
     this->club.clear();
        this->diamond.clear();
        this->heart.clear();
        this->spade.clear();
    for (int i = 0; i < 13; i++) {
        this->club.emplace_back(tempCard);
        this->diamond.emplace_back(tempCard);
        this->heart.emplace_back(tempCard);
        this->spade.emplace_back(tempCard);

    }
}*/

Deck::Deck(int seed, std::shared_ptr<Strategy> hStrategy, std::shared_ptr<Strategy> cStrategy) {
    char tempIf = 0;
    int spade7 = 0;
    //int count = 0;
    bool okCommand = false;
    std::shared_ptr<Player> tempPlayer;
    this->seed = seed;
    this->full = newCards();
    this->shuffled.clear();
    this->hStrategy = hStrategy;
    this->cStrategy = cStrategy;
    this->roundEnd = false;
    this->gameEnd = false;
    //this->default_deck();
    for (int i = 0; i < 52; i++) {
        this->order.emplace_back(i);
    }

    std::shuffle(this->order.begin(), this->order.end(), std::default_random_engine(seed));

    for (int i = 0; i < 52; i++) {
        this->shuffled.emplace_back(full[this->order[i]]);
    }
/*
    for (auto n: this->shuffled) {
        n.printCard();
        std::cout << std::endl;
    }*/

    for(auto n : this->shuffled) {
        if (n.num == '7' && n.suite == 'S') break;
        else spade7++;
    }

    std::cout << "Is Player1 a human (h) or a computer (c)?" << std::endl;
         while(okCommand == false) {
    std::cin >> tempIf;
    if ( tempIf == 'h') {
        this->player.emplace_back(std::make_shared<Player>(Player({this->shuffled.begin(), this->shuffled.begin() + 13}, hStrategy, false, 1)));
    okCommand = true;
    }
    else if ( tempIf == 'c') {
        this->player.emplace_back(std::make_shared<Player>(Player({this->shuffled.begin(), this->shuffled.begin() + 13}, cStrategy, true, 1)));
   okCommand = true;
    }
    else {std::cout << "invalid input" << std::endl;}
     }
     okCommand = false;
    std::cout << "Is Player2 a human (h) or a computer (c)?" << std::endl;
        while(okCommand == false) {
    std::cin >> tempIf;
    
    if ( tempIf == 'h') {
        this->player.emplace_back(std::make_shared<Player>(Player({this->shuffled.begin()+13, this->shuffled.begin() + 26}, hStrategy, false, 2)));
    okCommand = true;
    }
    else if ( tempIf == 'c') {
        this->player.emplace_back(std::make_shared<Player>(Player({this->shuffled.begin() + 13, this->shuffled.begin() + 26}, cStrategy, true, 2)));
    okCommand = true;
    }
    else {std::cout << "invalid input" << std::endl;}
        }
        okCommand = false;
    std::cout << "Is Player3 a human (h) or a computer (c)?" << std::endl;
        while(okCommand == false) {
      std::cin >> tempIf;
    if ( tempIf == 'h') {
        this->player.emplace_back(std::make_shared<Player>(Player({this->shuffled.begin()+26, this->shuffled.begin() + 39}, hStrategy, false, 3)));
    okCommand = true;
    }
    else if ( tempIf == 'c') {
        this->player.emplace_back(std::make_shared<Player>(Player({this->shuffled.begin()+26, this->shuffled.begin() + 39}, cStrategy, true, 3)));
    okCommand = true;
    }
    else {std::cout << "invalid input" << std::endl;}
        }
        okCommand = false;
    std::cout << "Is Player4 a human (h) or a computer (c)?" << std::endl;
    
    while(okCommand == false) {
      std::cin >> tempIf;
      if ( tempIf == 'h') {
        this->player.emplace_back(std::make_shared<Player>(Player({this->shuffled.begin() + 39, this->shuffled.begin() + 52}, hStrategy, false, 4)));
        okCommand = true;
      }
      else if ( tempIf == 'c') {
        this->player.emplace_back(std::make_shared<Player>(Player({this->shuffled.begin() + 39, this->shuffled.begin() + 52}, cStrategy, true, 4)));
        okCommand = true;
      }
      else {std::cout << "invalid input" << std::endl;}
    }
    this->now = (spade7)/ 13 ;
    std::cout << "this is spade 7" << spade7 << std::endl;
}

void Deck::checkEnd() {
    int index = 0;
    int maxIndex = 0;
    int maxScore = 0;
    bool ifEnd = false;
    for (auto n: this->player) {
        //std::cout << "this is player" << n->rank << std::endl;
        if (n->score() + n->newScore() >= 80) {
          //  std::cout << "the score in checkEnd()" << n->score()<< "+" << n->newScore() << std::endl;
            if (maxScore < n->score() + n->newScore()) {
                maxIndex = index;
                maxScore = n->score() + n->newScore();
            }
            ifEnd = true;
        }
        index++;
    }
    if ( index < 4 && ifEnd) {
        std::cout<< "Player" << this->player[maxIndex]->rank << " wins!\n";
        this->gameEnd = true;
    }
}

void Deck::playerMove(int index) {
    std::cout << "Cards on the table:" << std::endl;
    this->printDeck();
    this->player[this->now]->move(*this);
    this->now = (this->now + 1) % 4;
}

void Deck::printFullDeck() {
    for(int i = 0; i < 52; i++ ) {
        if ( i != 0 && (i + 1) % 13 == 0 ) {
            this->full[i].printCard();
            std::cout << std::endl;
        }
        else {
            this->full[i].printCard();
            std::cout << " ";
        }
    }
}

void Deck::printDeck() {
    std::cout << "Clubs:";
    for(auto n : this->club) {
        if ( n.numCard() != 0){
            std::cout << " "<< n.numCard();
        }
    }
    std::cout << std::endl;
    std::cout << "Dimonds:";
    for(auto n : this->diamond) {
        if ( n.numCard() != 0){
            std::cout << " " << n.numCard();
        }
    }
    std::cout << std::endl;
    std::cout << "Hears:";
    for(auto n : this->heart) {
        if ( n.numCard() != 0){
            std::cout << " " << n.numCard();
        }
    }
    std::cout << std::endl;
    std::cout << "Spades:";
    for(auto n : this->spade) {
        if ( n.numCard() != 0){
            std::cout << " " << n.numCard();
        }
           
    }
     std::cout << std::endl;
}










////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




int Player::newScore() {
    int ans = 0;
   // std::cout << "size of discards" << this->discards.size() << std::endl;
    if (this->discards.size() == 0) {
        return 0;
    }
    for (auto n : this->discards) {
        ans += n.x;
    }
   // std::cout  << " and its ans = " << ans << std::endl;
    return ans;
}

//helper to find legal plays 
bool ifValid(straightCard card, Deck &deck) {
    bool ans = false;
    if (card.suite == 'C') {
        for (auto n : deck.club) {
            if (n.num == card.num - 1 || n.num == card.num + 1) {
                ans = true;
            }
        }
        if (card.x == 7)  ans = true;
    }
    else if (card.suite == 'D') {
        for (auto n : deck.diamond) {
            if (n.num == card.num - 1 || n.num == card.num + 1) {
                ans = true;
            }
        }
        if (card.x == 7)  ans = true;
    }
    else if (card.suite == 'H') {
        for (auto n : deck.heart) {
            if (n.num == card.num - 1 || n.num == card.num + 1) {
                ans = true;
            }
        }
        if (card.x == 7)  ans = true;
    }
    else if (card.suite == 'S') {
        for (auto n : deck.spade) {
            if (n.num == card.num - 1 || n.num == card.num + 1) {
                ans = true;
            }
        }
        if (card.x == 7)  ans = true;
    }
    return ans;
}

//helper to find the index of target card
int target(std::string tempCard, std::vector<straightCard> cards) {
    int index = 0;
    bool ifFind = false;
    std::string num;
    std::string suite;
    for (auto n : cards) {
        num = n.num;
        suite  = n.suite;
       // std::cout << "num and suite and temcard" << num <<  suite << tempCard << std::endl;
        if ( tempCard.compare(0,1, num) == 0 && tempCard.compare(1,1, suite) == 0) {
            ifFind = true;
            break;
        }
        index++;
    }
    if (ifFind) {
        return index;
    }
    else {
        return -1;
    }
}

bool validCard(std::string s) {
    std::string snum =  s.substr(0,1);
    std::string suite = s.substr(1,1);
    std::string tempnum;
    bool numAns;
    bool suiteAns;
    for(int i = 2; i <= 9 ; i++) {
        tempnum = '0' + i;
        if(tempnum.compare(snum)){
            numAns = true;
        }
    }
    if (snum.compare("T")==0 || snum.compare("A") ==0 || snum.compare("J")==0 ||snum.compare("Q")== 0
    || snum.compare("K")== 0){
        numAns = true;
    }
    if (suite.compare("C")==0 || suite.compare("D") ==0 || suite.compare("H")==0 ||suite.compare("S")== 0){
        suiteAns = true;
    }
    return numAns && suiteAns;
}

void humanStrategy::move(Deck &deck, Player &player, std::vector<straightCard> validCards) {
    std::string command;
    std::string tempCard;
    int tempIndex;
    int tempValid;
    bool okCommand = false;
    straightCard indexCard;
  while(okCommand == false) {
      std::cin >> command;
      if (command == "deck") {
        deck.printFullDeck();
        okCommand = true;
      }
    else if (command == "discard") {
        std::cin >> tempCard;
       // std::cout << tempCard<< std::endl;
        tempIndex = target(tempCard, player.hands);
        if (validCard(tempCard) == false ) {
            std::cout  << "this is an invalid card" << std::endl;
        }
        else if (validCards.size() > 0 || tempIndex < 0)  {
            tempIndex = target(tempCard, player.hands);
            std::cout << "You have a legal play. You may not discard." << std::endl;
        }
        else {
            tempIndex = target(tempCard, player.hands);
            std::cout << "Player " << player.rank << " discards " << tempCard << "." << std::endl;
            indexCard = (player.hands[tempIndex]);
            player.hands.erase(player.hands.begin() + tempIndex);
            player.discards.emplace_back(indexCard);
            okCommand = true;
        }
    }
    else if ( command == "play") {
        std::cin >> tempCard;
       // std::cout << tempCard<< std::endl;
        tempIndex = target(tempCard, player.hands);
        tempValid = target(tempCard, validCards);
        if( validCard(tempCard) == false )  {
            std::cout  << "this is invalid card" << std::endl;
        }
        else if (validCards.size() == 0 || tempIndex < 0 || tempValid < 0) {
            if (validCards.size() == 0 ) std::cout << "1 wrong"<< std::endl;
            if (tempIndex < 0 ) std::cout << "2 wrong"<< std::endl;
            if (tempValid < 0) std::cout << "3 wrong"<< std::endl;
            
            std::cout << "This is not a legal play." << std::endl;
        }
        else {
            std::cout << "Player" << player.rank << " plays " << tempCard << "." << std::endl;
            indexCard = (player.hands[tempIndex]);
            //std::cout << "1" << std::endl;
            player.hands.erase(player.hands.begin() + tempIndex);
           // std::cout << "2" << std::endl;
            deck.playCard(indexCard);
            //std::cout << "3" << std::endl;
            okCommand = true;
        }
    }
    else if (command == "ragequit") {
        deck.player[deck.now]->strategy = deck.cStrategy;
        std::cout << "Player" << player.rank <<  " ragequits. A computer will now take over.\n";
        player.move(deck);
        okCommand = true;
    }
    else if ( command == "quit") {
        deck.gameEnd = true;
        okCommand = true;
    }
    else {
        std::cout  << "this is invalid input" << std::endl;
    }

  }

    if ( player.hands.size() == 0) {
        deck.roundEnd = true;
    }
    deck.checkEnd();
}

void computerStrategy::move(Deck &deck, Player &player, std::vector<straightCard> validCards) {
    std::string command;
    std::string tempCard;
    std::cin >> command;
    //int tempIndex;
  //  int tempValid;
    straightCard indexCard(0, 'N');
    if ( validCards.size() == 0) {
        std::cout << "Player " << player.rank << " discards " << tempCard << "." << std::endl;
        indexCard = (player.hands[0]);
        player.hands.erase(player.hands.begin());
        player.discards.emplace_back(indexCard);
    }
    else if ( validCards.size() > 0) {
        std::cout << "Player" << player.rank << " plays " << tempCard << "." << std::endl;
            indexCard = (player.hands[0]);
            player.hands.erase(player.hands.begin());
            deck.playCard(indexCard);
    }
    if ( player.hands.size() == 0) {
        deck.roundEnd = true;
    }
    deck.checkEnd();
}

void Player::newGame(std::vector<straightCard> dilivery) {
    this->hands.clear();
    this->hands = dilivery;
    this->discards.clear();
    this->oldscore += this->newScore();
}
    

Player::Player(std::vector<straightCard> dilivery, std::shared_ptr<Strategy> strategy, bool ifComp, int rank) {
    this->rank = rank;
    this->hands = dilivery;
    this->strategy = strategy;
    this->oldscore = 0;
    this->discards.clear();
    this->ifComp = ifComp;
}

int Player::score() {
    return this->oldscore;
}

void Player::printHands() {
    for ( auto n : this->hands) {
        std::cout << " " ;
        n.printCard();
    }
}

void Player::printDiscards() {
    for ( auto n : this->discards) {
        std::cout << " ";
        n.printCard();
        std::cout << std::endl;
    }
}

void Player::move(Deck &deck) {
    std::vector<straightCard> validCards;
    std::cout << "Your hand:";
    this->printHands();
    std::cout << std::endl;
    for (auto n : this->hands) {
        if ( ifValid(n, deck)) {
            validCards.emplace_back(n);
        }
        if ( n.num == '7' && n.suite == 'S') {
            validCards.emplace_back(n);
        }
    }
    std::cout << "Legal plays:";
    for (auto n: validCards) {
        std::cout << " ";
        n.printCard();
    }
    std::cout << std::endl;
    this->strategy->move(deck, *this, validCards);
}











////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







