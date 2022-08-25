/*
 * File:   terminal.c
 * Date:   8/11/2022
 * Author: Saad Redwan
 */

#include "terminal.h"

#define cardMonthStartInd 0 // start index of month of card expiry date
#define cardMonthEndInd 1   // end index of month of card expiry date
#define cardYearStartInd 3  // start index of year of card expiry date
#define cardYearEndInd 4    // end index of year of card expiry date

#define transMonthStartInd 3 // start index of month of transaction date
#define transMonthEndInd 4   // end index of month of transaction date
#define transYearStartInd 6  // start index of year of transaction date
#define transYearEndInd 9    // end index of year of transaction date

extern float gMaxTransAmount;  

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    EN_terminalError_t retResult = OK_TERMINAL;

    //to validate there is a right instance from ST_terminalData_t
    if (termData == NULL)
    {
        retResult = WRONG_DATE;
    }
    else
    {
        time_t CurrentTime = time(NULL);
        struct tm *CurrentDate = localtime(&CurrentTime);

        // Store the date in format of DD/MM/YYYY
        sprintf(termData->transactionDate, "%02d/%02d/%4d",
                CurrentDate->tm_mday,
                CurrentDate->tm_mon + 1,
                CurrentDate->tm_year + 1900);
    }
    return retResult;
}
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    uint8_t cardMonth;
    uint16_t cardYear;
    uint8_t termMonth;
    uint16_t termYear;

    EN_terminalError_t retResult = OK_TERMINAL;

    // cardExpirationDate format MM/YY
    cardMonth = convertDateToInteger(cardData.cardExpirationDate, cardMonthStartInd, cardMonthEndInd);
    cardYear = convertDateToInteger(cardData.cardExpirationDate, cardYearStartInd, cardYearEndInd);

    // transactionDate formate YY/MM/YYYY
    termMonth = convertDateToInteger(termData.transactionDate, transMonthStartInd, transMonthEndInd);
    termYear = convertDateToInteger(termData.transactionDate, transYearStartInd, transYearEndInd);

    cardYear = (termYear / 100) * 100 + cardYear; // so card expiry year date istead of 27 to be 2027 so can be compared with term year date

    if (cardYear == termYear)
    {
        if (cardMonth > termMonth)
        {
            retResult = OK_TERMINAL;
        }
        else
        {
            retResult = EXPIRED_CARD;
        }
    }
    else if (cardYear > termYear)
    {
        retResult = OK_TERMINAL;
    }
    else if (cardYear < termYear)
    {
        retResult = EXPIRED_CARD;
    }

    return retResult;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    float transAmount;
    EN_cardError_t retResult = OK_TERMINAL;

    printf("Please enter the transaction amount and press enter: ");
    scanf("%f", &transAmount);
    printf("\n");
    
    if (termData == NULL)
    {
        retResult = INVALID_AMOUNT;
    }
    else if (transAmount <=0.0f)
    {
        retResult = INVALID_AMOUNT;
    }
    else
    {
        termData->transAmount=transAmount;
        retResult = OK_TERMINAL;
    }

    return retResult;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{

    EN_cardError_t retResult = OK_TERMINAL;

    if (termData == NULL)
    {
        retResult = EXCEED_MAX_AMOUNT;
    }
    else if (termData->transAmount > termData->maxTransAmount || termData->maxTransAmount ==0.0f)
    {
        retResult = EXCEED_MAX_AMOUNT;
    }
    else
    {
        retResult = OK_TERMINAL;
    }

    return retResult;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
    EN_cardError_t retResult = OK_TERMINAL;

     if(termData==NULL)
    {
        retResult=INVALID_MAX_AMOUNT ;
    }
    else if(gMaxTransAmount>0)
    {
        termData->maxTransAmount = gMaxTransAmount;
        printf("\naa%fAA\n",gMaxTransAmount);
    }
    else 
    {
        retResult=INVALID_MAX_AMOUNT;
    }
    
    return retResult;
}
