#include "application.h"
void appStart(void) {
	ST_cardData_t cardData;
	EN_cardError_t cardErr;
	EN_terminalError_t termErr;
	cardErr = getCardHolderName(&cardData);
	while (cardErr == WRONG_NAME){
		printf("%s\n", "Incorrect name");
		//scanf_s("%*s");
		while ((getchar()) != '\n');
		cardErr = getCardHolderName(&cardData);
	}
	while ((getchar()) != '\n');
	cardErr = getCardExpiryDate(&cardData);
	while (cardErr == EXPIRED_CARD){
		puts("Invalid Expiry Date");
		while ((getchar()) != '\n');
		cardErr = getCardExpiryDate(&cardData);
	}

	while ((getchar()) != '\n');
	cardErr = getCardPAN(&cardData);
	while (cardErr == WRONG_PAN){
		puts("Invalid Card PAN");
		while ((getchar()) != '\n');
		cardErr = getCardPAN(&cardData);
	}
	ST_terminalData_t termData;
	getTransactionDate(&termData);
	if (isCardExpired(cardData, termData) != OK) {
		puts("Expired Card");
		puts("Ending application...");
		return;
	}
	while ((getchar()) != '\n');
	termErr = setMaxAmount(&termData);
	while (termErr == INVALID_MAX_AMOUNT) {
		puts("Invalid max amount");
		while ((getchar()) != '\n');
		termErr = setMaxAmount(&termData);
	}
	while ((getchar()) != '\n');
	termErr = getTransactionAmount(&termData);
	while (termErr == INVALID_AMOUNT) {
		puts("Invalid Amount");
		while ((getchar()) != '\n');
		termErr = getTransactionAmount(&termData);
	}

	if (isBelowMaxAmount(&termData)) {
		puts("Exceeded amount");
		puts("Ending application...");
		return;
	}

	ST_transaction_t transData;
	transData.cardHolderData = cardData;
	transData.terminalData = termData;
	EN_transState_t err = recieveTransactionData(&transData);
	transData.transState = err;
	saveTransaction(&transData);
	//getTransaction(2,&transData);
	if (err != APPROVED) {
		switch (err) {
		case DECLINED_INSUFFECIENT_FUND:
			puts("Declined Insufficient funds");
			break;
		case DECLINED_STOLEN_CARD:
			puts("Declined Stolen Card");
			break;
		case INTERNAL_SERVER_ERROR:
			puts("Internal server error");
			break;
		case FRAUD_CARD:
			puts("Fraud card");
		}
		return;
	}
	else {
		puts("Approved Transaction");
		puts("Ending application...");
		return;
	}
}
