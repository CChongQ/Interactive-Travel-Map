/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Information_bar.h
 * Author: qiuchenh
 *
 * Created on February 26, 2018, 5:24 PM
 */

#ifndef INFORMATION_BAR_H
#define INFORMATION_BAR_H

#include "newmap.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "Helper.h"
#include "interface_icon.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h" 
#include "graphics.h"
#include "Global.h"
#include "graphics_types.h"
#include "X11/keysymdef.h"

// draw a static search at the bottom of screen

void drawSearchBar() {

    // search search bars on top
    setcolor(RED);

    if (inter_to_POI == false) {
        //  std::cout << "Change to Inter" << std::endl;
        draw_surface(load_png_from_file("poi-switch-button.png"), changetoPOI.left(), changetoPOI.top());
        if (userInputWrong == false)setcolor(WHITE);
        else setcolor(t_color (255,175,147));
        fillrect(leftsearchBar.left(), leftsearchBar.bottom(), leftsearchBar.right(), leftsearchBar.top()); //left
        fillrect(rightSearchBar.left(), rightSearchBar.bottom(), rightSearchBar.right(), rightSearchBar.top()); //right
        setcolor(t_color(201, 198, 196));
        setfontsize(14);
        if (disappear_text_one == false) drawtext(bottom_l_x_left + bar_length / 2, bar_height_bottom * 0.55, "Street One ");
        if (disappear_text_two == false) drawtext(bottom_l_x_right + bar_length / 2, bar_height_bottom * 0.55, "Street Two ");

    } else {    
        draw_surface(load_png_from_file("intersection-switch-button.png"), changeToInter.left(), changeToInter.top());
        setcolor(WHITE);
        fillrect(changeToPOI_text.left(), changeToPOI_text.bottom(), changeToPOI_text.right(), changeToPOI_text.top()); //left      
        setcolor(t_color(201, 198, 196));
        setfontsize(14);
        if (disappear_POI == false) drawtext(bottom_l_x_left + bar_length / 2, bar_height_bottom * 0.55, "Point of Intersest ");
    }


    draw_surface(load_png_from_file("search_icon.png"), searchIcon.left(), searchIcon.top());

    // middle
    draw_surface(load_png_from_file("findpath.png"), findPath.left(), findPath.top());

    draw_surface(load_png_from_file("list_icon.png"), listInter.left(), listInter.top());
    //right side    
    draw_surface(load_png_from_file("zoom_in.png"), zoomInButton.left(), zoomInButton.top());

    draw_surface(load_png_from_file("zoom_out.png"), zoomOutButton.left(), zoomOutButton.top());

    draw_surface(load_png_from_file("question_mark.png"), questionMark.left(), questionMark.top());

    //draw details
    if (!details_opened) {
        
        draw_surface(load_png_from_file("details-tab.png"),
                detailsTab.left(),
                detailsTab.top());
        
    } else if (details_opened) {

  
        draw_surface(load_png_from_file("details-tab.png"),
                opened_detailsTab.left(),
                opened_detailsTab.top());
        setcolor(67, 68, 69);
        fillrect(opened_detailsTab.right(),
                bottom_l_y * 0.99,
                top_r_x * 0.99,
                bottom_l_y * 0.99 - detailsTabLength_size);
    }

    //draw changemap    
    if (!changeMap_opened) {
        draw_surface(load_png_from_file("change-map.png"),
                changeMapTab.left(),
                changeMapTab.top());
    } else if (changeMap_opened) {
        draw_surface(load_png_from_file("change-map.png"),
                opened_changeMapTab.left(),
                opened_changeMapTab.top());
        setcolor(98, 100, 103);
        fillrect(opened_changeMapTab.right(), 
                opened_changeMapTab.bottom(),
                top_r_x * 0.99,
                opened_changeMapTab.top());

        for (int i = 0; i < 19; i++) {
            print_changeMap_icons(i, mapNames[i]);
        }
    }

    //draw layers
    if (!layer_opened) {
        draw_surface(load_png_from_file("layers.png"),
                layerTab.left(),
                layerTab.top());
    } else if (layer_opened) {
        draw_surface(load_png_from_file("layers.png"),
                opened_layerTab.left(),
                opened_layerTab.top());
        setcolor(135, 138, 141);
        fillrect(opened_layerTab.right(),
                opened_layerTab.bottom(),
                top_r_x * 0.99,
                opened_layerTab.top());
        for (int i = 0; i < 6; i++) {
            if (layer_pressed[i]) {
                print_layer_icons(i, layerTypes_pressed[i]);
            } else {
                print_layer_icons(i, layerTypes[i]);
            }
        }
    }
  
}

void print_changeMap_icons(int i, std::string name) {
    const char *map_name = name.c_str();
    draw_surface(load_png_from_file(map_name),
            opened_changeMapTab.right() + mapIcon_size * i + changeMap_icon_gap * (i + 1),
            changeMap_icon_top_left_y);
}

void print_layer_icons(int i, std::string name) {
    const char *layer_name = name.c_str();
    draw_surface(load_png_from_file(layer_name),
            opened_layerTab.right() + layerIcon_size * i + layer_icon_gap * (i + 1),
            layer_icon_top_left_y);
}

