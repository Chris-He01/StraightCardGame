#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include "cards.h"


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




