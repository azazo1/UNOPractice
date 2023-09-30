//
// Created by azazo1 on 2023/9/27.
//

#ifndef UNO_CARD_H
#define UNO_CARD_H
#define TRUE 1
#define FALSE 0
// 卡牌符号合集 {
#define NUMBER_0 0
#define NUMBER_1 1
#define NUMBER_2 2
#define NUMBER_3 3
#define NUMBER_4 4
#define NUMBER_5 5
#define NUMBER_6 6
#define NUMBER_7 7
#define NUMBER_8 8
#define NUMBER_9 9
#define BAN 10
#define REVERT 11
#define PLUS2 12
#define CHANGE_COLOR 13 // 没有颜色
#define PLUS4 14 // 没有颜色
// }

// 卡牌颜色合集 {
#define R 'r'
#define G 'g'
#define B 'b'
#define Y 'y'
#define NO_COLOR 'n'
// }
#define MAX_CARDS 108

typedef struct { // UNO的一种卡
    char signal; // 卡牌符号
    char color;
} Card;

void cardToStr(Card *card, char *output); // 把卡牌转换成简单的字符串，该字符串应保证能装下3个字符(\0算上则要4位，\0由此函数添加)

#endif //UNO_CARD_H
