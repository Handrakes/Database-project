#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "project";

void T1(MYSQL* connection);
void T2(MYSQL* connection);
void T3(MYSQL* connection);
void T4(MYSQL* connection);
void T5(MYSQL* connection);

#define MAXLEN 100000

int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	FILE* fp;
	FILE* fp2;
	char input[MAXLEN];
	//char input2[MAXLEN];
	int state = 0;
	fp2 = fopen("20191611_2.txt", "r");

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}

		fp = fopen("20191611.txt", "r");
		if (!fp) {
			printf("ERROR in  File\n");
			return -1;
		}

		char a[100];
		char b[100];
		char c[100];


		while (!feof(fp)) {
			fgets(input, MAXLEN, fp);
			//printf("INPUTs : %s", input);
			sscanf(input, "%s %s %s\n", a, b, c);
			//if (strcmp(a, "delete") == 0) break;
			state = mysql_query(connection, input);
		}

		//const char* query = "select * from student";
		int state = 0;

		//state = mysql_query(connection, query);
		int type = -1;
		while (type) {
			printf("--------- SELECT QUERY TYPES ---------\n\n");
			printf("\t1. TYPE 1\n");
			printf("\t2. TYPE 2\n");
			printf("\t3. TYPE 3\n");
			printf("\t4. TYPE 4\n");
			printf("\t5. TYPE 5\n");
			printf("\t0. QUIT\n");
			printf("--------------------------------------\n");
			printf("Enter the number : ");
			scanf("%d", &type);

			if (type == 1) {
				T1(connection);
			}
			else if (type == 2) {
				T2(connection);
			}
			else if (type == 3) {
				T3(connection);
			}
			else if (type == 4) {
				T4(connection);
			}
			else if (type == 5) {
				T5(connection);
			}
			else if (type == 0) { //program Á¾·á
				printf("----------------------------------\n");
				printf("\tQuit the program!!\n");

				//fp2 = fopen("20191611_2.txt", "r");
				state = mysql_query(connection, input);
				while (!feof(fp2)) {
					fgets(input, MAXLEN, fp2);
					sscanf(input, "%s %s %s\n", a, b, c);
					//printf("INPUT %s", input);
					state = mysql_query(connection, input);
					//printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				}
				break;
			}
			else {
				printf("wrong input! check again\n");
			}

		}
		fclose(fp);
		fclose(fp2);
	}

	return 0;
}

void T1(MYSQL* connection) {
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	int k = -1;
	int truck_number;
	int type;

	while (k) {
		printf("-------------- TYPE 1 --------------\n");
		printf("** Assume truck X is destoryed in a crash. **\n");
		printf("Which X? : ");
		scanf("%d", &truck_number);

		printf("-------- Subtypes in TYPE 1 --------\n");
		printf("\t 1. TYPE I - 1.\n");
		printf("\t 2. TYPE I - 2.\n");
		printf("\t 3. TYPE I - 3.\n\n");

		printf("INPUT : ");
		scanf("%d", &type);

		char query[1000] = "\0";
		if (type == 1) { // find all customers who had a package on the truck at the time of the crash.
			printf("\nhere are customer name, customer id and package id about %d truck\n\n", truck_number);
			sprintf(query, "select c.customer_name, c.customer_id, sub2.package_id\
							from customer as c\
							join (\
								select p.package_id, p.shipping_id, p.customer_id\
								from package as p\
								join (\
									select *\
									from transportation\
									where transportation_id = %d\
								)as sub\
								on p.shipping_id = sub.shipping_id\
							)as sub2 \
							on c.customer_id = sub2.customer_id\
							", truck_number);
		}
		else if (type == 2) { // find all recipients who had a package on that truck at the time of the crash.
			printf("here are the recipients, package_id and customer_id\n");
			//sprintf(query, "select package_id, customer_id, recipient\
			//				from package");
			sprintf(query, "select p.recipient, p.package_id, p.customer_id\
							from package as p\
							join (\
								select *\
								from transportation\
								where transportation_id = %d\
							)as sub\
							on p.shipping_id = sub.shipping_id\
				", truck_number);
		}
		else if (type == 3) { // find the last successful delivery by that truck prior to the crash.
			printf("transportation_id, shipping_id and promised_date about last successful delivery!!\n");/*
			sprintf(query, "select distinct s2.transportation_id, s2.shipping_id, s2.delivered_date\
							from ship_pack_info s1\
							join ship_pack_info s2 on s2.transportation_id != %d\
							order by s2.delivered_date DESC\
							LIMIT 1\
							", truck_number);
							*/
			sprintf(query, "select *\
							from ship_pack_info s1\
							join ship_pack_info s2 on s2.transportation_id = %d\
							where s2.promised_date > s1.delivered_date\
							order by s1.delivered_date DESC\
							LIMIT 1\
							", truck_number);
			//if (query[0] == '\0') printf("No output exist\n");
		}
		else {
			printf("wrong input\n");
		}

		int state = 0;
		state = mysql_query(connection, query);
		//printf("state : %d\n", state);
		if (state == 0) {
			sql_result = mysql_store_result(connection);
			//printf("state : %d\n", state);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("%-10s %-10s %-10s\n", sql_row[0], sql_row[1], sql_row[2]);
			}
			mysql_free_result(sql_result);
		}
		printf("\n\n\n\n");
		printf("-------------SELECT TO RESTART--------------\n\n");
		printf("\t1. RESTART TYPE 1\n");
		printf("\t0. QUIT TO SELECT QUERY TYPES\n\n");
		printf("--------------------------------------------\n");
		scanf("%d", &k);
		if (k == 0) {
			break;
		}
	}


}

