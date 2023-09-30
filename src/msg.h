//
// Created by azazo1 on 2023/9/29.
//

#ifndef UNO_MSG_H
#define UNO_MSG_H

// todo define them
#define MSG_NOT_YOUR_TURN (char) 0 // 玩家在非他回合发出了出牌请求, 无 content
#define MSG_PLACE_CARD (char) 1 // 玩家请求出牌, content 为想要发出牌的序号(len:3), 在出 变色 和 +4 牌时还会追加颜色标记(len:1)
#define MSG_INVALID_ARGUMENT (char) 2 // 无效的沟通参数, 无 content
#define MSG_QUERY_CARDS (char) 3 // 玩家查询手牌, 无 content
#define MSG_QUERY_CARDS_RESULT (char) 4 // 玩家查询自己手牌内容的回复, content 为手牌卡牌序号(每个序号三个字符)连接而成
#define MSG_PLAYER_PLACE_CARD (char) 5 // 服务器广播玩家发牌, content 为玩家序号(len:1)和发出卡牌的序号(len:3)
#define MSG_CARD_NOT_IN_YOUR_HAND (char) 6 // 该卡牌不在该玩家手牌中, 无 content
#define MSG_SYNC_GAME_INFO (char) 7 // 查询游戏信息, 无content
#define MSG_SYNC_GAME_INFO_RESULT (char) 8 // 游戏信息同步消息回复, content 为 发送玩家数(len:1)，各个玩家的手牌数(len:玩家数*3)，当前回合玩家序号(len:1)，弃牌堆顶层的牌号(len:3)
#define MSG_CARD_CANNOT_BE_PLACE (char) 9 // 该卡牌不符合发出条件, 不含 content
#define MSG_BAN_PLAYER (char) 10 // 玩家被BAN卡封禁, content 为被封禁玩家的序号(len:1)
#define MSG_REVERT (char) 11 // REVERT 牌被打出, content 为改变后的方向(正方向为1, 反方向为0)
#define MSG_PLUS2 (char) 12 // PLUS2 牌被打出, content为被起作用的玩家, 两个被抽到的牌号(len:3)

/**
 * 读取消息类型
 * */
char parseMsgType(char *msgContent);

/**
 * 读取消息内容
 * */
void parseMsgContent(char *msgContent, char *storage);

#endif //UNO_MSG_H