void draw_string() {
    setcolor(BLACK);
    setfontsize(14);
    if (inter_to_POI == true) {
        if (!outputString_POI.empty()) {
            drawtext(bottom_l_x_left + bar_length / 2, bar_height_bottom * 0.55, outputString_POI);
        }
    } else {
        if (!outputString_s1.empty()) {
            drawtext(bottom_l_x_left + bar_length / 2, bar_height_bottom * 0.55, outputString_s1);
        }
        if (!outputString_s2.empty()) {
            drawtext(bottom_l_x_right + bar_length / 2, bar_height_bottom * 0.55, outputString_s2);
        }
    }

    
}

//This function will be called when user press any botton at mouse

void act_on_mouse_click(float x, float y, t_event_buttonPressed event) {

    if (event.button == 3) {
        set_visible_world(initial_coords);
    } else if (event.button == 1) {
        user_clicked_world.x = x;
        user_clicked_world.y = y;
        user_clicked_screen = world_to_scrn(user_clicked_world);
        //click on info area  
        set_keypress_input(true);
        if ((leftsearchBar.intersects(user_clicked_screen)) == true && inter_to_POI == false) {
            // user clicked on Street One search box
            inS1 = true;
            inS2 = false;
            disappear_text_one = true;
            searIconClick = false;
            help_pressed = false;
        }//user clicked on street2 search bar
        else if (changetoPOI.intersects(user_clicked_screen.x, user_clicked_screen.y) == true && inter_to_POI == false) {
        //    foundInterID.reset();
            inter_to_POI = true;
            help_pressed = false;
        } else if (changeToInter.intersects(user_clicked_screen.x, user_clicked_screen.y) == true && inter_to_POI == true) {
            inter_to_POI = false;
            help_pressed = false;
        } else if (changeToPOI_text.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
            disappear_POI = true;
            inPOI = true;
            inS1 = false;
            inS2 = false;
            help_pressed = false;
        } else if (rightSearchBar.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
            inS1 = false;
            inS2 = true;
            disappear_text_two = true;
            searIconClick = false;
            layer_opened = false;
            changeMap_opened = false;
            help_pressed = false;
        } else if (searchIcon.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
            searIconClick = true;
            interIconClick = false;
            disappear_text_one = true;
            disappear_text_two = true;
            highlight_street_intersections();
            searIconClick = false;
            help_pressed = false;
            userClickOnMap = false;

        } else if (findPath.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
            findPathClick = true;       
            highlightPointSize = userInputs.intersWorldP.size();
            if (userInputs.POIsName.size() >0 ) {
               drawHighlight_POI_path();
            } else {
                drawHightlightPath();
                for_highlight_ZOOM();
            }
            details_opened = true;
            layer_opened = false;
            changeMap_opened = false;
            help_pressed = false;
        } else if ((questionMark.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) && !help_pressed) {
            help_pressed = true;
            layer_opened = false;
            changeMap_opened = false;
        } else if ((helpTab1.intersects(user_clicked_screen.x, user_clicked_screen.y)) && help_pressed) {
            help_pressed = true;
            helpTab1_pressed = true;
            helpTab2_pressed = false;
            helpTab3_pressed = false;
        } else if ((helpTab2.intersects(user_clicked_screen.x, user_clicked_screen.y)) && help_pressed) {
            help_pressed = true;
            helpTab1_pressed = false;
            helpTab2_pressed = true;
            helpTab3_pressed = false;
        } else if ((helpTab3.intersects(user_clicked_screen.x, user_clicked_screen.y)) && help_pressed) {
            help_pressed = true;
            helpTab1_pressed = false;
            helpTab2_pressed = false;
            helpTab3_pressed = true;
        } else if ((helpScreen.intersects(user_clicked_screen.x, user_clicked_screen.y)) && help_pressed) {
            help_pressed = true;
        } else if (zoomInButton.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
            if (!inputString_s1.empty()) disappear_text_one = true;
            if (!inputString_s2.empty()) disappear_text_two = true;
            zoom_in(*drawscreen);
            layer_opened = false;
            changeMap_opened = false;
            help_pressed = false;
        } else if (zoomOutButton.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
            if (!inputString_s1.empty()) disappear_text_one = true;
            if (!inputString_s2.empty()) disappear_text_two = true;
            zoom_out(*drawscreen);
            layer_opened = false;
            changeMap_opened = false;
            help_pressed = false;
        } else if (listInter.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
            if (!inputString_s1.empty()) disappear_text_one = true;
            if (!inputString_s2.empty()) disappear_text_two = true;
            if (interIconClick == true) interIconClick = false;
            else interIconClick = true;
            layer_opened = false;
            changeMap_opened = false;
            help_pressed = false;
        } else if ((layerTab.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) && !layer_opened) {
            layer_opened = true;
            changeMap_opened = false;
        } else if ((changeMapTab.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) && !changeMap_opened) {
            changeMap_opened = true;
            layer_opened = false;
            help_pressed = false;
        } else if ((detailsTab.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) && !details_opened) {
            if (details_opened == true) details_opened = false;
            else details_opened = true;
            layer_opened = false;
            changeMap_opened = false;
        } else if (layer_opened && (wholeScreen.intersects(user_clicked_screen.x, user_clicked_screen.y)) == true) {
            if (opened_layerTab.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
                layer_opened = false;
                changeMap_opened = false;
                help_pressed = false;
            } else {
                for (int i = 0; i < 6; i++) {
                    if (layer_icon_boundaries[i].intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
                        changeMap_opened = false;
                        help_pressed = false;
                        if (layer_pressed[i]) {
                            layer_pressed[i] = false;
                        } else {
                            layer_pressed[i] = true;
                        }
                    }
                }
            }
        } else if (changeMap_opened && (wholeScreen.intersects(user_clicked_screen.x, user_clicked_screen.y) == true)) {
            if ((opened_changeMapTab.intersects(user_clicked_screen.x, user_clicked_screen.y) == true)) {
                layer_opened = false;
                changeMap_opened = false;
                help_pressed = false;
            } else {
                for (int i = 0; i < 19; i++) {
                    if (map_icon_boundaries[i].intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
                        changeMap_opened = false;
                        help_pressed = false;
                        details_opened = false;
                        int stringSize = mapNames[i].size() - 4;
                        std::string name = mapNames[i];
                        name = name.erase(stringSize, 4);
                        change_map(name);
                    }
                }
            }
        } else if ((opened_detailsTab.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) && details_opened) {
            details_opened = false;
            userTypeChangeP = false;
            layer_opened = false;
            changeMap_opened = false;
            only_draw_string = false;
            help_pressed = false;
        } 
        else if ((crossArea.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) && interIconClick == false) {
            details_opened = false;
            foundPath.clear();
            allHighlightSS.clear();
            multiplePath.clear();
            outInfo.clear();
            highlightColor = false;
            int which_cross = 0;
            for (int i = 0; i < abs(allCross.size()); i++) {
                //clicked on which one
                if (allCross[i].intersects(user_clicked_screen.x, user_clicked_screen.y) == true) {
                    which_cross = i;
                    break;
                }
            }      
            //delete this intersection
            userInputs.intersID.erase(userInputs.intersID.begin() + which_cross); //id
            userInputs.inputIntersections.erase(userInputs.inputIntersections.begin() + which_cross); //name
            userInputs.intersWorldP.erase(userInputs.intersWorldP.begin() + which_cross); //position           
            
            
        }
        else if ((changePage_left.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) && details_opened == true){        
            //user want to change to previous page on detailed bar           
            if (details_opened == true) {
                userTypeChangeP = true;
                if (curretWindow > 0) curretWindow--;
            } else userTypeChangeP = false;
        }
        else if ((changePage_right.intersects(user_clicked_screen.x, user_clicked_screen.y) == true) && details_opened == true){
             //user want to change to next page on detailed bar
            if (details_opened == true) {
                userTypeChangeP = true;
                if (curretWindow < abs(userInputs.intersID.size()-2)) curretWindow++;
               
            } else userTypeChangeP = false;
           
        }
        else {
            // when clicked on map, all texts in information area will be erased
            userClickOnMap = true;
            draw_inter_streets = false;
            searIconClick = false;
            layer_opened = false;
            changeMap_opened = false;
            help_pressed = false;
            LatLon newpoint(y_to_lat(y), x_to_lon(x));  
            checkClickInter(newpoint);

        }


        
    }
    drawscreen();
}




