/*
 * File:   server.h
 * Date:   8/11/2022
 * Author: Saad Redwan
 */

#include "server.h"

#define SIZE_OF_ACCOUNTS_DB 255
#define SIZE_OF_TRANSACTION_DB 255

uint8_t gIndexOfValidAccount;
uint16_t gLastIndexSavedTrans = 0;
static uint32_t gLastSequenceNumber = 1;

ST_accountsDB_t ST_Accounts_DB_t[SIZE_OF_ACCOUNTS_DB] = {
    1500000.00,
    "1561997000000000001",
    1000000.00,
    "1561997000000000002",
    1200000.00,
    "1561997000000000003",
    0.0,
    "1561997000000000004",
    6500000.00,
    "1561997000000000005",
    5000.00,
    "1561997000000000006",
};

ST_transaction_t ST_Transactions_DB_t[SIZE_OF_TRANSACTION_DB] = {0};

static EN_serverError_t isAvailabeAccount(char *PAN, uint8_t*gIndexOfValidAccount)
{
    uint8_t i;
    EN_serverError_t retResult;

    retResult = OK_SERVER;
    for (i = 0; i < SIZE_OF_ACCOUNTS_DB; i++)
    {
        if (strcmp(PAN, ST_Accounts_DB_t[i].primaryAccountNumber) == 0)
        {
            retResult = OK_SERVER;
            *gIndexOfValidAccount = i;
            break;
        }
        else if (strlen(ST_Accounts_DB_t[i].primaryAccountNumber) > 0) // if length of PAN >0 then there are accounts in BD.
        {
            retResult = ACCOUNT_NOT_FOUND;
            continue;
        }
        else
        {
            retResult = ACCOUNT_NOT_FOUND;
            break;
        }
    }

    return retResult;
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    EN_transState_t retResult = APPROVED;

    if (transData == NULL)
    {
        retResult = INTERNAL_SERVER_ERROR;
    }
    else
    {
        EN_serverError_t checkAcountValidity = ACCOUNT_NOT_FOUND;
        EN_serverError_t checkAmountAvilabilty = ACCOUNT_NOT_FOUND;


        checkAcountValidity = isValidAccount(&transData->cardHolderData);
        
        checkAmountAvilabilty = isAmountAvailable(transData);
        

        if (checkAcountValidity != OK_SERVER)
        {
            retResult = DECLINED_STOLEN_CARD;
        }
        else if (checkAmountAvilabilty != OK_SERVER)
        {
            retResult = DECLINED_INSUFFECIENT_FUND;
        }
        else
        {
            EN_serverError_t checkSavedTrans = SAVING_FAILED;

            checkSavedTrans = saveTransaction(transData);

            if (checkSavedTrans != OK_SERVER)
            {
                retResult = INTERNAL_SERVER_ERROR;
            }
            else
            {
                float newBalance = 0.f;
                
                newBalance = ST_Accounts_DB_t[gIndexOfValidAccount].balance - transData->terminalData.transAmount;
                ST_Accounts_DB_t[gIndexOfValidAccount].balance = newBalance;
                transData->transState=APPROVED;
            }
        }
    }

    return retResult;
}
EN_serverError_t isValidAccount(ST_cardData_t *cardData)
{
    EN_serverError_t retResult;
    retResult = ACCOUNT_NOT_FOUND;

    retResult = isAvailabeAccount(cardData->primaryAccountNumber, &gIndexOfValidAccount);

    return retResult;
}
EN_serverError_t isAmountAvailable(ST_transaction_t *transData)
{
    EN_serverError_t retResult = OK_SERVER;
    EN_serverError_t isAvailable = OK_SERVER;

    if (transData == NULL)
    {
        retResult = ACCOUNT_NOT_FOUND;
    }
    else
    {
        isAvailable = isAvailabeAccount(transData->cardHolderData.primaryAccountNumber, &gIndexOfValidAccount);
        
        if (isAvailable == OK_SERVER)
        {          
            if ((transData->terminalData.transAmount) > (ST_Accounts_DB_t[gIndexOfValidAccount].balance))
            {    
                retResult = LOW_BALANCE;
            }
        }
        else
        {
            retResult = ACCOUNT_NOT_FOUND;
        }
    }

    return retResult;
}
EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    EN_serverError_t retResult = OK_SERVER;

    if (transData == NULL)
    {
        transData->transState = INTERNAL_SERVER_ERROR;
        retResult = SAVING_FAILED;
    }
    else
    {
        if (gLastIndexSavedTrans < 255)
        {
            transData->transactionSequenceNumber = gLastSequenceNumber;
            ST_Transactions_DB_t[gLastIndexSavedTrans].transactionSequenceNumber = transData->transactionSequenceNumber;

            strcpy(ST_Transactions_DB_t[gLastIndexSavedTrans].cardHolderData.cardHolderName, transData->cardHolderData.cardHolderName);
            strcpy(ST_Transactions_DB_t[gLastIndexSavedTrans].cardHolderData.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber);
            strcpy(ST_Transactions_DB_t[gLastIndexSavedTrans].cardHolderData.cardExpirationDate, transData->cardHolderData.cardExpirationDate);

            ST_Transactions_DB_t[gLastIndexSavedTrans].terminalData.transAmount = transData->terminalData.transAmount;
            ST_Transactions_DB_t[gLastIndexSavedTrans].terminalData.maxTransAmount = transData->terminalData.maxTransAmount;
            strcpy(ST_Transactions_DB_t[gLastIndexSavedTrans].terminalData.transactionDate, transData->terminalData.transactionDate);

            ST_Transactions_DB_t[gLastIndexSavedTrans].transState = APPROVED;
            transData->transState = APPROVED;

            gLastSequenceNumber++;
            gLastIndexSavedTrans++;
            retResult = OK_SERVER;
        }
        else
        {
            transData->transState = INTERNAL_SERVER_ERROR;
            retResult = SAVING_FAILED;
        }
    }

    return retResult;
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
    EN_serverError_t retResult = OK_SERVER;

    if (transData == NULL)
    {
        retResult = TRANSACTION_NOT_FOUND;
    }
    else if (gLastSequenceNumber > 0)
    {
        uint8_t i;
        
        for (i = 0; i < SIZE_OF_TRANSACTION_DB; i++)
        {
            if (ST_Transactions_DB_t[i].transactionSequenceNumber == transactionSequenceNumber)
            {
                transData->transactionSequenceNumber = transactionSequenceNumber;

                strcpy(transData->cardHolderData.cardHolderName, ST_Transactions_DB_t[i].cardHolderData.cardHolderName);
                strcpy(transData->cardHolderData.primaryAccountNumber, ST_Transactions_DB_t[i].cardHolderData.primaryAccountNumber);
                strcpy(transData->cardHolderData.cardExpirationDate, ST_Transactions_DB_t[i].cardHolderData.cardExpirationDate);

                transData->terminalData.transAmount = ST_Transactions_DB_t[i].terminalData.transAmount;
                transData->terminalData.maxTransAmount = ST_Transactions_DB_t[i].terminalData.maxTransAmount;
                strcpy(transData->terminalData.transactionDate, ST_Transactions_DB_t[i].terminalData.transactionDate);

                transData->transState = ST_Transactions_DB_t[i].transState;

                retResult = OK_SERVER;
                break;
            }
            else
            {
                retResult = TRANSACTION_NOT_FOUND;
            }
        }
    }
    else
    {
        retResult = TRANSACTION_NOT_FOUND;
    }

    return retResult;
}
