//
//  agent.h
//  Threes
//
//  Created by Eric Cheng on 10/1/18.
//  Copyright © 2018 Eric Cheng. All rights reserved.
//

#ifndef agent_h
#define agent_h
#include <chrono>
#include <algorithm>
#include <random>

#include "Board.h"
#include "boardUtilOperation.h"
using namespace std;
default_random_engine generator;
uniform_int_distribution<int> distributionEmptySpace(0,15);
uniform_int_distribution<int> actionSpace(0,3);



class agent{
public:
    int actionTaken = -1;//left-->0 down-->1 right-->2 up-->3 for player only

    agent(int t){
        type = t;
    }
    
    int getScore(){
        return point;
    }
    double getTime(){
        return time;
    }
    void initialize(){
        point = 0;
        actionTaken = -1;
        reinitSpace();
        reinitBag();
        time = 0;
    }
    int action(vector<vector<int> > &status, int playerActionTaken = -1){
        if (type==0) {//evil
            auto begin = chrono::high_resolution_clock::now();
            
            
            vector<int> candSpace;
            switch (playerActionTaken) {
                case 0:
                    candSpace = slideLeftSpace;
                    break;
                case 1:
                    candSpace = slideDownSpace;
                    break;
                case 2:
                    candSpace = slideRightSpace;
                    break;
                case 3:
                    candSpace = slideUpSpace;
                    break;
                default:
                    candSpace = space;
                    break;
            }
            int i = 0;
            int tile = candSpace[i++];
            while (status[tile/4][tile%4]!=0) {
                if (i==candSpace.size()) return -1;
                tile = candSpace[i++];
            }
            if(bag.empty()==true) reinitBag();
            status[tile/4][tile%4] = bag.back();
            bag.pop_back();
            
            auto end = chrono::high_resolution_clock::now();
            auto dur = end - begin;
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
            long long dms = (long long)ms;
            
            char c = '0';
            if(0<=tile && tile<=9) c += tile;
            else if (10<=tile && tile<=15) c+=(7+tile);
            
            FILE *fp = fopen("ans.txt", "a");
            //FILE *fp = fopen("/Users/eric/Documents/AI/ComputerGamingTheory/Threes/ans.txt", "a");

            fprintf(fp, "%c%d", c, status[tile/4][tile%4]);
            if(dms!=0) fprintf(fp, "(%lld)", dms);
            fclose(fp);
        }
        else if(type==1){//player
            auto begin = chrono::high_resolution_clock::now();

            reinitAction();
            actionTaken = actionL.back();
            actionL.pop_back();
            int score;
            while ((score=slide(status))==-1) {
                if(actionL.empty()) return -1;
                actionTaken = actionL.back();
                actionL.pop_back();
            }
            point += score;
            
            auto end = chrono::high_resolution_clock::now();
            auto dur = end - begin;
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
            long long dms = (long long)ms;
            
            
            char c;
            char sl = '#';
            switch (actionTaken) {
                case 0:
                    c = 'L';
                    break;
                case 1:
                    c = 'D';
                    break;
                case 2:
                    c = 'R';
                    break;
                case 3:
                    c = 'U';
                    break;
            }
            FILE *fp = fopen("/Users/eric/Documents/AI/ComputerGamingTheory/Threes/ans.txt", "a");
            fprintf(fp, "%c%c[%d]", sl, c, score);
            if(dms!=0) fprintf(fp, "(%lld)", dms);
            fclose(fp);


        }
        return 0;
    }
private:
    int type;
    int point; // player only
    vector<int> bag; //evil only
    vector<int> space;//evil only
    vector<int> slideLeftSpace;
    vector<int> slideRightSpace;
    vector<int> slideUpSpace;
    vector<int> slideDownSpace;

    vector<int> actionL; //player only
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    double time;
    
    
    
    
    void reinitSpace(){
        space.clear();
        slideLeftSpace.clear();
        slideRightSpace.clear();
        slideUpSpace.clear();
        slideDownSpace.clear();
        for(int i=0; i<16; i++) if(i%4==0) slideRightSpace.push_back(i);
        for(int i=0; i<16; i++) if(i<=3) slideDownSpace.push_back(i);
        for(int i=0; i<16; i++) if(i%4==3) slideLeftSpace.push_back(i);
        for(int i=0; i<16; i++) if(i>=12) slideUpSpace.push_back(i);
        
        
        for(int i=0; i<16; i++) space.push_back(i);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle (space.begin(), space.end(), default_random_engine(seed));
        shuffle (slideRightSpace.begin(), slideRightSpace.end(), default_random_engine(seed));
        shuffle (slideDownSpace.begin(), slideDownSpace.end(), default_random_engine(seed));
        shuffle (slideLeftSpace.begin(), slideLeftSpace.end(), default_random_engine(seed));
        shuffle (slideUpSpace.begin(), slideUpSpace.end(), default_random_engine(seed));

    }
    void reinitAction(){
        for(int i=0; i<4; i++) actionL.push_back(i);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle (actionL.begin(), actionL.end(), default_random_engine(seed));
    }
    void reinitBag(){
        bag.clear();
        bag.push_back(1);
        bag.push_back(2);
        bag.push_back(3);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle (bag.begin(), bag.end(), default_random_engine(seed));
    }
    int slideLeft(vector<vector<int> > &status){
        vector<vector<int> > prev = status;
        int score = 0;
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (c!=3 && status[r][c]==0) {//empty left --> directly shift
                    status[r][c] = status[r][c+1];
                    status[r][c+1] = 0;
                    continue;
                }
                if (c!=3 && ((status[r][c]==1&&status[r][c+1]==2) || (status[r][c]==2&&status[r][c+1]==1))) {//1&&2 merge
                    status[r][c] = 3;
                    status[r][c+1] = 0;
                    score += 3;
                    continue;
                }
                if(c!=3 && status[r][c]>=3 && status[r][c]==status[r][c+1]){//same status merge but not 1/2
                    status[r][c] = status[r][c] + 1;
                    status[r][c+1] = 0;
                    score += pow(3, status[r][c]-2) - 2*pow(3, status[r][c]-3);
                    continue;
                }
                
            }
        }
        
        /*
        for(int i=0; i<4; i++){
            for (int j=0; j<4; j++) {
                printf("%d ", prev[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        for(int i=0; i<4; i++){
            for (int j=0; j<4; j++) {
                printf("%d ", status[i][j]);
            }
            printf("\n");
        }
        printf("\n\n\n\n");
        */
        if (prev!=status) {
            return score;
        }
        return -1;
    }
    int slide(vector<vector<int> > &status){
        int score;
        switch (actionTaken) {
            case 0:
                score = slideLeft(status);
                break;
            case 1:
                rotate_right(status);
                score = slideLeft(status);
                rotate_left(status);
                break;
            case 2:
                reflect_horizontal(status);
                score = slideLeft(status);
                reflect_horizontal(status);
                break;
            case 3:
                rotate_right(status);
                actionTaken = 2;
                score = slide(status);
                actionTaken = 3;
                rotate_left(status);
                break;
            default:
                break;
        }
        return score;
    }
};

#endif /* agent_h */
