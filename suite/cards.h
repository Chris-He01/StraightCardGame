#ifndef DECK_H
#define DECK_H


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





#endif
