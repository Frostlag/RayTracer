#include "BlockList.hpp"
#include <iostream>
using namespace std;


BlockList::BlockList(int h, int w, int subdivisions){
    lastProgress = 0;
    int xPerSubdivision = w / subdivisions, yPerSubdivision = h / subdivisions;

    int hcap = h/2 + h % 2;
    int wcap = w/2 + w % 2;

    squareRowsNeeded =  subdivisions * h;
    squareRowsDone = 0;

    int startY = -h/2, endY = startY + yPerSubdivision;
	for (int j = 0; j < subdivisions; j++){
		if (j == subdivisions - 1 && endY != hcap)
		{
			endY = hcap-1;
		}

		int startX = -w/2, endX = startX + xPerSubdivision;

		for (int k = 0; k < subdivisions; k++){
			if (k == subdivisions - 1 && endX != wcap)
			{
				endX = wcap-1;
			}
            Block block(startX, startY, endX, endY, k * xPerSubdivision, j * yPerSubdivision);
            blockList.push_back(block);
			// thread* temp = new thread(A4_Render_Thread, root, &image, eye, view, up, fovy, ambient, lights, E, baseZ, u, r, invV, startX, startY, endX ,endY, k * xPerSubdivision, j * yPerSubdivision);
			// threadvector.push_back(temp);
			startX += xPerSubdivision;
			endX += xPerSubdivision;
		}
		startY += yPerSubdivision;
		endY += yPerSubdivision;
	}


}

Block BlockList::getBlock(){
    lk.lock();
    if (blockList.empty()){
        lk.unlock();
        throw string("Done");
    }
    Block ret = blockList.front();
    blockList.pop_front();
    lk.unlock();

    return ret;
}

void BlockList::doneRow(){
    squareRowsDone++;

}

void BlockList::outputProgress(){
    float progress = 100 * ((float)squareRowsDone) / (float)squareRowsNeeded;
    cout << progress << "%" << endl;
    if ( progress - lastProgress > 0.1){
        lastProgress = progress;

    }
}
