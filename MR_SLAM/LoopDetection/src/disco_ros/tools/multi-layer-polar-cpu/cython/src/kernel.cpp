#include <stdio.h>
#include <ctime>
#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm> 
#include <cstdlib>
#include <memory>
#include <iostream>
#include <vector>

float rad2deg(float radians)
{
    return radians * 180.0 / M_PI;
}

float deg2rad(float degrees)
{
    return degrees * M_PI / 180.0;
}

float xy2theta(const float & _x, const float & _y )
{
    if ( _x >= 0 & _y >= 0) 
        return (180/M_PI) * atan(_y / _x);

    else if ( _x < 0 & _y >= 0) 
        return 180 - ( (180/M_PI) * atan(_y / (-_x)) );

    else if ( _x < 0 & _y < 0) 
        return 180 + ( (180/M_PI) * atan(_y / _x) );

    else if ( _x >= 0 & _y < 0)
        return 360 - ( (180/M_PI) * atan((-_y) / _x) );
} // xy2theta


// convert pointcloud to grid map
void point2gridmap(float* point, int* ring, int* sector, int* height, int d_size, int max_length, int max_height, int num_ring, int num_sector, int num_height) 
{

    for(int i = 0; i < d_size; i++){
        float gap_ring, gap_sector, gap_height;

        gap_ring = (float)max_length / (float)num_ring;
        gap_sector = 360.0/(float)num_sector;
        gap_height = 2.0 * (float)max_height / (float)num_height;

        float x, y, z;
        x = point[i];
        y = point[i + d_size];
        z = point[i + 2 * d_size];
        
        if(x == 0.0)
            x = 0.0001;
        if(y == 0.0)
            y = 0.0001;
        if(z == 0.0)
            z = 0.0001;

        float theta = xy2theta(x, y);
        float faraway = sqrt(pow(x,2) + pow(y,2));

        int idx_ring = floor(faraway / gap_ring);
        int idx_sector = floor(theta / gap_sector);
        int idx_height = floor((z + (float)max_height) / gap_height);

        if(idx_ring >= num_ring)
        idx_ring = num_ring - 1;

        height[i] = idx_height;
        ring[i] = idx_ring;
        sector[i] = idx_sector;
    }

}

