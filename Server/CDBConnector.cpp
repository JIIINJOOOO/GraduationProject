#include "CDBConnector.h"
#include <iostream>
using namespace std;

void CDBConnector::AllocateHandle() {
	// Allocate ODBC Handle
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		// OBCD Driver Version Έν½Γ
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			// Allocate Connect Handler
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
				printf("DBConnector ODBC Handle Allocate Success \n");
			else {
				SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state, 
					&native, message, sizeof(message), &length);
				printf("%s : %ld : %ld : %s \n", state, rec, native, message);
			}
		}

		else {
			SQLGetDiagRec(SQL_HANDLE_ENV, henv, ++rec, state,
				&native, message, sizeof(message), &length);
			printf("%s : %ld : %ld : %s \n", state, rec, native, message);
		}
	}
	else {
		SQLGetDiagRec(SQL_HANDLE_ENV, henv, rec, state,
			&native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s \n", state, rec, native, message);
	}
}

void CDBConnector::ConnectDataSource() {
	// Connect MsSQL with ODBC
	retcode = SQLConnect(hdbc, (SQLCHAR*)ODBC_NAME, SQL_NTS,
		(SQLCHAR*)DB_ID, SQL_NTS, (SQLCHAR*)DB_PW, SQL_NTS);
	printf("Complete Connect DB \n");
}

int CDBConnector::ExcuteStatementDirect(SQLCHAR* sql) {
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
		printf("DB Connect Success \n");
	}
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state,
			&native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s \n", state, rec, native, message);
	}

	retcode = SQLExecDirect(hstmt, sql, SQL_NTS);	// Excute SQL

	if (retcode == SQL_SUCCESS)
		printf("Query Suceess \n");
	else {
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, ++rec, state,
			&native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s \n", state, rec, native, message);
		return SQL_EXUTE_FAIL;
	}

	return SQL_EXUTE_OK;
}

void CDBConnector::PrepareStatement(SQLCHAR* sql) {
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
		printf("DB Connect Success \n");
	}
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state,
			&native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s \n", state, rec, native, message);
	}

	retcode = SQLPrepare(hstmt, sql, SQL_NTS);

	if (retcode == SQL_SUCCESS)
		printf("Query Prepate Success\n");
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state,
			&native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s \n", state, rec, native, message);
	}
}

void CDBConnector::ExcuteStatement() {
	retcode = SQLExecute(hstmt);

	if (retcode == SQL_SUCCESS)
		printf("Query Exute Success \n");
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state,
			&native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s \n", state, rec, native, message);
	}
}

bool CDBConnector::RetrieveResult(std::string Cid, std::string Cpass) {
	char AccountId[10], AccouuntPass[10];
	SQLLEN id, pass;

	SQLBindCol(hstmt, 1, SQL_C_CHAR, &AccountId, sizeof(AccountId), &id);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, &AccouuntPass, sizeof(AccouuntPass), &pass);
	
	retcode = SQLFetch(hstmt);

	SQLFreeStmt(hstmt, SQL_UNBIND);
	
	// Delete garbage value - Do Bind ID and pass, value is stored garbage value
	for (int i = Cid.length(); i < 10; ++i)
		AccountId[i] = NULL;
	for (int i = Cpass.length(); i < 10; ++i)
		AccouuntPass[i] = NULL;

	if (string(AccountId) == Cid && string(AccouuntPass) == Cpass) return true;
	return false;
}

void CDBConnector::DisconnectDataSource() {
	if (hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = NULL;
	}

	SQLDisconnect(hdbc);

	if (hdbc) {
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = NULL;
	}

	if (henv) {
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		henv = NULL;
	}
}