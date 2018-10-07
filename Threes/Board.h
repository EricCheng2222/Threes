//
//  Board.h
//  Threes
//
//  Created by Eric Cheng on 10/1/18.
//  Copyright © 2018 Eric Cheng. All rights reserved.
//

#ifndef Board_h
#define Board_h
#include <vector>
#include <chrono>
using namespace std;
#include "agent.h"

struct statistic {
    statistic();
    statistic(int LT, int s, double opsP, double opsE){
        largestTile = LT;
        score = s;
        opsPlayer = opsP;
        opsEvil = opsE;
    }
    int largestTile = 0;
    int score = 0;
    double opsPlayer;
    double opsEvil;
};

class Board{
public:
    
    void initialize(agent* x, agent* y){
        evil = x;
        player = y;
        playStep = 0;
        status.clear();
        status.resize(4);
        for (int i=0; i<4; i++) {
            for(int j=0; j<4; j++){
                status[i].push_back(0);
            }
        }
        auto start = chrono::high_resolution_clock::now();
        auto dur = start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur.time_since_epoch()).count();
        long long int dummyTime = (long long)ms;
        
        FILE *fp = fopen("/Users/eric/Documents/AI/ComputerGamingTheory/Threes/ans.txt", "a");
        fprintf(fp, "dummy:random@%lld|", dummyTime);
        fclose(fp);
    }
    bool isFinish(){
        return false;//temp
    }
    agent* whoseTurn(){
        if (playStep<9 || playStep%2==0) return evil;
        return player;
    }
    int playByAgent(agent *a){
        playStep++;
        int rtnVal = a->action(status, player->actionTaken);
        return rtnVal;
    }
    void showStatistic(int total){
        
        int countT[16] = {0};
        int score = 0;
        int maxScore = 0;
        
        double playerTimeSum = 0;
        double evilTimeSum = 0;
        
        for(auto stat : Statistic){
            countT[stat.largestTile]++;
            score += stat.score;
            playerTimeSum += stat.opsPlayer;
            evilTimeSum += stat.opsEvil;
            if(maxScore < stat.score) maxScore = stat.score;
        }
        printf("1000\t");
        printf("avg = %d, ", score/total);
        printf("max = %d, ", maxScore);
        printf("ops = %lf (%lf|%lf)\n", (playerTimeSum+evilTimeSum)/(2*total), playerTimeSum/total, evilTimeSum/total);
        
        

        
        int sumUp[16] = {0};
        for (int i=0; i<16; i++) {
            if (countT[i]==0) continue;
            for (int j=i; j<16; j++) {
                sumUp[i] += countT[j];
            }
        }
        for (int i=0; i<16; i++) {
            int tile;
            if (sumUp[i]!=0) {
                if(i<=3) tile = i;
                else tile = 3*pow(2, i-3);
                printf("\t%d", tile);
                printf("\t%.1f%%", (float)sumUp[i]*100/float(total));
                printf("\t(%.1f%%)\n", (float)(sumUp[i]-sumUp[i+1])*100/float(total));
            }
        }
        
        
    }
    void saveStatistic(){
        int maxT = 0;
        for (int i=0; i<4; i++) {
            for(int j=0; j<4; j++){
                if(maxT < status[i][j]) maxT = status[i][j];
            }
        }
        statistic tmp(maxT, player->getScore(), player->getTime(), evil->getTime());
        Statistic.push_back(tmp);
        auto start = chrono::high_resolution_clock::now();
        auto dur = start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur.time_since_epoch()).count();
        long long int dummyTime = (long long)ms;
        FILE *fp = fopen("/Users/eric/Documents/AI/ComputerGamingTheory/Threes/ans.txt", "a");
        fprintf(fp, "|dummy:random@%lld\n", dummyTime);
        fclose(fp);
    }
    
private:
    agent* evil;
    agent* player;
    int playStep;
    vector<vector<int> > status;
    vector<statistic> Statistic;
};

#endif /* Board_h */
