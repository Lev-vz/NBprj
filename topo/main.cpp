/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: lev
 *
 * Created on 13 октября 2020 г., 10:30
 */

#include <cstdlib>
#include <iostream>

using namespace std;

struct node{
    int q=0;
    int *point;
    int cnt=0;
};

int *arr1, *arr2;
node *nodeArr;
int maxCnt  = 0;

void chain(int prev, int curr, int cnt);
void cutChain(int, int);

int main(int argc, char** argv) {
    int n = 21;
    //cin>>n;
    arr1= new int[n];
    arr2= new int[n];
    nodeArr = new node[n];
    
    arr1[0]=7; arr2[0]=1;
    arr1[1]=1; arr2[1]=2;
    arr1[2]=2; arr2[2]=10;
    arr1[3]=10; arr2[3]=11;
    arr1[4]=2; arr2[4]=3;
    arr1[5]=3; arr2[5]=4;
    arr1[6]=4; arr2[6]=8;
    arr1[7]=8; arr2[7]=9;
    arr1[8]=4; arr2[8]=5;
    arr1[9]=8; arr2[9]=12;
    arr1[10]=5; arr2[10]=6;
    arr1[11]=11; arr2[11]=13;
    arr1[12]=13; arr2[12]=14;
    arr1[13]=12; arr2[13]=15;
    arr1[14]=15; arr2[14]=16;
    arr1[15]=16; arr2[15]=17;
    arr1[16]=17; arr2[16]=18;
    arr1[17]=18; arr2[17]=19;
    arr1[18]=19; arr2[18]=20;
    arr1[19]=20; arr2[19]=21;
    
    for(int i=0;i<n-1;i++){
        //cin>>arr1[i]>>arr2[i];
        arr1[i]--;
        arr2[i]--;
        nodeArr[arr1[i]].q++;
        nodeArr[arr2[i]].q++;
    }
    
    int *pt = new int[n-1];
    int ptCnt = 0;
    
    for(int i=0; i < n; i++){
        nodeArr[i].point = new int[nodeArr[i].q];
        if(nodeArr[i].q==1) pt[ptCnt++] = i;
    }
    
    for(int i=0; i < n; i++){
        nodeArr[arr1[i]].point[nodeArr[arr1[i]].cnt++] = arr2[i];
        nodeArr[arr2[i]].point[nodeArr[arr2[i]].cnt++] = arr1[i];
    }
    
//    for(int i=0; i < n; i++){
//        cout<< "node"<<i<<"("<<nodeArr[i].q<<")";
//        for(int j=0; j < nodeArr[i].q; j++) cout<<"["<<nodeArr[i].point[j]<<"]";
//        cout<< "\n";
//    }
    for(int i=0; i < ptCnt; i++){
        arr2[0] = pt[i];
        chain(pt[i], nodeArr[pt[i]].point[0], 1);

//        for(int j=0; j < maxCnt; j++){
//            cout<< arr1[j]<<"+";
//        }
//        cout<< "\n";
    }
    
    int start = arr1[0];
    int end = arr1[maxCnt-1];

    int dotCut = maxCnt/2-1;
//    cout<<"dotCut = "<<dotCut<< "\n";
    int t1 = arr1[dotCut];
    int t2 = arr1[dotCut+1];
    cutChain(t1, t2);
    cutChain(t2, t1);
    
//    for(int i=0; i < n; i++){
//        cout<< "node"<<i<<"("<<nodeArr[i].q<<")";
//        for(int j=0; j < nodeArr[i].q; j++) cout<<"["<<nodeArr[i].point[j]<<"]";
//        cout<< "\n";
//    }
    
    if(nodeArr[start].q==0){
        t1 = start+1;
    }else{
        arr2[0] = start;
        maxCnt  = 0;
        chain(start, nodeArr[start].point[0], 1);
//        for(int j=0; j < maxCnt; j++){
//            cout<< arr1[j]<<"+";
//        }
//        cout<< "\n";
        t1 = arr1[maxCnt/2]+1;
    }
    maxCnt  = 0;
    arr2[0] = end;
    chain(end, nodeArr[end].point[0], 1);
//    for(int j=0; j < maxCnt; j++){
//        cout<< arr1[j]<<"+";
//    }
//    cout<< "\n";
    t2 = arr1[maxCnt/2]+1;
    
    cout<<t1<<" "<<t2<< "\n";
    
    return 0;
}

void cutChain(int t1, int t2){
    bool flag = false;
    nodeArr[t1].q--;
    for(int i=0; i<nodeArr[t1].q; i++){
        if(nodeArr[t1].point[i]==t2) flag = true;
        if(flag) nodeArr[t1].point[i] = nodeArr[t1].point[i+1];
    }
}

void chain(int prev, int curr, int cnt){//, int *arr1, node *nodeArr)
    arr2[cnt] = curr;
    if(nodeArr[curr].q == 1){
        if(cnt >= maxCnt){
            maxCnt = cnt+1;
            for(int i=0; i<=cnt; i++) arr1[i] = arr2[i];
        }
    }

    for(int i = 0; i < nodeArr[curr].q; i++){
        if(nodeArr[curr].point[i] != prev){
            arr2[cnt] = curr;
            chain(curr,nodeArr[curr].point[i],cnt+1);
        }
    }
}