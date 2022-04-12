#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

//void addOrder(int )
void getSalaryInfoByPeriod(const char* start, const char* finish, char ** query)
{
    char firstPartOfQuery[300];
    strcpy(firstPartOfQuery, "SELECT driver_id, SUM(transportation_cost*0.2) FROM orders WHERE delivery_date>\"");
    char startDate[100];
    strcpy(startDate,start);
    char moddlePartOfQuery[100];
    strcpy(moddlePartOfQuery, "\" AND delivery_date<\"");
    char finishDate[100];
    strcpy(finishDate,finish);
    char lastPartOfTheQuery[100];
    strcpy(lastPartOfTheQuery, "\" GROUP BY driver_id;");

    *query = strcat(strcat(strcat(strcat(firstPartOfQuery, startDate),moddlePartOfQuery), finishDate), lastPartOfTheQuery);
}

void getSalaryOfDriverByPeriod(const char* start, const char* finish, const char* surname, char ** query)
{
    char firstPartOfQuery[300];
    strcpy(firstPartOfQuery, "SELECT orders.driver_id, SUM(orders.transportation_cost*0.2) FROM orders INNER JOIN drivers ON orders.driver_id=drivers.id  WHERE delivery_date>\"");
    char startDate[100];
    strcpy(startDate,start);
    char middlePartOfQuery1[100];
    strcpy(middlePartOfQuery1, "\" AND delivery_date<\"");
    char finishDate[100];
    strcpy(finishDate,finish);
    char middlePartOfQuery2[100];
    strcpy(middlePartOfQuery2, "\" AND drivers.surname=\"");
    char surnameStr[300];
    strcpy(surnameStr, surname);
    char lastPartOfTheQuery[100];
    strcpy(lastPartOfTheQuery, "\" GROUP BY orders.driver_id;");

    *query = strcat(strcat(strcat(strcat(strcat(strcat(firstPartOfQuery, startDate),middlePartOfQuery1),finishDate),middlePartOfQuery2),surnameStr), lastPartOfTheQuery);
}


