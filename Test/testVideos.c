/*
 * File:   forTestVideos
 * Date:   8/11/2022
 * Author: Saad Redwan
 */

#include "Server/server.h"

extern ST_accountsDB_t ST_Accounts_DB_t[];
extern ST_transaction_t ST_Transactions_DB_t[];
extern uint8_t gIndexOfValidAccount;
extern uint8_t gLastIndexSavedTrans;
/*
void main(void)
{
    //test video for getCardHolderName function

    EN_serverError_t testResult=OK_SERVER;

    ST_transaction_t *transData =(ST_transaction_t*) malloc(sizeof(ST_transaction_t));

    getCardPAN(&(transData->cardHolderData));
    getTransactionAmount(&(transData->terminalData));

    testResult = recieveTransactionData(transData);
    

    if(testResult != OK_SERVER)
    {
        printf("\nTest Faild\n\n");

    }
    else
    {
        printf("\nTest Pass\n");
        printf("\nThe new balance of account %s is %f\n\n",
                transData->cardHolderData.primaryAccountNumber,
                ST_Accounts_DB_t[gIndexOfValidAccount].balance);
    }

}
*/