//This function is used to draw highlight path between two intersections based on user input
void drawHightlightPath() {
    std::vector<unsigned> temp;
    if(userClickOnMap == false){
      temp = find_path_between_intersections(userInputs.intersID[userInputs.intersID.size() - 2], userInputs.intersID[userInputs.intersID.size() - 1], 15);  
    }
    else{
        temp = find_path_between_intersections(clickTwoInter.interOne,clickTwoInter.interTwo, 15);  
    }
   
    foundPath.push_back(temp);
    for (int j = 0; j < abs(temp.size()); j++) {
        allHighlightSS.push_back(temp[j]);
    }
    
    computeVectorPath(foundPath[foundPath.size()-1]);
    
    drawDirection();

    highlightColor = true;
}

//
////This function is used to draw highlight path between an intersections and POI 
void drawHighlight_POI_path() {
    //std::cout << "enter drawHighlight_POI_path()" <<std::endl;
    // std::cout << "userInputs.POIsName.size() ===" << userInputs.POIsName.size()  <<std::endl;
    std::vector<unsigned> temp;
    unsigned closest_interID;
    if (userInputs.POIsName.size() == 1) {
        //only one inter to one POI
        temp = find_path_to_point_of_interest(userInputs.intersID[0], userInputs.POIsName[0], 15);
        unsigned last_SSID = temp[temp.size()-1]; //lst ss id
        closest_interID = (mymap->streetSegmentUOM[last_SSID]).to; //nearest intersection id
       
        lastPOI_closestInter = closest_interID;
 

    } else {
        
        // std::vector<unsigned> temp2 = find_path_to_point_of_interest(lastPOI_closestInter, userInputs.POIsName[userInputs.POIsName.size() - 1], 15);
        //the second last poi     
        temp = find_path_to_point_of_interest(lastPOI_closestInter, userInputs.POIsName[userInputs.POIsName.size() - 1], 15);
        unsigned last_SSID = temp[temp.size() - 1];
        closest_interID = (mymap->streetSegmentUOM[last_SSID]).to;
       
        lastPOI_closestInter = closest_interID;
       




    }

    foundPath_POI.push_back(temp);

    
    POI_highlightPoint.push_back(get_world_point(closest_interID));
  //  std::cout << "POI_highlightPoint.size() original = " << POI_highlightPoint.size() << std::endl;

    for (int j = 0; j < abs(temp.size()); j++) {     
        allHighlightSS.push_back(temp[j]);
    }

    computeVectorPath(foundPath_POI[foundPath_POI.size() - 1]);

    drawDirection();

    highlightColor = true;
    /* foundPath = find_path_to_point_of_interest(foundInterID.interOne, foundInterID.POI, 15);
    computeVectorPath(foundPath);
    if (foundPath.size() == 0) {
    } else {
        highlightColor = true;
    }*/

}


