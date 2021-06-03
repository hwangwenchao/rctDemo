#include "ncShreddedCommon.h"
#include <iostream>
#include <vector>

std::vector<Box> GetBindingBox(int m, int n, int pic[][9])
{

    std::vector<Box> boxes;
    Box box;
    int boxLength = 0;
    int boxWidth = 0;
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            //std::cout<<pic[i][j]<<std::endl;
            //std::cout<<pic[i][j]<<" ";
            if(i>0 && j>0 && pic[i][j] != pic[i-1][j]
                          && pic[i][j] != pic[i+1][j]
                          && pic[i][j] != pic[i][j-1]
                          && pic[i][j] != pic[i][j+1])
            {
                box.m_minX = i;
                box.m_minY = j;
                box.m_maxX = i;
                box.m_maxY = j;
                boxes.push_back(box);
            }
            //
            if(pic[i][j] == 1 || pic[i][j+1] == 1)
            {
                boxLength +=1;
            }
        }
        //std::cout<<std::endl;
    }

    return boxes;

}

#ifdef __WINDOWS__
int _tmain(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    std::cout<<"------------start--------------"<<std::endl;

    int pic[4][9] = {
        {0, 0, 0, 1, 1, 1, 0, 1, 1},
        {0, 0, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 1, 0, 0, 1, 0, 0, 1},
        {0, 0, 1, 0, 0, 0, 1, 1, 1},
    };

    std::vector<Box> boxes = GetBindingBox(4, 9, pic);
    for(size_t i = 0; i< boxes.size(); i++){
        std::cout<<"(("<<boxes.at(i).m_minX<<","<<boxes.at(i).m_minY<<"),";
        std::cout<<"("<<boxes.at(i).m_maxX<<", "<<boxes.at(i).m_maxY<<"))";
    }


    return 0;
}