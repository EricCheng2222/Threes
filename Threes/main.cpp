//
//  main.cpp
//  Threes
//
//  Created by Eric Cheng on 9/30/18.
//  Copyright © 2018 Eric Cheng. All rights reserved.
//

#include <iostream>
#include <vector>
#include <random>
#include <chrono>       // std::chrono::system_clock
#include "Board.h"
#include "agent.h"
using namespace std;



int main(int argc, const char * argv[]) {
    int evilType = 0;
    int playerType = 1;
    int totalPlay = 1000;
    
    agent evil(evilType);
    agent player(playerType);
    Board b;
    for (int i=0; i<totalPlay; i++) {
        b.initialize(&evil, &player);
        evil.initialize();
        player.initialize();
        while (b.isFinish()==false) {
            agent *nowPlay = b.whoseTurn();
            int stat = b.playByAgent(nowPlay);
            if (stat==-1) break;
        }
        b.saveStatistic();
    }
    b.showStatistic(totalPlay);
    
    return 0;
}
