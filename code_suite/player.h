#ifndef PLAYER_H
#define PLAYER_H
#include <memory>
#include <vector>
#include "deck.h"



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


#endif
