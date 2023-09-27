//
// Created by azazo1 on 2023/9/27.
//
#include "card.h"
#include<string.h>

void cardToStr(Card *card, char *output) {
    char rst[4];
    switch (card->signal) {
        case NUMBER_0: {
            strcpy(rst, "n0");
            break;
        }
        case NUMBER_1: {
            strcpy(rst, "n1");
            break;
        }
        case NUMBER_2: {
            strcpy(rst, "n2");
            break;
        }
        case NUMBER_3: {
            strcpy(rst, "n3");
            break;
        }
        case NUMBER_4: {
            strcpy(rst, "n4");
            break;
        }
        case NUMBER_5: {
            strcpy(rst, "n5");
            break;
        }
        case NUMBER_6: {
            strcpy(rst, "n6");
            break;
        }
        case NUMBER_7: {
            strcpy(rst, "n7");
            break;
        }
        case NUMBER_8: {
            strcpy(rst, "n8");
            break;
        }
        case NUMBER_9: {
            strcpy(rst, "n9");
            break;
        }
        case BAN: {
            strcpy(rst, "bn");
            break;
        }
        case REVERT: {
            strcpy(rst, "rv");
            break;
        }
        case PLUS2: {
            strcpy(rst, "p2");
            break;
        }
        case CHANGE_COLOR: {
            strcpy(rst, "cc");
            break;
        }
        case PLUS4: {
            strcpy(rst, "p4");
            break;
        }
    }
    switch (card->color) {
        case R: {
            strcat(rst, "r");
            break;
        }
        case Y: {
            strcat(rst, "y");
            break;
        }
        case B: {
            strcat(rst, "b");
            break;
        }
        case G: {
            strcat(rst, "g");
            break;
        }
        case NO_COLOR: {
            strcat(rst, "N");
            break;
        }
    }
    strcpy(output, rst);
}
