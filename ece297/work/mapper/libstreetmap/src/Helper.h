/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Helper.h
 * Author: qiuchenh
 *
 * Created on February 26, 2018, 4:09 PM
 */
#include "graphics.h"
#include "Global.h"
#include "graphics_types.h"
#ifndef HELPER_H
#define HELPER_H



// unit conversion functions
float xcoord(double loncoord) {
    return loncoord * DEG_TO_RAD * cos(average_lat);
}

float ycoord(double latcoord) {
    return latcoord * DEG_TO_RAD;
}

float x_to_lon(float x) {
    return x / (DEG_TO_RAD * cos(average_lat));
}

float y_to_lat(float y) {
    return y / DEG_TO_RAD;
}


//feature type colors
t_color getFeatureColor(FeatureType type) {
    t_color color;
    switch (type) {
        default: color.red = 0;
            color.green = 0;
            color.blue = 0; //unknown
            break;
        case 1: color.red = 145;
            color.green = 242;
            color.blue = 151; //park
            break;
        case 2: color.red = 236;
            color.green = 242;
            color.blue = 174; //beach
            break;
        case 3: color.red = 135;
            color.green = 217;
            color.blue = 255; //lake
            break;
        case 4: color.red = 135;
            color.green = 217;
            color.blue = 255; //river
            break;
        case 5: color.red = 0;
            color.green = 255;
            color.blue = 127; //island
            break;
        case 6: color.red = 255;
            color.green = 255;
            color.blue = 224; //shoreline
            break;
        case 7: color.red = 184;
            color.green = 184;
            color.blue = 184; //building
            break;
        case 8: color.red = 145;
            color.green = 242;
            color.blue = 151; //greenspace
            break;
        case 9: color.red = 0;
            color.green = 250;
            color.blue = 154; //golfcourse
            break;
        case 10: color.red = 135;
            color.green = 217;
            color.blue = 255; //stream
    }
    return color;
}
////feature type names

std::string getFeatureType(FeatureType type) {
    std::string featureType;
    switch (type) {
        default: //unknown
            break;
        case 1: featureType = "Park"; //park
            break;
        case 2: featureType = "Beach"; //beach
            break;
        case 3: featureType = "Lake"; //lake
            break;
        case 4: featureType = "River"; //river
            break;
        case 5: featureType = "Island"; //island
            break;
        case 6: featureType = "Shoreline"; //shoreline
            break;
        case 7: featureType = "Building"; //building
            break;
        case 8: featureType = "Greenspace"; //greenspace
            break;
        case 9: featureType = "Golfcourse"; //golfcourse
            break;
        case 10: featureType = "Stream"; //stream

    }
    return featureType;
}

//Help user to zoom in to a small area
void Zoom_in_area() {
  
    float xl_new = for_search_zoom.x - zoomWidth;
    float yb_new = for_search_zoom.y + zoomWidth * screen_ratio;
    float xt_new = for_search_zoom.x + zoomWidth;
    float yt_new = for_search_zoom.y - zoomWidth * screen_ratio;
    //std::cout << "for_search_zoom.x = " << for_search_zoom.x <<std::endl;
   // std::cout << "yt_new  = " <<yt_new<<std::endl;
    set_visible_world(xl_new, yb_new, xt_new, yt_new);

}

void for_highlight_ZOOM(){
    //zoom into the highlighted path
    t_point leftH;
    t_point rightH;
    if (inter_to_POI == true){
        
    }
    if (userInputs.intersWorldP[0].x < userInputs.intersWorldP[1].x ){
        leftH =userInputs.intersWorldP[0];
        rightH =userInputs.intersWorldP[1];
    }
    else {
       rightH =userInputs.intersWorldP[0];
        leftH =userInputs.intersWorldP[1];
    }
    
    float lengthX = (rightH.x-leftH.x)/2;
    float lengthY = abs((rightH.y-leftH.y)/2);
    t_point midP;
    float xl_new, yb_new,xt_new, yt_new ;
    float zoomwidth =0.0001;
    
    if (lengthX > lengthY) zoomwidth = lengthX;
    else zoomwidth = lengthY;
    if (leftH.y < rightH.y) {   // left above right;
          midP.x = leftH.x + lengthX;
          midP.y = leftH.y + lengthY; 
    }  
    else { //right above left
          midP.x = leftH.x + lengthX;
          midP.y = rightH.y + lengthY;
     
    }     
    xl_new = midP.x - lengthX - zoomwidth;
    yb_new = midP.y + (lengthY+zoomwidth)*screen_ratio;
    xt_new = midP.x +  lengthX + zoomwidth;
    yt_new = midP.y - (lengthY+zoomwidth)*screen_ratio;
    set_visible_world(xl_new, yb_new, xt_new, yt_new);
   // drawrect(xl_new, yb_new, xt_new, yt_new);
   
   setcolor(RED);
   
   
}


//The function get the point where user clicked in world coordinate
t_point get_world_point (unsigned id){
    double lon = xcoord((mymap->intersectionUOM[id]).intersectionPosition1.lon());
    double lat = ycoord((mymap->intersectionUOM[id]).intersectionPosition1.lat());
    t_point worldCoor;
    worldCoor.x = lon;
    worldCoor.y = lat;
    for_search_zoom = worldCoor;
   // std::cout << "for_search_zoom.x= " <<for_search_zoom.x<< std::endl;
    return worldCoor;
}

 //given a inter ID, return a t_point of this location in screen coor
t_point coordinate_convert_inter(unsigned inter_id_intersection) {

    t_point worldC = get_world_point(inter_id_intersection);
    t_point screenCoor;   
    screenCoor = world_to_scrn(worldC); 
  //   std::cout << "screenCoor.x= " <<screenCoor.x<< std::endl;
    
    return screenCoor;
}





#endif /* HELPER_H */

