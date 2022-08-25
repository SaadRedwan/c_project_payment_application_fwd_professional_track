/*
 * File:   app.c
 * Date:   8/11/2022
 * Author: Saad Redwan
 */

#include "app.h"

extern uint8_t gIndexOfValidAccount; 
extern ST_accountsDB_t ST_Accounts_DB_t[];

void appStart(void)
{
    ST_cardData_t cardData;
    ST_terminalData_t termData;
    ST_transaction_t transData;

    EN_serverError_t checkResultServ;
    EN_terminalError_t checkResultTerm;

    // Card module: get card data from user.
    getCardHolderName(&cardData);
    fflush(stdin);
    getCardExpiryDate(&cardData);
    fflush(stdin);
    getCardPAN(&cardData);
    fflush(stdin);

    // Teminal module: get terminat data and make some checks.
    setMaxAmount(&termData);
    getTransactionDate(&termData);
    

    checkResultTerm = isCardExpired(cardData, termData);

    if (checkResultTerm != OK_TERMINAL)
    {
        printf("\nDeclined Expired Card\n");
    }
    else
    {
        //to reset the value of checkResult to avoid confusion from last checkResult.
        checkResultTerm =INVALID_MAX_AMOUNT; 

        getTransactionAmount(&termData);
        fflush(stdin);
        checkResultTerm = isBelowMaxAmount(&termData);

        if ( checkResultTerm != OK_TERMINAL)
        {
            printf("\nDeclined Amount Exceeding Limit");
        }
        else
        {
            //to reset the value of checkResult to avoid confusion from last checkResult.
            checkResultServ =ACCOUNT_NOT_FOUND; 
            checkResultServ = isValidAccount(&cardData);

            if(checkResultServ != OK_TERMINAL)
            {
                printf("\nDecliend invalid account\n");
            }
            else
            {
                //to reset the value of checkResult to avoid confusion from last checkResult.
                checkResultServ =ACCOUNT_NOT_FOUND;

                transData = (ST_transaction_t) {0,cardData,termData,0};

                checkResultServ = isAmountAvailable(&transData);

                if( checkResultServ != OK_TERMINAL )
                {
                    printf("\nDecliend insuffecient funds\n");
                }
                else
                {   
                    EN_transState_t checkResultStat;
                    checkResultStat = INTERNAL_SERVER_ERROR;

                    checkResultStat = recieveTransactionData(&transData);

                    if(checkResultStat != APPROVED)
                    {
                        printf("\nFaild\n");
                    }
                    else
                    {
                        printf("\nSuccess Transactions Process\n");
                        printf("Your Updated balance is %f",ST_Accounts_DB_t[gIndexOfValidAccount].balance);
                    }
                }
            }
        }
    }
}

void main(void)
{
    printf("\t\t\tWelcom to The Payment Application FWD\n");
    printf("\t\t\t The Application will start Now\n\n");

    appStart();
    
}