//This function checks whether user clicked on a correct intersection position
//If it's correct, it will put information about clicked intersection into "clickTwoInter"

void checkClickInter(LatLon& newPoint) {
    inter_id = find_closest_intersection(newPoint);
    double distance = find_distance_between_two_points(newPoint, (mymap->intersectionUOM[inter_id]).intersectionPosition1);
    if (distance <= 15) {
        //the distance between clicked position and the nearest intersection is less than 15m
        //user clicked on correct position
        click_screen = coordinate_convert_inter(inter_id);
        if (clickTwoInter.interOne == 0) {
            //user click on a intersection for the first time
            if (foundPath.size() != 0) foundPath.clear();
            if (foundPath_POI.size() != 0) foundPath_POI.clear();
            multiplePath.clear();
            outInfo.clear();
            allHighlightSS.clear();
            clickTwoInter.interOne = inter_id;
            clickTwoInter.interOneW = get_world_point(inter_id);
            Zoom_in_area();
            click_screen = coordinate_convert_inter(inter_id);
        } else {
            if (clickTwoInter.interTwo == 0) {
                //user click the second intersection
                clickTwoInter.interTwo = inter_id;
                clickTwoInter.interTwoW = get_world_point(inter_id);
                clickTwoInter.twoInput = true;
                details_opened = true;
                drawHightlightPath();
            } else {
                //user click on the third intersection
                //which means that it will start a new round to check
                std::cout << "Clear---------" << std::endl;
                clickTwoInter.reset();
                foundPath.clear();
                multiplePath.clear();
                outInfo.clear();
                allHighlightSS.clear();
                highlightColor = false;
                clickTwoInter.interOne = inter_id;
                clickTwoInter.interOneW = get_world_point(inter_id);
                Zoom_in_area();
                click_screen = coordinate_convert_inter(inter_id);
            }
        }
    } else {
        std::cout << "Wrong position" << std::endl;
    }
}

//This function compute a vector for each street segment to check the driving direction

void computeVectorPath(std::vector<unsigned>& inputIDs) {
    // std::cout << "Enter computeVectorPath" <<std::endl; 
    std::vector<float> position;
    oneInterToInter singlePath;
    twoSS tempTwoSS;
    for (int i = 0; i < abs(inputIDs.size()); i++) {
        if (inputIDs.size() == 1) {
            std::cout << "Only one street segment" << std::endl;
        } else {
            if (i + 1 < abs(inputIDs.size())) {
                if ((mymap->streetSegmentUOM[inputIDs[i]]).streetID != (mymap->streetSegmentUOM[inputIDs[i + 1]]).streetID) {
                    // vector of a street segment is equal to: "to" point - "from" point
                    //also store the vector of next street segment if user need to make a turn
                    unsigned thisStreetID = (mymap->streetSegmentUOM[inputIDs[i]]).streetID;
                    unsigned nextStreetID = (mymap->streetSegmentUOM[inputIDs[i + 1]]).streetID;

                    // if two street segment have same id, they are in the same street, no need to change driving direction
                    if (thisStreetID == nextStreetID) {
                        std::cout << "Same Street ID!!!!" << std::endl;
                    } else {

                        unsigned fromID_before = (mymap->streetSegmentUOM[inputIDs[i]]).from;
                        unsigned toID_before = (mymap->streetSegmentUOM[inputIDs[i]]).to;
                        unsigned fromID_after = (mymap->streetSegmentUOM[inputIDs[i + 1]]).from;
                        unsigned toID_after = (mymap->streetSegmentUOM[inputIDs[i + 1]]).to;
                        if (toID_before != fromID_after) {
                            if (fromID_before == fromID_after) {
                                unsigned temp;
                                temp = fromID_before;
                                fromID_before = toID_before;
                                toID_before = temp;
                            } else if (fromID_before == toID_after) {
                                unsigned temp;
                                temp = fromID_before;
                                fromID_before = toID_before;
                                toID_before = temp;
                                temp = fromID_after;
                                fromID_after = toID_after;
                                toID_after = temp;
                            } else if (toID_before == toID_after) {
                                unsigned temp;
                                temp = fromID_after;
                                fromID_after = toID_after;
                                toID_after = temp;
                            }                           
                        }
                        t_point from_T = get_world_point(fromID_before);
                        t_point to_T = get_world_point(toID_before);
                        tempTwoSS.before.x = to_T.x - from_T.x;
                        tempTwoSS.before.y = to_T.y - from_T.y;
                        t_point from_T_after = get_world_point(fromID_after);
                        t_point to_T_after = get_world_point(toID_after);
                        tempTwoSS.after.x = to_T_after.x - from_T_after.x;
                        tempTwoSS.after.y = to_T_after.y - from_T_after.y;
                        
                        tempTwoSS.before_SID = thisStreetID;
                        tempTwoSS.after_SID = nextStreetID;
                        
                        vectorP tempP = std::make_pair(tempTwoSS, inputIDs[i]);
                        singlePath.push_back(tempP);
                        
                    }
                    
                    
                }
            } else break;
        }
    }
    
    multiplePath.push_back(singlePath);  
}



//This function calculate turn direction use cross product, and store the result into a vector

