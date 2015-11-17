#pragma once
#include <list>
#include <mutex>
#include <atomic>

struct Block{
    int startX, startY, endX, endY, startPX, startPY;
    Block(int startX, int startY, int endX, int endY, int startPX, int startPY):startX(startX), startY(startY), endX(endX), endY(endY), startPX(startPX), startPY(startPY){};
};

class BlockList{
    std::atomic_uint squareRowsDone;
    unsigned int squareRowsNeeded;
    float lastProgress;
    std::mutex lk;
    std::list<Block> blockList;

public:
    BlockList(int h, int w, int subDivisions);
    Block getBlock();
    void doneRow();
    void outputProgress();
    bool isAlmostDone(){return blockList.empty();}
};
