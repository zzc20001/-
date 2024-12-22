USE `second_hand_trading_system`;

# 随机生成任意长度的字符串
DELIMITER //

CREATE FUNCTION rand_string(len integer)
returns text no sql
BEGIN

 declare counter int default 1;
 declare str text default '';
 
 if len < 1 then
 return null;
 end if;

 while counter <= len do
 set str = concat(str, char(floor(rand() * 94) + 33));
 set counter = counter + 1;
 end while;

 return str;

END

//
DELIMITER ;

# 将记录插入product表

DELIMITER //

CREATE PROCEDURE InsertProduct()
BEGIN
DECLARE i INT DEFAULT 1;

WHILE i <= 10000 DO

INSERT INTO product (name, description, price, category, upload_date, status, user_id) VALUES
(rand_string(floor(3 + rand() * 3)), rand_string(floor(3 + rand() * 3)), ROUND(RAND() * (100 - 1) + 1), rand_string(floor(3 + rand() * 3)), CURRENT_TIMESTAMP(), 'available',  3);

SET i = i + 1;
END WHILE;
END //

DELIMITER ;

# 将记录插入product_images表

DELIMITER //

CREATE PROCEDURE InsertProductImages()
BEGIN
DECLARE i INT DEFAULT 1;
DECLARE j INT DEFAULT 9;

WHILE i <= 10920  AND j <= 10928 DO

INSERT INTO product_images (productid, image_path, created_at) VALUES
(j, "../image/uploaded_image_7.jpg", CURRENT_TIMESTAMP());

SET i = i + 1;
SET j = j + 1;
END WHILE;
END //

DELIMITER ;

# 将记录插入order表

DELIMITER //

CREATE PROCEDURE InsertOrder()
BEGIN
DECLARE i INT DEFAULT 1;

WHILE i <= 10000 DO

INSERT INTO orders (buyer_id, seller_id, product_id, order_date, status) VALUES
(RAND() * 4 + 3, RAND() * 4 + 3, RAND() * (10928 - 7) + 7, CURRENT_TIMESTAMP(), 'completed');

SET i = i + 1;
END WHILE;
END //

DELIMITER ;


CALL InsertProduct();
CALL InsertProductImages();
CALL InsertOrder();