void drawDirection() {
    std::cout << "Enter draw direction" << std::endl;
    if (multiplePath.size() > 0) {
        for (int i = 0; i < abs(multiplePath.size()); i++) {
            //multiplePath[i] is current path
            for (int j = 0; j < abs(multiplePath[i].size()); j++) {
                twoSS calPoint = (multiplePath[i])[j].first;
                float crossProduct = calPoint.before.x * calPoint.after.y - calPoint.before.y * calPoint.after.x;
                if (crossProduct < 0)
                    (multiplePath[i])[j].first.direction = "Right";
                else
                    (multiplePath[i])[j].first.direction = "Left";
            }
        }
    }

}

void drawDetails() {

    
    if ( userInputs.intersID.size() <= 1 && clickTwoInter.interTwo == 0 ) { //if user click detailed bar before finish input
        std::cout << "Not Enough input" << std::endl;
    } else {
        std::string from;
        std::string to;
        if (userClickOnMap == true) {          
            from = (mymap->intersectionUOM[clickTwoInter.interOne]).intersectionName1;
            to = (mymap->intersectionUOM[clickTwoInter.interTwo]).intersectionName1;
             drawPathByPath(from, to, multiplePath[0], 0); 
        } else if (foundPath_POI.size() == 0){
         //   std::cout << "userInputs.intersID.size() = " << userInputs.intersID.size() << std::endl;
           // int num_path = userInputs.intersID.size() - 1;
             
                    //  fillrect(changePage_right.left(), changePage_right.bottom(), changePage_right.right(), changePage_right.top()); //left
                    //  fillrect(changePage_left.left(), changePage_left.bottom(), changePage_left.right(), changePage_right.top());
                    draw_surface(load_png_from_file("changPage_left.png"), changePage_left.left(), changePage_right.top());
                    draw_surface(load_png_from_file("changePage_right.png"), changePage_right.left(), changePage_right.top());
                
            if (userTypeChangeP == false) {
               //only show the first couple of intersections
                from = (mymap->intersectionUOM[userInputs.intersID[0]]).intersectionName1;
                to = (mymap->intersectionUOM[userInputs.intersID[1]]).intersectionName1;
                drawPathByPath(from, to, multiplePath[0], 0);               
            }
            else {
              

                //user change page              
                from = (mymap->intersectionUOM[userInputs.intersID[curretWindow]]).intersectionName1;
                to = (mymap->intersectionUOM[userInputs.intersID[curretWindow+1]]).intersectionName1;                               
                 drawPathByPath(from, to, multiplePath[curretWindow], curretWindow);        
            }
                     
        }
        else if(foundPath_POI.size() != 0) {
           // std::cout << "!!!!!!!" <<std::endl;
            if (userTypeChangeP == false) {
               //only show the first couple of intersections
            //   std::cout << "only show the first couple of intersections" <<std::endl;
                from = (mymap->intersectionUOM[userInputs.intersID[0]]).intersectionName1;
                to =  userInputs.POIsName[0];
                drawPathByPath(from, to, multiplePath[0], 0);               
            }
            else{
                //user change page              
                from = userInputs.POIsName[curretWindow-1];
                to = userInputs.POIsName[curretWindow];                            
                 drawPathByPath(from, to, multiplePath[curretWindow], curretWindow);        
            }
        }
    }
  std::cout << "Exit drawDetails" <<std::endl; 
}


//This function draw information in the detailed bar

