-- Created by Vertabelo (http://vertabelo.com)
-- Last modification date: 2022-04-04 07:36:43.657

-- tables
-- Table: cars
CREATE TABLE cars (
    id int NOT NULL CONSTRAINT cars_pk PRIMARY KEY,
    identifical_number varchar(20) NOT NULL,
    model varchar(15),
    mileage real,
    load_capacity real
);

-- Table: category
CREATE TABLE category (
    id int NOT NULL CONSTRAINT category_pk PRIMARY KEY,
    name varchar(5)
);

-- Table: drivers
CREATE TABLE drivers (
    id int NOT NULL CONSTRAINT drivers_pk PRIMARY KEY,
    personnel_number varchar(35),
    surname varchar(15),
    category_id int NOT NULL,
    driving_experience real,
    address varchar(35),
    birthday_date text,
    CONSTRAINT FK_0 FOREIGN KEY (category_id)
    REFERENCES category (id)
);

-- Table: orders
CREATE TABLE orders (
    id int NOT NULL CONSTRAINT orders_pk PRIMARY KEY,
    status varchar(20),
    delivery_date text,
    driver_id int NOT NULL,
    car_id int NOT NULL,
    mileage real,
    cargo_weight real,
    transportation_cost decimal(19,4),
    CONSTRAINT FK_1 FOREIGN KEY (driver_id)
    REFERENCES drivers (id),
    CONSTRAINT FK_2 FOREIGN KEY (car_id)
    REFERENCES cars (id)
);

-- End of file.

