#include "server.h"
//#pragma once
//omar//
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
    EN_serverError_t serverState = isValidAccount(&(transData->cardHolderData));
    if (serverState == ACCOUNT_NOT_FOUND)
        return FRAUD_CARD;
    fclose(fp);
    if (isBlockedAccount(&accountVar) == BLOCKED_ACCOUNT)
        return DECLINED_STOLEN_CARD;
    serverState = isAmountAvailable(&(transData->terminalData));
    if (serverState == LOW_BALANCE)
        return DECLINED_INSUFFECIENT_FUND;
    errno_t err;
    if ((err = fopen_s(&fp, "accountsDB.txt", "r+")) != 0) {
        return INTERNAL_SERVER_ERROR;
    }
    fseek(fp, position, SEEK_SET);
    int numberInt = balanceAmount - transData->terminalData.transAmount;
    char number[12];
    char state[2];
    sprintf_s(number, sizeof(number), "%d", numberInt);
    sprintf_s(state, sizeof(state), "%d", accountVar.state);
    //char PAN[] = "72672826357652456";
    char stored2[36] = "";
    strcat_s(stored2, sizeof(stored2), number);
    //strcat_s(stored2, sizeof(stored2), "     ");
    strcat_s(stored2, sizeof(stored2), " ");
    strcat_s(stored2, sizeof(stored2), state);
    strcat_s(stored2, sizeof(stored2), " ");
    strcat_s(stored2, sizeof(stored2), transData->cardHolderData.primaryAccountNumber);
    fflush(fp);
    fprintf_s(fp, "%s", stored2);
    fflush(fp);
    fclose(fp);
    return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
    errno_t err;
    char buffer[1000];
    char PAN[21];
    PAN[20] = '\0';
    float balance;
    EN_accountState_t accountState;
    if ((err = fopen_s(&fp, "accountsDB.txt", "r+")) != 0) {
        return ACCOUNT_NOT_FOUND;
    }
    while (fgets(buffer, sizeof(buffer), fp)) {
        //printf("%s", buffer);
        sscanf_s(buffer, "%f %d %s", &balance,&accountState,
            PAN, (unsigned)_countof(PAN));
        //printf("%d %s\n", temp_score, temp_name);
        if (!strcmp(PAN, cardData->primaryAccountNumber)) {
            balanceAmount = balance;
            accountVar.state = accountState;
            strcpy_s(cardData->primaryAccountNumber, 20, PAN);
            accountVar.balance = balance;
            return OK;
        }
        //printf("%s %s\n", PAN, cardData->primaryAccountNumber);
        position = ftell(fp);
    }
    return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
    //ST_cardData_t * pCardData = termData.
    //errno_t err;
    //char buffer[1000];
    ////char PAN[21];
    ////int balance;
    //if ((err = fopen_s(&fp, "accountsDB.txt", "r+")) != 0) {
    //    return LOW_BALANCE;
    //}
    if (termData->transAmount > balanceAmount) {
        return LOW_BALANCE;
    }
    //while (fgets(buffer, sizeof(buffer), fp)) {
    //    //printf("%s", buffer);
    //    sscanf_s(buffer, "%d %s", &balance,
    //        PAN, (unsigned)_countof(PAN));
    //    //printf("%d %s\n", temp_score, temp_name);
    //    if (!strcmp(PAN, termData)) {
    //        return OK;
    //    }
    //}
    return OK_server;
    //return EN_serverError_t();
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
    errno_t err;
    if ((err = fopen_s(&fp, "transactionDB.txt", "r")) != 0) {
        return SAVING_FAILED;
    }
    fseek(fp, 0L, SEEK_SET);
    int ch = 0;
    int lines = 0;
    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == '\n')
        {
            lines++;
        }
    }
    fclose(fp);
    if ((err = fopen_s(&fp, "transactionDB.txt", "a")) != 0) {
        return SAVING_FAILED;
    }
    int sequence = (++lines);
    transData->transactionSequenceNumber = sequence;
    int transState = transData->transState;
    fprintf_s(fp, "%d %s %s %s %f %f %s %d\n", sequence, transData->cardHolderData.cardHolderName,
        transData->cardHolderData.primaryAccountNumber, transData->cardHolderData.cardExpirationDate, transData->terminalData.transAmount, transData->terminalData.maxTransAmount,
        transData->terminalData.transactionDate, transState);
    fclose(fp);
    ST_transaction_t transData2;
    if (getTransaction(sequence, &transData2) == TRANSACTION_NOT_FOUND) {
        return SAVING_FAILED;
    }
    printTransactData(&transData2);
    return OK_server;
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountReference) {
    if (accountReference->state == RUNNING)
        return OK_server;
    return BLOCKED_ACCOUNT;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData) {
    errno_t err;
    char buffer[1000];
    char buffer2[1000];
    char buffer3[26];
    buffer3[25] = '\0';
    char PAN[21];
    PAN[20] = '\0';
    unsigned int sequence;
    if ((err = fopen_s(&fp, "transactionDB.txt", "r+")) != 0) {
        return INTERNAL_SERVER_ERROR;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        //printf("%s", buffer);
        //char* initialBuffer = buffer;
        sscanf_s(buffer, "%d", &sequence);
        //char* endBuffer = buffer;
        //printf("%d %s\n", temp_score, temp_name);
        if (sequence == transactionSequenceNumber) {
            int i = 0;
            int sequenceLen;
            while (isdigit(buffer[i++]));
            sequenceLen = (i-1);
            while (!isdigit(buffer[i++]));
            memcpy(buffer2, buffer + i - 1, sizeof(buffer) - (i-1));
            memcpy(buffer3, buffer + sequenceLen + 1, (i-2-sequenceLen));
            buffer3[i - 3 - sequenceLen] = '\0';
            transData->transactionSequenceNumber = sequence;
            strcpy_s(transData->cardHolderData.cardHolderName, 25, buffer3);
            sscanf_s(buffer2, "%s %s %f %f %s %d", transData->cardHolderData.primaryAccountNumber, (unsigned)_countof(transData->cardHolderData.primaryAccountNumber), transData->cardHolderData.cardExpirationDate, (unsigned)_countof(transData->cardHolderData.cardExpirationDate), &transData->terminalData.transAmount,
                &transData->terminalData.maxTransAmount, transData->terminalData.transactionDate, (unsigned)_countof(transData->terminalData.transactionDate), &transData->transState);
            ///////////////////////
            return OK_server;
        }
        //printf("%s %s\n", PAN, cardData->primaryAccountNumber);
        //position = ftell(fp);
    }
    return TRANSACTION_NOT_FOUND;
}

void printTransactData(ST_transaction_t* transData) {
    printf("%d %s %s %f %f %s %d\n",transData->transactionSequenceNumber, transData->cardHolderData.primaryAccountNumber, transData->cardHolderData.cardExpirationDate, transData->terminalData.transAmount,
        transData->terminalData.maxTransAmount, transData->terminalData.transactionDate, transData->transState);
    return;
}