void drawPathByPath(std::string from_name, std::string to_name, oneInterToInter& currentP, int pathID) {
  

    int fromW =  opened_detailsTab.right() + 60 + ( detailsTab.left() - 100 - (opened_detailsTab.right() + 60) )/2;
    int toW = fromW;

    //From intersection
    setcolor(WHITE);
    setfontsize(14);

    setcolor(t_color(241, 246, 242));
    fillrect(opened_detailsTab.right() + 60, bottom_l_y * 0.99 - png_size * 5.4286 + 28, detailsTab.left() - 100, bottom_l_y * 0.99 - png_size * 5.4286 + 2); //from name
    fillrect(opened_detailsTab.right() + 60, bottom_l_y * 0.99 - png_size * 5.4286 + 63, detailsTab.left() - 100, bottom_l_y * 0.99 - png_size * 5.4286 + 37); //to name
   
    setcolor(WHITE);
    drawtext(opened_detailsTab.right() + 30, bottom_l_y * 0.99 - png_size * 5.4286 + 15, "From: ");
    setcolor(BLACK);
    drawtext(fromW, bottom_l_y * 0.99 - png_size * 5.4286 + 15, from_name);
    //To intersection
    int which_pin = pathID % 4;

    if (pathID == abs(multiplePath.size() - 1) )
        draw_surface(load_png_from_file("blueEnd_print.png"), opened_detailsTab.right() + 30, bottom_l_y * 0.99 - png_size * 5.4286 + 30);
    else {
        if (which_pin == 0)
            draw_surface(load_png_from_file("1.png"), opened_detailsTab.right() + 30, bottom_l_y * 0.99 - png_size * 5.4286 + 30);
        else if (which_pin == 1)
            draw_surface(load_png_from_file("2.png"), opened_detailsTab.right() + 30, bottom_l_y * 0.99 - png_size * 5.4286 + 30);
        else if (which_pin == 2)
            draw_surface(load_png_from_file("3.png"), opened_detailsTab.right() + 30, bottom_l_y * 0.99 - png_size * 5.4286 + 30);
        else if (which_pin == 3)
            draw_surface(load_png_from_file("4.png"),opened_detailsTab.right() + 30, bottom_l_y * 0.99 - png_size * 5.4286 + 30);
    }
    
  
    setcolor(WHITE);
    drawtext(opened_detailsTab.right() + 20, bottom_l_y * 0.99 - png_size * 5.4286 + 50, "To");
    setcolor(BLACK);

    drawtext(toW, bottom_l_y * 0.99 - png_size * 5.4286 + 50, to_name);


    // A seperate line
    setcolor(WHITE);
    setlinewidth(1);
    drawline(opened_detailsTab.right(), bottom_l_y * 0.99 - png_size * 5.4286 + 68, detailsTab.left(), bottom_l_y * 0.99 - png_size * 5.4286 + 68);



    setfontsize(12);
    if (currentP.size() == 0) {
        std::cout << "The Path has only one street segment" << std::endl;
       // int tempSize = foundPath[pathID].size() -1;
        std::string streetName;
        if (foundPath_POI.size() != 0) {
            //find path between poi and inter
           // std::cout << "find path between poi and inter" << std::endl;
            unsigned streetID = (mymap->streetSegmentUOM[(foundPath_POI[pathID])[0]]).streetID;
            streetName = mymap->streeNAME[streetID];

        } else {
            //find path between inter and inter
            unsigned streetID = (mymap->streetSegmentUOM[(foundPath[pathID])[0]]).streetID;
            streetName = mymap->streeNAME[streetID];
        }
     
         
        drawtext(opened_detailsTab.right() + 80, bottom_l_y * 0.99 - png_size * 5.4286 + 79, "Go straight on : ");
        drawtext(opened_detailsTab.right() + 250, bottom_l_y * 0.99 - png_size * 5.4286 + 79, streetName);
    } else {
        //size of currentP is equal to how many turns driver will have 
        //each page will have at most 5 lines of info
        if (abs(currentP.size()) <= 5 && abs(currentP.size()) > 0) {
            //information can be displayed on one page 
            std::cout << "Don't need to change output window" << std::endl;

            printINFO(currentP,false);
        } else {
            //information need to be displayed on several page
            //create several vectors to stores informations, each vector stores at most 7 pieces of information
            //information is displayed on different pages based on their index 
            std::cout << "Need to change output window" << std::endl;
            userChangePage = true;
            int num_of_vec = currentP.size() / 5; //num of pages(vectors))       
            outInfo.clear();
            int beginI = 0;
            for (int i = beginI; i < num_of_vec; i++) { //split currentP into vectors                
                oneInterToInter temp(currentP.begin() + i * 5, currentP.begin() + i * 5 + 4);
                outInfo.push_back(temp);
                beginI = i + 1;
            }
            //  std::cout << "num_of_ outInfo = " << outInfo.size() << std::endl;
            int modu = currentP.size() % 5;
            // std::cout << "num_of_modu = " << modu << std::endl;

            if (modu != 0) {
                oneInterToInter temp(currentP.end() - 1 - modu, currentP.end());
                outInfo.push_back(temp);
                //   std::cout << "num_of_ outInfo 2= " << outInfo.size() << std::endl;
            }
            //Determine display format         
            if (userTypeChangeP == false) {
                printINFO(outInfo[0],true);
                curretWindow = 0;
            } else {
                std::cout << "userTypeChangeP "<<std::endl; 
                printINFO(outInfo[curretWindow],true);
            }
        }
    }
         
    //std::cout << "EXIT drawPBP" <<std::endl;
}


//This function is called by the function drawDetails to print all information as a help function 
void printINFO(oneInterToInter& trueList, bool multiple) {
    int outputlint = 1;
    int drawAreaLength = detailsTab.left() - (opened_detailsTab.right() + 20);

    if (curretWindow == 0) {
        unsigned streetID = trueList[0].first.before_SID;
        std::string streetName = mymap->streeNAME[streetID];
        draw_surface(load_png_from_file("forward.png"), opened_detailsTab.right(), bottom_l_y * 0.99 - png_size * 5.4286 + 65);
        drawtext(opened_detailsTab.right() + drawAreaLength / 6, bottom_l_y * 0.99 - png_size * 5.4286 + 80, "Go straight on ");
        drawtext(opened_detailsTab.right() + 20 + drawAreaLength / 2, bottom_l_y * 0.99 - png_size * 5.4286 + 79, streetName);
    }
    else {
        outputlint = 0;
    }
    
    for (int j = 0; j < abs(trueList.size()); j++) {
        /*       if (foundPath.size() > 1 || foundPath_POI.size() > 1) { //multiple pages         
            drawtext(detailsTab.left() - 50, bottom_l_y * 0.99 - png_size * 5.4286 + 10, "Page ");
            std::string str = std::to_string(curretWindow + 1);
            drawtext(detailsTab.left() - 10, bottom_l_y * 0.99 - png_size * 5.4286 + 10, str);
        }
         */
        
        // multiple page
        setfontsize(12);
        if (multiple == true){
             drawtext(detailsTab.left() - 35, bottom_l_y * 0.99 - png_size * 5.4286 + 10, "Page ");
             std::string str = std::to_string(curretWindow + 1);
             drawtext(detailsTab.left() - 10, bottom_l_y * 0.99 - png_size * 5.4286 + 10, str);
        }
 
        //print trueList in order

        unsigned streetID_thisS;
        streetID_thisS = trueList[j].first.after_SID;
        // streetID_nextS = trueList[j].first.after_SID;       
        std::string streetName_this = mymap->streeNAME[streetID_thisS];
        //std::string streetName_next = mymap->streeNAME[streetID_nextS];           
        int strL = streetName_this.length();
        //int addW = 9 * strL; //assume width of each character is 3
        setfontsize(12);

        
        if (trueList[j].first.direction == "Right") {
            draw_surface(load_png_from_file("right_Direction.png"), opened_detailsTab.right(), bottom_l_y * 0.99 - png_size * 5.4286 + 65 + outputlint * 20);
            drawtext(opened_detailsTab.right() + drawAreaLength / 6, bottom_l_y * 0.99 - png_size * 5.4286 + 80 + outputlint * 20, "Turn Right Onto");
        } else if (trueList[j].first.direction == "Left") {
            draw_surface(load_png_from_file("left_Direction.png"), opened_detailsTab.right(), bottom_l_y * 0.99 - png_size * 5.4286 + 65 + outputlint * 20);
            drawtext(opened_detailsTab.right() + drawAreaLength / 6, bottom_l_y * 0.99 - png_size * 5.4286 + 80 + outputlint * 20, "Turn Left Onto");
        }

        drawtext(opened_detailsTab.right() + 20 + drawAreaLength / 2, bottom_l_y * 0.99 - png_size * 5.4286 + 79 + outputlint * 20, streetName_this);

        setcolor(WHITE);

        outputlint++;

    }

    //print the last path to travel to destination 
      /*if (curretWindow == abs(outInfo.size() - 1) || outInfo.size() == 0) {
        unsigned streetID = trueList[trueList.size() - 1].first.after_SID;
        std::string streetName = mymap->streeNAME[streetID];
        drawtext(opened_detailsTab.right() + drawAreaLength / 6, bottom_l_y * 0.99 - png_size * 5.4286 + 80 + outputlint * 20, "Go straight on: ");
        drawtext(opened_detailsTab.right() + 20 + drawAreaLength / 2, bottom_l_y * 0.99 - png_size * 5.4286 + 79 + outputlint * 20, streetName);
    }
*/
    
    
   
}






