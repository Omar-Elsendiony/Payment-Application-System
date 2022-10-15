#include "terminal.h"
#pragma once

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	char strTemp[11];
	struct tm newtime;
	time_t tm;
	tm = time(0);
	localtime_s(&newtime, &tm);
	int month = 1 + newtime.tm_mon;
	int year = 1900 + newtime.tm_year;
	int day = newtime.tm_mday;
	//printf("%d \n", year);
	strTemp[0] = (day / 10) + '0';
	strTemp[1] = (day % 10) + '0';
	strTemp[2] = '/';
	strTemp[3] = (month / 10) + '0';
	strTemp[4] = (month % 10) + '0';
	strTemp[5] = '/';
	strTemp[6] = (year / 1000) + '0';
	strTemp[7] = ((year % 1000) / 100) + '0';
	strTemp[8] = ((year % 100) / 10) + '0';
	strTemp[9] = year % 10 + '0';
	strTemp[10] = '\0';
	strcpy_s(termData->transactionDate, 11, strTemp);
	return OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	char currentYear[3];
	memcpy(currentYear, (char*)&(termData.transactionDate) + 8, 2); //22
	currentYear[2] = '\0';
	char expiryYear[3];
	memcpy(expiryYear, (char*)&(cardData.cardExpirationDate) + 3, 2);
	expiryYear[2] = '\0';
	int cYear = atoi(currentYear);
	int eYear = atoi(expiryYear);
	if (cYear > eYear) {
		return EXPIRED_CARD;
	}
	else if (cYear < eYear)
		return OK_term;
	char currentMonth[3];
	memcpy(currentMonth, (char*)&(termData.transactionDate) + 3, 2);
	currentMonth[2] = '\0';
	char expiryMonth[3];
	memcpy(expiryMonth, (char*)&(cardData.cardExpirationDate), 2);
	expiryMonth[2] = '\0';
	int cMonth = atoi(currentMonth);
	int eMonth = atoi(expiryMonth);
	if (cMonth > eMonth) {
		return EXPIRED_CARD;
	}
	//char* currentYear = termData.transactionDate
	return OK_term;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float transAmount;
	//scanf_s("%6[^\n]f", transAmount, 7);
	printf("Please enter transaction amount:");
	scanf_s("%f", &transAmount);
	if (transAmount <= 0)
		return INVALID_AMOUNT;
	termData->transAmount = transAmount;
	return OK_term;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->maxTransAmount < termData->transAmount) {
		return EXCEED_MAX_AMOUNT;
	}
	return OK_term;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	float maxAmount;
	//scanf_s("%6[^\n]f", transAmount, 7);
	printf("Max transaction amount:");
	scanf_s("%f", &maxAmount);
	if (maxAmount <= 0)
		return INVALID_MAX_AMOUNT;
	termData->maxTransAmount = maxAmount;
	return OK_term;
}
