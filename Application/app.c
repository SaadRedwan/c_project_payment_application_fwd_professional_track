/*
 * File:   app.c
 * Date:   8/11/2022
 * Author: Saad Redwan
 */

#include "app.h"

void appStart(void)
{
    ST_cardData_t cardData;
    ST_terminalData_t termData;
    ST_transaction_t transData;

    EN_serverError_t checkResult;

    // Card module: get card data from user.
    getCardHolderName(&cardData);
    getCardExpiryDate(&cardData);
    getCardPAN(&cardData);

    // Teminal module: get terminat data and make some checks.
    setMaxAmount(&termData);
    getTransactionDate(&termData);

    checkResult = isCardExpired(cardData, termData);

    if (checkResult != OK_TERMINAL)
    {
        printf("\nDeclined Expired Card.");
    }
    else
    {
        //to reset the value of checkResult to avoid confusion from last checkResult.
        checkResult =ACCOUNT_NOT_FOUND; 

        getTransactionAmount(&termData);
        checkResult = isBelowMaxAmount(&termData);

        if ( checkResult != OK_TERMINAL)
        {
            printf("\nDeclined Amount Exceeding Limit.");
        }
        else
        {
            //to reset the value of checkResult to avoid confusion from last checkResult.
            checkResult =ACCOUNT_NOT_FOUND; 

            checkResult = isValidAccount(&cardData);

            if(checkResult != OK_TERMINAL)
            {
                printf("\nDecliend invalid account");
            }
            else
            {
                //to reset the value of checkResult to avoid confusion from last checkResult.
                checkResult =ACCOUNT_NOT_FOUND;

                if( checkResult != OK_TERMINAL )
                {
                    printf("\nDecliend insuffecient funds");
                }
                else
                {   
                    EN_transState_t checkResult;
                    checkResult = INTERNAL_SERVER_ERROR;

                    transData = (ST_transaction_t) {0,cardData,termData,0};

                    checkResult = recieveTransactionData(&transData);

                    if(checkResult != APPROVED)
                    {
                        printf("Faild");
                    }
                    else
                    {
                        printf("Success Transactions Process");
                    }
                }
            }
        }
    }
}

void main(void)
{
    printf("Welcom to The Payment Application");
    printf("The Application will start Now");

    appStart();
    
}