void T2(MYSQL* connection) {
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	int k = -1;
	int year;
	while (k) {
		printf("---------- TYPE 2 ----------\n\n");
		printf("** Find the customer who has shipped the most packages in past year **\n");
		printf("Which year? : ");
		scanf("%d", &year);

		char query[1000] = "\0";
		sprintf(query, "select c.customer_id, c.customer_name\
							from customer c\
							join (\
								select customer_id, COUNT(*) as bill_count\
								from bill\
								where YEAR(bill_date) = '%d'\
								group by customer_id\
								order by bill_count DESC\
								LIMIT 1\
							) as sub\
							on c.customer_id = sub.customer_id", year);
		printf("ID \t NAME\n");
		/*
		const char* query = "select customer_id\
							//from (\
								//select *, count(bill_id) as bill_count\
								//from bill\
								where YEAR(bill_date) = YEAR(CURDATE()) - 1\
								group by customer_id\
								order by bill_count DESC\
								limit 1\
							)";
							*/
		int state = 0;
		state = mysql_query(connection, query);
		//printf("state : %d\n", state);
		if (state == 0) {
			sql_result = mysql_store_result(connection);
			//printf("state : %d\n", state);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("%-10s %-15s \n", sql_row[0], sql_row[1]);
			}
			mysql_free_result(sql_result);
		}
		printf("\n\n\n\n");
		printf("-------------SELECT TO RESTART -------------\n\n");
		printf("\t1. RESTART TYPE 2\n");
		printf("\t0. QUIT TO SELECT QUERY TYPES\n");
		printf("--------------------------------------------\n");
		scanf("%d", &k);
		if (k == 0) {
			break;
		}
	}
}

