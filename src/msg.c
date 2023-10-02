//
// Created by azazo1 on 2023/10/2.
//
#include <string.h>
#include "msg.h"

char parseMsgType(char *rawMsg) {
    return rawMsg[0];
}

void parseMsgContent(char *rawMsg, char *storage, int *length) {
    strcpy(storage, ++rawMsg);
    *length = (int) strlen(storage);
}