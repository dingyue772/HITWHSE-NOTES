/*
 Navicat MySQL Data Transfer

 Source Server         : localmysql
 Source Server Type    : MySQL
 Source Server Version : 80023
 Source Host           : localhost:3306
 Source Schema         : flaskapp

 Target Server Type    : MySQL
 Target Server Version : 80023
 File Encoding         : 65001

 Date: 04/09/2023 17:22:34
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for t_indictment
-- ----------------------------
DROP TABLE IF EXISTS `t_indictment`;
CREATE TABLE `t_indictment`  (
  `indictment_name` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL COMMENT '起诉书名称',
  `person_name` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL COMMENT '操作人',
  `status` int NOT NULL COMMENT '识别状态:1=已识别,0=未识别',
  `create_time` datetime NULL DEFAULT NULL COMMENT '创建时间',
  `update_time` datetime NULL DEFAULT NULL COMMENT '更新时间',
  PRIMARY KEY (`indictment_name`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci COMMENT = '起诉书信息' ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
