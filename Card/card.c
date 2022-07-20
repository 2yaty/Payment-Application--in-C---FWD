#include <stdio.h>
#include "card.h"
#include <string.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData){

    printf("Please enter the card name: ");
    gets(cardData->cardHolderName);
    uint32_t i = strlen(cardData->cardHolderName);

    if( i > 24 ||  i < 20  ){
        return WRONG_NAME;
    }

    return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){

    printf("Please enter the Expiry date as 'MM/YY' : ");
    gets(cardData->cardExpirationDate);
    uint32_t i = strlen(cardData->cardExpirationDate);

    if( i != 5  ){
        return WRONG_EXP_DATE;
    }

    return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData){

    printf("Please enter the card PAN: ");
    gets(cardData ->primaryAccountNumber);
    uint32_t i = strlen(cardData ->primaryAccountNumber);

    if( i > 19 ||  i < 16   ){
        return WRONG_PAN;
    }

    return OK ;
}


int main (void){


    ST_cardData_t buyer;
    EN_cardError_t error = getCardHolderName(&buyer);

    if(error == WRONG_NAME){
        printf("Wrong name ");
    } else{
        printf("the name is correct");
    }
}