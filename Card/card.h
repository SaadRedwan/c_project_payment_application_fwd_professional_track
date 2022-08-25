/*
 * File:   card.h
 * Date:   8/11/2022
 * Author: Saad Redwan
 */


#ifndef CARD_H
#define CARD_H
/*
*include usefull libraries
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct ST_cardData_t
{
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[5];
} ST_cardData_t;

typedef enum EN_cardError_t
{
    OK_CARD,
    WRONG_NAME,
    WRONG_EXP_DATE,
    WRONG_PAN
} EN_cardError_t;


EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);


static uint16_t convertDateToInteger(const char *mainString, uint8_t start, uint8_t end)
{
    uint16_t resultInteger;
    uint8_t j = 0;
    uint8_t i;

    char  resultString[4];

    for (i = start; i <= end; ++i)
    {
        resultString[j++] = mainString[i];
    }
    resultString[j] = '\0';

    resultInteger= atoi(resultString);
    
    return resultInteger;
}

#endif //CARD_H