/// The function is called when user type any bottom at keyboard

void act_on_keyboard(char c, int keysym) {
    only_draw_string = true;
    userTypeChangeP = false;
#ifdef X11

    switch (keysym) {
        case XK_Return:
            special = true;
            //user finish input
            if (!inputString_s1.empty() && inputString_s2.empty()) {
                //user only enter one street name
                draw_inter_streets = false;
                user_enter_string_twice = false;
            } else if (!inputString_s1.empty() && !inputString_s2.empty()) {
                //user enter two street names
                // begin to print information and highlight intersections
                draw_inter_streets = true;
                user_enter_string_twice = true;
            }
            break;
        case XK_BackSpace:
            //user want to delete the character from input string
            special = true;
            if (inS1) {
                if (inputString_s1.size() > 0) {
                    inputString_s1.pop_back(); //(inputString_s1.begin() + (inputString_s1.size() - 1));
                    std::string temp(inputString_s1.begin(), inputString_s1.end());
                    outputString_s1 = temp;

                }
            } else if (inS2) {
                if (outputString_s2.size() > 0) {
                    inputString_s2.pop_back(); //erase(inputString_s2.begin() + (inputString_s2.size() - 1));
                    std::string temp(inputString_s2.begin(), inputString_s2.end());
                    outputString_s2 = temp;
                }
            } else if (inPOI) {
                if (outputString_POI.size() > 0) {
                    inputString_POI.pop_back(); //erase(inputString_s2.begin() + (inputString_s2.size() - 1));
                    std::string temp(inputString_POI.begin(), inputString_POI.end());
                    outputString_POI = temp;
                    // std::cout <<"outputString_POI = " <<outputString_POI <<std::endl;
                }
            }
            break;
        case XK_Tab:
            special = true;
            if (inS1 == true) {
                inS1 = false;
                inS2 = true;
                if (!inputString_s1.empty()) disappear_text_one = true;
                else disappear_text_one = false;
                disappear_text_two = true;
            }
            break;     
        default: special = false;
    }

#endif

    if (c != NULL && !special) {
        user_click_map = false;
        if (inS1) {
            inputString_s1.push_back(c);
            std::string temp(inputString_s1.begin(), inputString_s1.end());
            outputString_s1 = temp;

        } else if (inS2) {
            inputString_s2.push_back(c);
            std::string temp(inputString_s2.begin(), inputString_s2.end());
            outputString_s2 = temp;

        } else if (inPOI) {
            inputString_POI.push_back(c);
            std::string temp(inputString_POI.begin(), inputString_POI.end());
            outputString_POI = temp;
        }
    }
    drawscreen();
}


