#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char strTemp[26];
	strTemp[0] = '\0';
	printf("Please enter name:");
	scanf_s("%25[^\n]", strTemp, 26);
	size_t len = strlen(strTemp);
	//printf("%d\n", len);
	if (len == 0)
		return WRONG_NAME;
	//printf("%d\n", strlen(strTemp));
	if ((len >= 25) || (len< 20))
		return WRONG_NAME;
	strcpy_s(cardData->cardHolderName, 25, strTemp);
	return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	char strTemp[7];
	char monthSubString[3];
	strTemp[0] = '\0';
	printf("Please enter card expiry date:");
	scanf_s("%6[^\n]", strTemp, 7);
	size_t len = strlen(strTemp);
	//printf("%d \n", strlen(strTemp));
	int month = 0;
	if (len == 5 && strTemp[2] == '/') {
		memcpy(monthSubString, strTemp, 2);
		for (int i = 0; i < 2; i++) { //io/88
			if (isdigit(strTemp[i])) {
				month += (strTemp[i] - '0') * (int)pow(10, (1 - i));
			}
			else {
				return WRONG_EXP_DATE;
			}
		}
		if (month > 12 || month < 1) {
			return WRONG_EXP_DATE;
		}
		for (int i = 3; i < 5; i++) {  //check that the year value is numeric
			if (!isdigit(strTemp[i])) {
				return WRONG_EXP_DATE;
			}
		}
	}
	else {
		return WRONG_EXP_DATE;
	}
	strcpy_s(cardData->cardExpirationDate, 6, strTemp);
	return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	char strTemp[21];
	strTemp[0] = '\0';
	printf("Please enter card primary account number:");
	scanf_s("%20[^\n]", strTemp,21);
	size_t len = strlen(strTemp);
	//printf("%d \n", len);
	if (len < 16 || len > 19) {
		return WRONG_PAN;
	}
	strcpy_s(cardData->primaryAccountNumber, 20, strTemp);
	return OK;
}
