#pragma once
#pragma comment (lib, "odbc32.lib")
#include <Windows.h>
#include <sqlext.h>
#include <stdio.h>
#include <string>

#define ODBC_NAME "LastHero_Server"
#define DB_ID "JaeHyeon_Yu"
#define DB_PW "902711"

#define SQL_EXUTE_FAIL 0
#define SQL_EXUTE_OK 1

struct DB_EVENT {
	int user_id;
	int ev_id;
	char name[10];
	char pass[10];
};

class CDBConnector{
	// connect MSSQL, Use ODBC
private:
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;
	SQLRETURN retcode;

	// for specific error
	SQLSMALLINT length;
	SQLINTEGER rec = 0, native;
	SQLCHAR state[7], message[256];

public:
	CDBConnector() = default;
	~CDBConnector() = default;

	void AllocateHandle(); // Allocate ODBC HANDLE
	void ConnectDataSource(); // Connect DBMS
	int ExcuteStatementDirect(SQLCHAR* sql); // Excute SQL with direct
	void PrepareStatement(SQLCHAR* sql); // Excute SQL with prepare
	void ExcuteStatement(); // Excute prepared SQL
	bool RetrieveResult(std::string id, std::string pass); // Read SQL result

	bool RetrieveResultLogin(int uid);

	bool RetrieveResultUpdate(std::string id);
	void DisconnectDataSource(); // free Handle
};

void DB_Thread();
void AddQuary(int uid, int eid, const char name[], const char pass[]);