//This function is called when user press the search icon
void highlight_street_intersections() {
     std::cout << "Enter highlight_street_intersections-------------------------" << std::endl;
    //this function is used to determine whether user input two intersections to highlight a path
    // or user input an intersection and a POI to highlight a path
    if ((inputString_s1.size() == 0 || inputString_s2.size() == 0) && inputString_POI.size() == 0) {
        //user provide too few arguments
        std::cout << "Invalid argument" << std::endl;
    } else {
        if (inter_to_POI == true) { //user enter a POI   
            if (userInputs.POIsName.size() == 0) {
                //first time to enter POI
                foundPath.clear();
                allHighlightSS.clear();
                multiplePath.clear();
                outInfo.clear();
            }
            
            userInputs.POIsName.push_back(outputString_POI);

        } else {
            //user enter intersections
            draw_inter_streets = true;
            std::string street1(inputString_s1.begin(), inputString_s1.end());
            std::string street2(inputString_s2.begin(), inputString_s2.end());
            inter_id_twoStreet = find_intersection_ids_from_street_names(street1, street2);
            if (inter_id_twoStreet.size() == 0) {
                std::cout << "Not Found" << std::endl;
                userInputWrong = true;
            } else {        
                 userInputWrong = false;
                if (userInputs.POIsName.size() != 0) {
                   userInputs. intersID.clear();
                   userInputs. inputIntersections.clear();
                   userInputs.intersWorldP.clear();
                }
                userInputs.intersID.push_back(inter_id_twoStreet[0]);
                userInputs.intersWorldP.push_back((get_world_point(inter_id_twoStreet[0])));              
                std::string inter_name = (mymap->intersectionUOM[inter_id_twoStreet[0]]).intersectionName1;
                if (userInputs.inputIntersections.size() != 0) {
                    //in case user click search icon twice
                    if (userInputs.inputIntersections[userInputs.inputIntersections.size() - 1] != inter_name) userInputs.inputIntersections.push_back(inter_name);
                } else userInputs.inputIntersections.push_back(inter_name); 
                 //zoom into first intersection
                /*  if (userInputs.intersID.size() == 1) {
                 t_point temp_zoom_firstIner = get_world_point(inter_id_twoStreet[0]);
                  Zoom_in_area();   
                }*/
              
                
                
            }             
        }
    }
}

//This function is used to check whether user close the intersection list
void checkBarOpen() {
 std::cout << "Enter checkBarOpen" << std::endl;
    if ((searIconClick == true || interIconClick == false) && userInputs.intersID.size() >0 ) {
        //user enter intersections without close the pop-up window
        int line_height = 25;
        int line_width = 450;
        int cross_icon = 20;
        int POI_first =0;
        allCross.clear();

        if (userInputs.POIsName.size() == 0) {
            for (int i = 0; i < abs(userInputs.inputIntersections.size()); i++) {
                int left_buttom_y = listInter.bottom() + 2 + (1 + i) * line_height;
                //space
                setcolor(WHITE);
                fillrect(listInter.left(), left_buttom_y, listInter.left() + line_width, left_buttom_y - line_height);
                //boundary
                setlinewidth(2);
                setcolor(t_color(140, 141, 140));
                drawrect(listInter.left(), left_buttom_y, listInter.left() + line_width, left_buttom_y - line_height);
                //text
                setcolor(BLACK);
                setfontsize(12);

                t_bound_box tempNew(listInter.left(), left_buttom_y, listInter.left() + cross_icon + 10, listInter.bottom());
                crossArea = tempNew;
                allCross.push_back(t_bound_box(listInter.left(), left_buttom_y, listInter.left() + cross_icon + 10, left_buttom_y - line_height));

                draw_surface(load_png_from_file("cross.png"), listInter.left() + 5, left_buttom_y - line_height + (line_height - cross_icon) / 2);
                drawtext(listInter.left() + cross_icon + (line_width - cross_icon) / 2, left_buttom_y - line_height / 2, userInputs.inputIntersections[i]);
            }
            //std::cout << "allCross size = " << allCross.size() << std::endl; 
        }
        else{
              
             if (userInputs.inputIntersections.size()>0) {
                 POI_first =1;
                int left_buttom_y = listInter.bottom() + 2 + line_height;
                //space
                setcolor(WHITE);
                fillrect(listInter.left(), left_buttom_y, listInter.left() + line_width, left_buttom_y - line_height);
                //boundary
                setlinewidth(2);
                setcolor(t_color(140, 141, 140));
                drawrect(listInter.left(), left_buttom_y, listInter.left() + line_width, left_buttom_y - line_height);
                //text
                setcolor(BLACK);
                setfontsize(12);

                t_bound_box tempNew(listInter.left(), left_buttom_y, listInter.left() + cross_icon + 10, listInter.bottom());
                crossArea = tempNew;
                allCross.push_back(t_bound_box(listInter.left(), left_buttom_y, listInter.left() + cross_icon + 10, left_buttom_y - line_height));

                    draw_surface(load_png_from_file("cross.png"), listInter.left() + 5, left_buttom_y - line_height + (line_height - cross_icon) / 2);
                drawtext(listInter.left() + cross_icon + (line_width - cross_icon) / 2, left_buttom_y - line_height / 2, userInputs.inputIntersections[0]);
             }
             
             for (int i = 0; i < abs(userInputs.POIsName.size()); i++) {

                int left_buttom_y = listInter.bottom() + 2 + (1 + POI_first + i) * line_height;
                //space
                setcolor(WHITE);
                fillrect(listInter.left(), left_buttom_y, listInter.left() + line_width, left_buttom_y - line_height);
                //boundary
                setlinewidth(2);
                setcolor(t_color(140, 141, 140));
                drawrect(listInter.left(), left_buttom_y, listInter.left() + line_width, left_buttom_y - line_height);
                //text
                setcolor(BLACK);
                setfontsize(12);

                t_bound_box tempNew(listInter.left(), left_buttom_y, listInter.left() + cross_icon + 10, listInter.bottom());
                crossArea = tempNew;
                allCross.push_back(t_bound_box(listInter.left(), left_buttom_y, listInter.left() + cross_icon + 10, left_buttom_y - line_height));

                draw_surface(load_png_from_file("cross.png"), listInter.left() + 5, left_buttom_y - line_height + (line_height - cross_icon) / 2);
                drawtext(listInter.left() + cross_icon + (line_width - cross_icon) / 2, left_buttom_y - line_height / 2, userInputs.POIsName[i]);
            }
        }
       
    } 
}

#endif /* INFORMATION_BAR_H */
