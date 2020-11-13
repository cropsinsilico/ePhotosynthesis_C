/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ePhotosynthesis.cpp
 * Author: friedel
 *
 * Created on November 9, 2020, 3:46 PM
 */

#include <stdlib.h>
#include <string.h>
#include "globals.hpp"
/*
 * 
 */
int main(int argc, char** argv) {
    bool record = false;
    if (argc != 1)
        if (strcmp(argv[0], "record") == 0)
            record = true;
    Sim_Ephotosynthesis(record);
    return (EXIT_SUCCESS);
}

