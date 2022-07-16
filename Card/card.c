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
/*
    char i = 0;
    cardData ->cardHolderName [i] = getchar();

    while (cardData ->cardHolderName[i++] != '\n'){

        if (i > 24)
        {
            return WRONG_NAME ;
        }

        cardData ->cardHolderName[i] = getchar();
    }

    cardData ->cardHolderName[--i] = '\0';

    if (i < 20)
    {
        return WRONG_NAME;
    
    }
      */
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

/*
    char i = 0;
    cardData ->cardExpirationDate [i] = getchar();

    while (cardData ->cardExpirationDate[i++] != '\n'){

        if (i > 5)
        {
            return WRONG_EXP_DATE ;
        }

        cardData ->cardExpirationDate[i] = getchar();
        
        
    }

    cardData ->cardExpirationDate[--i] = '\0';

    if (i < 5)
    {
        return WRONG_EXP_DATE;
    }
    */
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

    /*
    char i = 0;
    cardData->primaryAccountNumber[i] = getchar() ;


    while (cardData->primaryAccountNumber[i++] != '\n'){

        if (i > 19)
        {
            return WRONG_PAN;
        }


        cardData->primaryAccountNumber[i] = getchar();
    }

    cardData->primaryAccountNumber[--i] = '\0';

    fflush(stdin);

    if (i < 16)
    {
        return WRONG_PAN;
    }
     */
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