void T3(MYSQL* connection) {
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	int k = -1;
	int year;
	while (k) {
		printf("---------- TYPE 3 ----------\n\n");
		printf("** Find the customer who has spent the most money on shipping in the past year **\n");
		printf("Which year? : ");
		scanf("%d", &year);

		char query[1000] = "\0";
		/*
		sprintf(query, "select c.customer_id, c.customer_name\
							from customer c\
							join (\
								select customer_id, COUNT(*) as bill_count\
								from bill\
								where YEAR(bill_date) = '%d'\
								group by customer_id\
								order by bill_count DESC\
								LIMIT 1\
							) as sub\
							on c.customer_id = sub.customer_id",year);
		*/
		sprintf(query, "select cs.customer_id, cs.customer_name, sub2.s\
						from customer as cs\
						join (\
							select customer_id, sum(charges) as s\
							from package p\
							join (\
								select package_id\
								from bill\
								where year(bill_date) = %d\
							) as sub\
							on p.package_id = sub.package_id\
							group by customer_id\
							order by s DESC\
							LIMIT 1\
						) as sub2\
						on cs.customer_id = sub2.customer_id\
			", year);
		int state = 0;
		state = mysql_query(connection, query);
		if (state == 0) {
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("%-10s %-10s %-10s\n", sql_row[0], sql_row[1], sql_row[2]);
			}
			mysql_free_result(sql_result);
		}
		printf("\n\n\n\n");
		printf("-------------SELECT TO RESTART -------------\n\n");
		printf("\t1. RESTART TYPE 3\n");
		printf("\t0. QUIT TO SELECT QUERY TYPES\n");
		printf("--------------------------------------------\n");
		scanf("%d", &k);
		if (k == 0) {
			break;
		}
	}
}

void T4(MYSQL* connection) {
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	int k = -1;
	//int year;
	while (k) {
		printf("---------- TYPE 4 ----------\n\n");
		printf("** Find the packages that were not delivered within the promised time. **\n\n");
		//printf("Which year? : ");
		//scanf("%d", &year);

		const char* query = "select p.customer_id, p.package_id, sub.promised_date, sub.delivered_date\
							from package as p\
							join (\
								select shipping_id, promised_date, delivered_date\
								from ship_pack_info \
								where promised_date < delivered_date\
							) as sub\
							on p.shipping_id = sub.shipping_id\
							order by p.customer_id\
							";

		printf("Here customer_id, package_id, promised_date, delivered_date about late package.....\n");
		int state = 0;
		state = mysql_query(connection, query);
		if (state == 0) {
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("%-10s %-10s %-15s %-15s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
			}
			mysql_free_result(sql_result);
		}
		printf("\n\n\n\n");
		printf("-------------SELECT TO RESTART -------------\n\n");
		printf("\t1. RESTART TYPE 4\n");
		printf("\t0. QUIT TO SELECT QUERY TYPES\n");
		printf("--------------------------------------------\n");
		scanf("%d", &k);
		if (k == 0) {
			break;
		}
	}
}

void T5(MYSQL* connection) {
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	int year;
	int month;

	int k = -1;
	while (k) {
		printf("------------ TYPE 5 ----------\n\n");
		printf("INPUT year : ");
		scanf("%d", &year);
		printf("INPUT month : ");
		scanf("%d", &month);

		char query[1000] = "\0";
		sprintf(query, "select c.customer_id, c.customer_name, c.address, sub2.s\
						from customer c\
						join (\
							select customer_id, sum(charges) as s\
							from package as p\
							join (\
								select package_id\
								from bill\
								where year(bill_date) = %d and month(bill_date) = %d\
							) as sub\
							on p.package_id = sub.package_id\
							group by customer_id\
						)as sub2\
						on c.customer_id = sub2.customer_id\
						", year, month);
		printf("\n");
		printf("------------------- Bill ------------------\n");
		int state = 0;
		state = mysql_query(connection, query);
		if (state == 0) {
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("------------------------------------------\n");
				printf("\tCustomer ID\t: %s\n", sql_row[0]);
				printf("\tName		: %s\n", sql_row[1]);
				printf("\tAddress		: %s\n", sql_row[2]);
				printf("\tAmount owed\t: %s\n", sql_row[3]);
				printf("------------------------------------------\n");
				//printf("%-10s %-10s %-10s %-10s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
			}
			printf("tips(20%) will charge");
			printf("Thank you for using our services!!!");
			mysql_free_result(sql_result);
		}
		printf("\n\n\n\n");
		printf("-------------SELECT TO RESTART -------------\n\n");
		printf("\t1. RESTART TYPE 4\n");
		printf("\t0. QUIT TO SELECT QUERY TYPES\n");
		printf("--------------------------------------------\n");
		scanf("%d", &k);
		if (k == 0) {
			break;
		}

	}
}



