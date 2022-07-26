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

    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){

    printf("Please enter the Expiry date as 'MM/YY' : ");
    gets(cardData->cardExpirationDate);
    uint32_t i = strlen(cardData->cardExpirationDate);

    if( i != 5 || cardData->cardExpirationDate[2] != '/'){
        return WRONG_EXP_DATE;
    }

    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData){

    printf("Please enter the card PAN: ");
    gets(cardData ->primaryAccountNumber);
    uint32_t i = strlen(cardData ->primaryAccountNumber);

    if( i > 19 ||  i < 16   ){
        return WRONG_PAN;
    }

    return CARD_OK ;
}

/*
int main (void){


    ST_cardData_t buyer ;
    EN_cardError_t error = getCardPAN(&buyer);

    if(error == WRONG_PAN){
        printf("Wrong PAN ");
    } else{
        printf(" correct PAN");
    }
}
 */

/**
* getCardHolderName function testing


int main(){

    ST_cardData_t buyer ;
    EN_cardError_t error = getCardHolderName(&buyer);

    if(error == CARD_OK){

        printf("the name is correct");
        return 0;
    }

    printf("the name is not correct ");

}
*/

/**
* getCardExpiryDate function testing


int main(){

    ST_cardData_t buyer ;
    EN_cardError_t error = getCardExpiryDate(&buyer);

    if(error == CARD_OK){

        printf("the date is correct");
        return 0;
    } else
        printf("the date is not correct");
}
*/

/**
* getCardPAN function testing

int main(){

    ST_cardData_t buyer ;
    EN_cardError_t error = getCardPAN(&buyer);

    if(error == CARD_OK){
        printf("PAN is entered correctly ");
        return 0;
    }
    printf("PAN is entered incorrectly ");

}
*/