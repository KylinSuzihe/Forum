/*
 Navicat Premium Data Transfer

 Source Server         : Kylin
 Source Server Type    : MySQL
 Source Server Version : 80018
 Source Host           : localhost:3306
 Source Schema         : forum

 Target Server Type    : MySQL
 Target Server Version : 80018
 File Encoding         : 65001

 Date: 10/01/2022 11:18:06
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for comment
-- ----------------------------
DROP TABLE IF EXISTS `comment`;
CREATE TABLE `comment`  (
  `author` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `time` datetime(0) NULL DEFAULT NULL,
  `content` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `post` int(11) NULL DEFAULT NULL,
  `likes_num` int(11) NULL DEFAULT NULL,
  `floor` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of comment
-- ----------------------------
INSERT INTO `comment` VALUES ('kylin', '2022-01-05 17:20:00', '我什么都不会，我只会阿巴巴巴', 10002, 0, 1);
INSERT INTO `comment` VALUES ('kylin', '2022-01-05 22:22:22', 'abab', 10002, 0, 2);
INSERT INTO `comment` VALUES ('fusu', '2022-01-05 18:57:12', '您太强啦！不要装弱啦！', 10001, 0, 2);
INSERT INTO `comment` VALUES ('苏子鹤', '2022-01-05 18:47:12', '哎呀，C++课设实在是太难了，我一点也不会呀，求求大家帮帮我，想想什么题目比较好写，求求了！！！', 10001, 0, 1);
INSERT INTO `comment` VALUES ('kylin', '2022-01-05 02:41:51', '这个帖子人好少呀', 10002, 0, 3);
INSERT INTO `comment` VALUES ('kylin', '2022-01-05 02:42:12', '刚刚输出的啥？', 10002, 0, 4);
INSERT INTO `comment` VALUES ('suzihe', '2022-01-06 14:45:05', 'MySQL还是挺好用的，建议学一学', 10002, 0, 5);
INSERT INTO `comment` VALUES ('suzihe', '2022-01-06 15:41:36', 'wolaifeihuahhh', 10002, 0, 6);
INSERT INTO `comment` VALUES ('kylin', '2022-01-01 09:04:35', '唉，错误好多', 10005, 0, 1);
INSERT INTO `comment` VALUES ('张三', '2022-01-01 11:06:37', '小数和分数怎么相互 转换呀，有没有dalao交一下', 10003, 0, 1);
INSERT INTO `comment` VALUES ('kylin', '2022-01-01 11:01:53', '这个好像挺难得', 10003, 0, 2);
INSERT INTO `comment` VALUES ('苏子鹤', '2022-01-01 11:11:45', '蹲一个回答', 10003, 0, 3);
INSERT INTO `comment` VALUES ('张三', '2022-01-01 11:12:01', '有木有大佬啊求求了', 10003, 0, 4);
INSERT INTO `comment` VALUES ('张三', '2022-01-01 11:12:51', '这个是不是直接用sqrt就行呀', 10004, 0, 1);
INSERT INTO `comment` VALUES ('张三', '2022-01-01 11:14:32', '用递归可以做吗', 10004, 0, 2);

-- ----------------------------
-- Table structure for post
-- ----------------------------
DROP TABLE IF EXISTS `post`;
CREATE TABLE `post`  (
  `id` int(32) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `author` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `reply_num` int(32) NULL DEFAULT NULL,
  `col_num` int(32) NULL DEFAULT NULL,
  `time` datetime(0) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 10010 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of post
-- ----------------------------
INSERT INTO `post` VALUES (10001, '[求助]C++课设怎么做', 'kylin', 2, 1, '2022-01-05 00:00:00');
INSERT INTO `post` VALUES (10002, '[交流]MySQL数据库使用问题', 'suzihe', 6, 1, '2022-01-05 15:18:00');
INSERT INTO `post` VALUES (10003, '[小数分数转换]讨论帖', '张三', 4, 0, '2021-12-10 18:38:51');
INSERT INTO `post` VALUES (10004, '[计算公式：求平均根]讨论帖', '张三', 2, 0, '2021-12-10 08:20:09');
INSERT INTO `post` VALUES (10005, '测试删除功能', 'kylin', 1, 0, '2022-01-01 09:04:35');

-- ----------------------------
-- Table structure for user_post_map
-- ----------------------------
DROP TABLE IF EXISTS `user_post_map`;
CREATE TABLE `user_post_map`  (
  `user` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `post` int(32) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_post_map
-- ----------------------------
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10002);
INSERT INTO `user_post_map` VALUES ('suzihe', 10001);
INSERT INTO `user_post_map` VALUES ('suzihe', 10002);
INSERT INTO `user_post_map` VALUES ('suzihe', 10002);
INSERT INTO `user_post_map` VALUES ('suzihe', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('suzihe', 10001);
INSERT INTO `user_post_map` VALUES ('suzihe', 10002);
INSERT INTO `user_post_map` VALUES ('kylin', 10001);
INSERT INTO `user_post_map` VALUES ('kylin', 10003);
INSERT INTO `user_post_map` VALUES ('kylin', 10008);
INSERT INTO `user_post_map` VALUES ('kylin', 10007);
INSERT INTO `user_post_map` VALUES ('kylin', 10009);
INSERT INTO `user_post_map` VALUES ('kylin', 10007);
INSERT INTO `user_post_map` VALUES ('kylin', 10006);
INSERT INTO `user_post_map` VALUES ('kylin', 10005);
INSERT INTO `user_post_map` VALUES ('kylin', 10005);
INSERT INTO `user_post_map` VALUES ('kylin', 10005);
INSERT INTO `user_post_map` VALUES ('kylin', 10002);
INSERT INTO `user_post_map` VALUES ('张三', 10003);
INSERT INTO `user_post_map` VALUES ('张三', 10003);
INSERT INTO `user_post_map` VALUES ('kylin', 10003);
INSERT INTO `user_post_map` VALUES ('苏子鹤', 10003);
INSERT INTO `user_post_map` VALUES ('苏子鹤', 10003);
INSERT INTO `user_post_map` VALUES ('张三', 10003);
INSERT INTO `user_post_map` VALUES ('张三', 10004);
INSERT INTO `user_post_map` VALUES ('张三', 10004);

SET FOREIGN_KEY_CHECKS = 1;
