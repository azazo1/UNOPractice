//
// Created by azazo1 on 2023/9/29.
//

#ifndef UNO_MSG_H
#define UNO_MSG_H

// 跳过 (char) 0 防止与 \0 重复
#define MSG_PLACE_CARD (char) 1 // 玩家请求出牌, content 为想要发出牌的序号(len:3, 不足三位用零补充), 在出 变色 和 +4 牌时还会追加颜色标记(len:1)
#define MSG_INVALID_ARGUMENT (char) 2 // 服务器通知玩家无效的沟通参数, 无 content
#define MSG_QUERY_CARDS (char) 3 // 玩家查询手牌, 无 content
#define MSG_QUERY_CARDS_RESULT (char) 4 // 服务器回复玩家查询自己手牌内容, content 为手牌卡牌序号(每个序号三个字符)连接而成
#define MSG_PLAYER_PLACE_CARD (char) 5 // 服务器广播玩家发牌, content 为玩家序号(len:1)和发出卡牌的序号(len:3)
#define MSG_CARD_NOT_IN_YOUR_HAND (char) 6 // 服务器回复玩家该卡牌不在该玩家手牌中, 无 content
#define MSG_SYNC_GAME_INFO (char) 7 // 玩家查询游戏信息, 无 content
#define MSG_SYNC_GAME_INFO_RESULT (char) 8 // 服务器回复游戏信息同步消息, content 为 发送玩家数(len:1), 查询者自身的玩家序号(len:1), 各个玩家的手牌数(len:玩家数*3)，当前回合玩家序号(len:1)，弃牌堆顶层的牌号(len:3)
#define MSG_CARD_CANNOT_BE_PLACE (char) 9 // 服务器回复该卡牌不符合发出条件, 不含 content
// 跳过回车符
#define MSG_REVERT (char) 11 // 服务器广播 REVERT 牌被打出, content 为改变后的方向(正方向为1, 反方向为0)
#define MSG_PLUS2 (char) 12 // 服务器广播 PLUS2 牌被打出, content 为被起作用的玩家序号(len:1), 两个被抽到的牌号(len:2*3) 非该玩家的客户端应忽略该牌号
#define MSG_PLUS4 (char) 13 // 服务器广播 PLUS4 牌被打出, content 为被加四张牌的玩家序号(len:1), 新的可用颜色代号(len:1), 被加上的牌的牌号(len4*3) 非该玩家的客户端应忽略牌号
#define MSG_CHANGE_COLOR (char) 14 // 服务器广播 CHANGE_COLOR 牌被打出, content 为新的可用颜色(len:1)
#define MSG_PASS (char) 15 // 玩家选择不出牌, 过牌, 无 content
#define MSG_PLAYER_PASSED (char) 16 // 服务器广播玩家过牌, content 为过牌玩家序号(len:1), 玩家抽取的两张牌(len:2*3)
#define MSG_GAME_OVER (char) 17 // 服务器广播游戏结束, content 为每位玩家的得分(len:5)连接，按玩家序号排序
#define MSG_DOUBT (char) 18 // 某位玩家质疑另一位玩家没有宣言 UNO, content 为被质疑的玩家
#define MSG_PLAYER_DOUBTED (char) 19 // 服务器广播某位玩家质疑另外一位玩家, content 为是否质疑成功(len:1), 质疑者(len:1), 被质疑者(len:1), 若质疑成功则追加两张牌的牌号(len:2*3), 非被质疑玩家的客户端应忽略此牌号
#define MSG_UNO (char) 20 // 玩家宣布 UNO, 无 content
#define MSG_PLAYER_UNOED (char) 21 // 服务器广播玩家uno, content 为uno玩家的序号(len:1)
#define MSG_UNO_FAILED (char) 22 // 服务器通知尝试UNO的玩家UNO失败, 无 content
#define MSG_GAME_START (char) 23 // 服务器广播游戏开始, 无 content
#define MSG_GAME_OVER_WITH_EXCEPTION (char) 24 // 服务器广播游戏异常结束, 无 content
#define MSG_ITS_YOUR_TURN (char) 25 // 服务器通知玩家到他的回合, 无 content
#define MSG_BAN_PLAYER (char) 26 // 服务器广播玩家被BAN卡封禁, content 为被封禁玩家的序号(len:1)
#define MSG_GAME_OVER_WITH_CARDLIB_EXHAUSTED (char) 27 // 牌库不足并结束游戏, content 为每位玩家的得分(len:5)连接，按玩家序号排序
#define MSG_NOT_YOUR_TURN (char) 28 // 服务器通知玩家在非他回合发出了出牌/质疑/UNO请求, 无 content

/**
 * 读取消息类型
 * */
char parseMsgType(char *rawMsg);

/**
 * 读取消息内容, 存放到storage中, length为长度
 * */
void parseMsgContent(char *rawMsg, char *storage, int *length);

#endif //UNO_MSG_H
