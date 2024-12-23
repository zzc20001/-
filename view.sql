CREATE VIEW user_info AS
    SELECT name, email, password, contact_info
    FROM user;

CREATE VIEW product_info AS
    SELECT t1.name, t1.description, t1.price, t1.category, t1.status, t2.image_path
    FROM product as t1, product_images as t2 
    WHERE t1.product_id = t2.productid;

CREATE VIEW orders_info AS
    SELECT order_id, buyer_id, seller_id, product_id, status
    FROM orders;