int main()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc=1;
    char *sql;

    /* Open database */
    rc = sqlite3_open("autopark.db", &db);

    if( rc ) {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    } else {
       fprintf(stderr, "Opened database successfully\n");
    }

    // Setting the view of selecting
    char *settings = ".mode column .headers on";
    sqlite3_exec(db, settings, callback, 0, &zErrMsg);

    while(1)
    {
        printf("Choose the option:" );
        printf("\n\t1) SELECT");
        printf("\n\t2) INSERT");
        printf("\n\t3) DELETE");
        printf("\n\t4) UPDATE");
        //printf("\n\t5) ");
        printf("\n\t5) get salary info by period");
        printf("\n\t6) get salsry of particular driver by period");
        printf("\n\t7) QUIT");

        printf("\n\nYour choice: ");
        int choice;
        scanf("%d", &choice);

        char firstPartOfQuery[500];
        char answer[500];
        char lastPartOfQuery[500];

        switch(choice)
        {
        case 1:
            printf("Choose the parameter:" );
            printf("\n\t1) by particular driver id");
            printf("\n\t2) by car id");
            printf("\n\t3) show total order count, transporatation cost, weight");
            printf("\n\t4) show info about the driver with the least number of orders");
            printf("\n\t5) show info about the car with the biggest mileage");

            printf("\n\nYour choice: ");
            scanf("%d", &choice);

            switch(choice)
            {
            case 1:
                printf("\n\nEnter the ID of driver to see info about his orders:\n");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "SELECT * from ORDERS where ORDERS.DRIVER_ID=");
                strcpy(lastPartOfQuery, ";");
                // sql = strcat(strcat(firstPartOfQuery, answer), lastPartOfQuery);
                break;
            case 2:
                printf("\n\nEnter the id of car to see the total mileage and total cargo weight:\n");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "SELECT cars.identifical_number, sum(orders.mileage) AS total_mileage, sum(orders.cargo_weight) AS total_weight " \
                                         "FROM orders INNER JOIN cars ON orders.car_id=cars.id " \
                                         "WHERE orders.car_id = ");
                strcpy(lastPartOfQuery, ";");
                //sql = strcat(strcat("SELECT * from ENROLLEE where NAME LIKE '", answer), "%';");
                break;
            case 3:
                //printf("\n\nEnter the city name:\n");
                strcpy(answer, "");

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "SELECT orders.driver_id, count(orders.driver_id) as order_count, sum(orders.cargo_weight) as total_weight, sum(transportation_cost) as total_cost " \
                                         "FROM orders GROUP BY driver_id");
                strcpy(lastPartOfQuery, ";");
                //sql = strcat(strcat("SELECT * from ENROLLEE where CITY=", answer), ";");
                break;
            case 4:
                //printf("\n\nEnter the city name:\n");
                strcpy(answer, "");

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "SELECT * " \
                                         "FROM drivers INNER JOIN " \
                                            "(SELECT driver_order_table.driver_id " \
                                            " FROM " \
                                                "(SELECT driver_id, COUNT(driver_id) AS order_count " \
                                                " FROM orders GROUP BY driver_id) AS driver_order_table " \
                                                " WHERE driver_order_table.order_count = " \
                                                    "(SELECT MIN(order_count) " \
                                                    " FROM " \
                                                        "(SELECT COUNT(driver_id) AS order_count " \
                                                        " FROM orders GROUP BY driver_id))) " \
                                         "AS appropriate_driver_id_table ON appropriate_driver_id_table.driver_id = drivers.id");
                strcpy(lastPartOfQuery, ";");
                //sql = strcat(strcat("SELECT * from ENROLLEE where CITY=", answer), ";");
                break;
            case 5:
                //printf("\n\nEnter the city name:\n");
                strcpy(answer, "");

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "SELECT cars.id, cars.identifical_number, cars.model, cars.load_capacity " \
                                         "FROM cars INNER JOIN (SELECT car_id, MAX(total_mileage) AS max_mileage " \
                                                               "FROM (SELECT orders.car_id, SUM(orders.mileage) AS total_mileage " \
                                                                     "FROM orders " \
                                                                     "GROUP BY orders.car_id)) " \
                                                                "AS cars_max_mileage " \
                                         "WHERE cars.id = cars_max_mileage.car_id;");
                strcpy(lastPartOfQuery, ";");
                //sql = strcat(strcat("SELECT * from ENROLLEE where CITY=", answer), ";");
                break;
            default:
                printf("\n\nWrong input");
                return 0;
            }

            //

            break;
        case 2:
            printf("Choose the table:" );
            printf("\n\t1) cars");
            printf("\n\t2) category");
            printf("\n\t3) drivers");
            printf("\n\t4) orders");

            printf("\n\nYour choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                printf("\n\nEnter values of columns ID,IDENTIFICAL_NUMBER,MODEL,MILEAGE,LOAD_CAPACITY without spaces and with quotes if needed:\n");
                scanf("%s", answer);

                strcpy(firstPartOfQuery, "INSERT INTO cars (ID,IDENTIFICAL_NUMBER,MODEL,MILEAGE,LOAD_CAPACITY) "  \
                                    "VALUES(");
                strcpy(lastPartOfQuery, ");" \
                                    "SELECT * from cars;");
                break;
            case 2:
                printf("\n\nEnter values of columns ID,NAME without spaces and with quotes if needed:\n");
                scanf("%s", answer);

                strcpy(firstPartOfQuery, "INSERT INTO category (ID,NAME) "  \
                                    "VALUES(");
                strcpy(lastPartOfQuery, ");" \
                                    "SELECT * from category;");
                break;
            case 3:
                printf("\n\nEnter values of columns id,personnel_number,surname,category_id,driving_experience,address,birthday_date without spaces and with quotes if needed:\n");
                scanf("%s", answer);

                strcpy(firstPartOfQuery, "INSERT INTO drivers (id,personnel_number,surname,category_id,driving_experience,address,birthday_date) "  \
                                    "VALUES(");
                strcpy(lastPartOfQuery, ");" \
                                    "SELECT * from drivers;");
                break;
            case 4:
                printf("\n\nEnter values of columns id,status,delivery_date,driver_id,car_id,mileage,cargo_weight,transportation_cost without spaces and with quotes if needed:\n");
                scanf("%s", answer);

                strcpy(firstPartOfQuery, "INSERT INTO orders (id,status,delivery_date,driver_id,car_id,mileage,cargo_weight,transportation_cost) "  \
                                    "VALUES(");
                strcpy(lastPartOfQuery, ");" \
                                    "SELECT * from orders;");
                break;
            default:
                printf("\n\nWrong input");
                return 0;


            }

            break;
        case 3:
            printf("Choose the table:" );
            printf("\n\t1) cars");
            printf("\n\t2) category");
            printf("\n\t3) drivers");
            printf("\n\t4) orders");

            printf("\n\nYour choice: ");
            scanf("%d", &choice);

            switch(choice)
            {
            case 1:
                printf("\n\nEnter the id of the row to delete:\n");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "DELETE from CARS where ID=");
                strcpy(lastPartOfQuery, ";" \
                                    "SELECT * from CARS;");
                break;
            case 2:
                printf("\n\nEnter the id of the row to delete:\n");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "DELETE from CATEGORY where ID=");
                strcpy(lastPartOfQuery, ";" \
                                    "SELECT * from CATEGORY;");
                break;
            case 3:
                printf("\n\nEnter the id of the row to delete:\n");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "DELETE from DRIVERS where ID=");
                strcpy(lastPartOfQuery, ";" \
                                    "SELECT * from DRIVERS;");
                break;
            case 4:
                printf("\n\nEnter the id of the row to delete:\n");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "DELETE from ORDERS where ID=");
                strcpy(lastPartOfQuery, ";" \
                                    "SELECT * from ORDERS;");
                break;
            default:
                printf("\n\nWrong input");
                return 0;

            }

            break;
        case 4:
            printf("Choose the table:" );
            printf("\n\t1) cars");
            printf("\n\t2) category");
            printf("\n\t3) drivers");
            printf("\n\t4) orders");

            printf("\n\nYour choice: ");
            scanf("%d", &choice);

            switch(choice)
            {
            case 1:
                printf("\n\nEnter the row \"SET model = ... , identifical_number= ... , ... WHERE id = ...\" ");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "UPDATE CARS ");
                strcpy(lastPartOfQuery, ";" \
                                    "SELECT * from CARS;");
                break;
            case 2:
                printf("\n\nEnter the row \"SET name = ... , ... WHERE id = ...\" ");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "UPDATE category ");
                strcpy(lastPartOfQuery, ";" \
                                    "SELECT * from category;");
                break;
            case 3:
                printf("\n\nEnter the row \"SET personnel_number = ... ,surname= ... , ... WHERE id = ...\" ");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "UPDATE drivers ");
                strcpy(lastPartOfQuery, ";" \
                                    "SELECT * from drivers;");
                break;
            case 4:
                printf("\n\nEnter the row \"SET status= ... ,delivery_date= ... , ... WHERE id = ...\" ");
                scanf("%s", answer);

                // Initialize firstPartOfQuery and lastPartOfQuery arrays
                strcpy(firstPartOfQuery, "UPDATE orders ");
                strcpy(lastPartOfQuery, ";" \
                                    "SELECT * from orders;");
                break;
            default:
                printf("\n\nWrong input");
                return 0;
            }

            break;
        case 5:
        {
            printf("\n\nEnter the start date: ");
            char startDate[300];
            scanf("%s", startDate);
            printf("\n\nEnter the end date: ");
            char endDate[300];
            scanf("%s", endDate);

            char * query;
            getSalaryInfoByPeriod(startDate, endDate, &query);
            strcpy(firstPartOfQuery, query);
            strcpy(answer, "");
            strcpy(lastPartOfQuery, "");
            break;
        }
        case 6:
            printf("\n\nEnter the start date: ");
            char startDate[300];
            scanf("%s", startDate);
            printf("\n\nEnter the end date: ");
            char endDate[300];
            scanf("%s", endDate);
            printf("\n\nEnter the surname: ");
            char surname[300];
            scanf("%s", surname);

            char * query;
            getSalaryOfDriverByPeriod(startDate, endDate,surname, &query);
            strcpy(firstPartOfQuery, query);
            strcpy(answer, "");
            strcpy(lastPartOfQuery, "");
            break;
        case 7:
            return 0;
        default:
            printf("\n\nWrong input");
            return 0;
        }

        sql = strcat(strcat(firstPartOfQuery, answer), lastPartOfQuery);

        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", zErrMsg);
           sqlite3_free(zErrMsg);
        } else {
           fprintf(stdout, "Records created successfully\n");
        }

    }
    sqlite3_close(db);
    return 0;
}
