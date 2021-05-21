#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include "player.h"


void humanStrategy::move(Deck &deck, Player &player, std::vector<straightCard> validCards) {
    std::string command;
    std::string tempCard;
    int tempIndex;
    int tempValid;
    bool okCommand = false;
    straightCard indexCard;
    //std::cout << "in humman s" << std::endl;
  while(okCommand == false) {
      std::cin >> command;
      if (command == "deck") {
        deck.printFullDeck();
      }
    else if (command == "discard") {
        std::cin >> tempCard;
       // std::cout << tempCard<< std::endl;
        tempIndex = target(tempCard, player.hands);
        if (validCard(tempCard) == false || tempIndex < 0) {
            std::cout  << "this is an invalid card" << std::endl;
        }
        else if (validCards.size() > 0 )  {
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
        if( validCard(tempCard) == false || tempIndex < 0 )  {
            std::cout  << "this is a invalid card" << std::endl;
        }
        else if (validCards.size() == 0 || tempValid < 0) {
           // if (validCards.size() == 0 ) std::cout << "1 wrong"<< std::endl;
           // if (tempIndex < 0 ) std::cout << "2 wrong"<< std::endl;
            //if (tempValid < 0) std::cout << "3 wrong"<< std::endl;
            
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
        player.strategy->move(deck, player, validCards);
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
    std::string tempString;
    int tempIndex;
    int tempValid;
    straightCard indexCard(0, 'N');
   // std::cout << "there is no valid11" << std::endl;
    if ( validCards.size() == 0) {
     //   std::cout << "there is no valid" << std::endl;
        std::cout << "Player " << player.rank << " discards " << player.hands[0].num << player.hands[0].suite << "." << std::endl;
        indexCard = (player.hands[0]);
        player.hands.erase(player.hands.begin());
        player.discards.emplace_back(indexCard);
    }
    else if ( validCards.size() > 0) {
       // std::cout << "there is valid" << std::endl;
        std::cout << "Player" << player.rank << " plays " <<  validCards[0].num << validCards[0].suite << "." << std::endl;
        tempString = validCards[0].num;
        tempCard.append(tempString);
        tempString = validCards[0].suite;
        tempCard.append(tempString);
        tempIndex = target(tempCard, player.hands);
        indexCard = (player.hands[tempIndex]);
        player.hands.erase(player.hands.begin() + tempIndex);
        deck.playCard(indexCard);
    }
    deck.checkEnd();
}

void Player::newGame(std::vector<straightCard> dilivery) {
     this->oldscore += this->newScore();
    this->hands.clear();
    this->hands = dilivery;
    this->discards.clear();
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
    }
    std::cout << std::endl;
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









