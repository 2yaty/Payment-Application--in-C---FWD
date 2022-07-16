#include <stdio.h>
#include "card.h"
#include <string.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData){
    uint8_t temp [26];
    printf("Please enter the card name: ");

    gets(temp);
    uint32_t i = strlen(temp);
    if( i > 24 ||  i < 20  ){

        return WRONG_NAME;
    }
    strcpy(cardData->cardHolderName , temp);

    return OK;

    
    
}
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){

    uint8_t temp [7];

    printf("Please enter the Expiry date as 'MM/YY' : ");

    gets(temp);
    uint32_t i = strlen(temp);
    if( i != 5  ){

        return WRONG_EXP_DATE;
    }
    strcpy(cardData->cardExpirationDate , temp);

    return OK;

    
}
EN_cardError_t getCardPAN(ST_cardData_t *cardData){

    uint8_t temp [21];
    printf("Please enter the card PAN: ");

    gets(temp);
    uint32_t i = strlen(temp);
    if( i > 19 ||  i < 16   ){

        return WRONG_PAN;
    }
    strcpy(cardData ->primaryAccountNumber , temp);

    return OK ;
}

/*
int main(void){

    EN_cardError_t error ;

    ST_cardData_t newCard ;

    error = getCardHolderName(&newCard);

    printf("\n%s\n" , newCard.cardHolderName);

    if (error != OK)
    {
        printf("\nwrong name");
        return 0;
    }
    

    

    error = getCardExpiryDate(&newCard);
    
    printf("\n%s\n", newCard.cardExpirationDate);

    if (error != OK)
    {
        printf("\nwrong date");
        return 0;
    }

    

    error =getCardPAN(&newCard);
    
    printf("\n%s\n" , newCard.primaryAccountNumber);

    if (error != OK)
    {
        printf("\nwrong num");
        return 0;
    }

    

    return 0;
}
*/
