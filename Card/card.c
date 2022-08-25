/*
 * File:   card.c
 * Date:   8/11/2022
 * Author: Saad Redwan
 */

#include "card.h"

#define cardMonthStartInd   0
#define cardMonthEndInd     1
#define cardYearStartInd    3
#define cardYearEndInd      4

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    uint8_t cardHolderName[25];
    uint8_t cardHolderNameLength;
    EN_cardError_t retResult = OK_CARD;
    EN_cardError_t isChar=OK_CARD;

    printf("\n");
    printf("Please enter the card holder name then press enter: ");
    scanf("%[^\n]s", cardHolderName);
    printf("\n");

    cardHolderNameLength = strlen(cardHolderName);
    // printf("%d\n",cardHolderNameLength);
    if(cardData==NULL)
    {
        retResult=WRONG_NAME;
    }
    else if (cardHolderNameLength < 25 && cardHolderNameLength > 19)
    {
        int i;

         for (i = 0; cardHolderName[i] != '\0'; i++)
        {
            if((cardHolderName[i]>='a' && cardHolderName[i]<='z') 
                || (cardHolderName[i]>='A' && cardHolderName[i]<='Z') 
                || cardHolderName[i]==' ')
            {
                continue;
            }
            else
            {
                retResult = WRONG_NAME;
                isChar = WRONG_NAME;
                break;
            }
        }
        if (isChar == OK_CARD)
        {
            strcpy(cardData->cardHolderName, cardHolderName);
        }
    }
    else
    {
        retResult = WRONG_NAME;
    }

    return retResult;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    uint8_t cardExpirationDate[5];
    uint8_t cardExpirationDateLength;
    EN_cardError_t retResult = OK_CARD;
    
    //To validate the month and year if it bigger then 12 or less than 1
    uint8_t month;
    uint8_t year;

    printf("Please enter the expiry date (format \"MM/YY\") then press enter: ");
    scanf("%[^\n]s", cardExpirationDate);
    printf("\n");

    month = convertDateToInteger(cardExpirationDate,cardMonthStartInd,cardMonthEndInd);
    year = convertDateToInteger(cardExpirationDate,cardYearStartInd,cardYearEndInd);
    cardExpirationDateLength = strlen(cardExpirationDate);

    if(cardData==NULL)
    {
        retResult=WRONG_NAME;
    }
    else if (cardExpirationDateLength == 5 && cardExpirationDate[2] == '/' && year>0 && month>0 && month <13)
    {
        strcpy(cardData->cardExpirationDate, cardExpirationDate);
    }
    else
    {
        retResult = WRONG_EXP_DATE;
    }

    return retResult;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    uint8_t primaryAccountNumber[20];
    uint8_t primaryAccountNumberLength;
    EN_cardError_t retResult = OK_CARD;
    EN_cardError_t isNumeric = OK_CARD;

    printf("Please enter the Primary Account Numbe (PAN) then press enter (note: numbers only): ");
    scanf("%[^\n]s", primaryAccountNumber);
    printf("\n");

    primaryAccountNumberLength = strlen(primaryAccountNumber);

    if(cardData==NULL)
    {
        retResult=WRONG_NAME;
    }
    else if (primaryAccountNumberLength > 15 && primaryAccountNumberLength < 20)
    {
        int i;

        for (i = 0; primaryAccountNumber[i] != '\0'; i++)
        {
            if(primaryAccountNumber[i]>='0' && primaryAccountNumber[i]<='9')
            {
                continue;
            }
            else
            {
                retResult = WRONG_PAN;
                isNumeric = WRONG_PAN;
                break;
            }
        }
        if (isNumeric == OK_CARD)
        {
            strcpy(cardData->primaryAccountNumber, primaryAccountNumber);
        }
    }
    else
    {
        retResult = WRONG_PAN;
    }

    return retResult;
}
