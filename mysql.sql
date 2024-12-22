CREATE DATABASE IF NOT EXISTS `second_hand_trading_system`;
USE `second_hand_trading_system`;

CREATE TABLE `user` (
    `user_id` int NOT NULL AUTO_INCREMENT,
    `name` varchar(50) NOT NULL,
    `contact_info` varchar(100) DEFAULT NULL,
    `email` varchar(100) DEFAULT NULL,
    `password` varchar(255) NOT NULL,
    `role` enum('buyer','seller') NOT NULL,
    `registration_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`user_id`),
    UNIQUE KEY `email_UNIQUE` (`email`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `product` (
    `product_id` int NOT NULL AUTO_INCREMENT,
    `name` varchar(100) NOT NULL,
    `description` text,
    `price` decimal(10,2) NOT NULL,
    `category` varchar(100) DEFAULT NULL,
    `upload_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    `status` enum('available','sold') DEFAULT 'available',
    `user_id` int DEFAULT NULL,
    PRIMARY KEY (`product_id`),
    KEY `seller_id_idx` (`user_id`),
    CONSTRAINT `fk_Product_1_user_id` FOREIGN KEY (`user_id`) REFERENCES `User` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `orders` (
    `order_id` int NOT NULL AUTO_INCREMENT,
    `buyer_id` int DEFAULT NULL,
    `seller_id` int DEFAULT NULL,
    `product_id` int DEFAULT NULL,
    `order_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    `status` enum('completed','pending','cancelled') DEFAULT 'pending',
    PRIMARY KEY (`order_id`),
    KEY `buyer_id` (`buyer_id`),
    KEY `seller_id` (`seller_id`),
    KEY `product_id` (`product_id`),
    CONSTRAINT `Order_ibfk_3` FOREIGN KEY (`product_id`) REFERENCES `Product` (`product_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `product_images` (
    `imageid` int NOT NULL AUTO_INCREMENT,
    `productid` int NOT NULL,
    `image_path` varchar(255) NOT NULL,
    `created_at` datetime DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`imageid`),
    KEY `productid` (`productid`),
    CONSTRAINT `product_images_ibfk_1` FOREIGN KEY (`productid`) REFERENCES `Product` (`product_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `private_message` (
  `id` int NOT NULL,
  `uid_from` int NOT NULL,
  `uid_to` int NOT NULL,
  `message_content` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `message` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int NOT NULL,
  `message_content` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `date` timestamp NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;