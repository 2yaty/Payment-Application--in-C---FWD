#include <stdio.h>
#include "server.h"
#include <string.h>


ST_accountsDB_t current_Card ;
uint8_t  card_index = 0; // used to go directly to the record in updating the balance
uint64_t transactionNumber = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData){

    if(isValidAccount(&transData->cardHolderData) != SERVER_OK){
        transData->transState = DECLINED_STOLEN_CARD;
        saveTransaction(transData);
        return DECLINED_STOLEN_CARD;
    }

    if(isBlocked(transData->cardHolderData)) {
        return DECLINED_STOLEN_CARD;
    }

    if(isAmountAvailable(&transData->terminalData) != SERVER_OK){
        transData->transState = DECLINED_INSUFFECIENT_FUND;
        saveTransaction(transData);
        return DECLINED_INSUFFECIENT_FUND;
    }

    updateBalance(transData->terminalData.transAmount);

    transData->transState = APPROVED;

    transData->transactionSequenceNumber = transactionNumber++;

    if(saveTransaction(transData) != SERVER_OK){
        return INTERNAL_SERVER_ERROR;
    }

    return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData){

ST_accountsDB_t  temp;
FILE* databaseFilePtr;
uint8_t  i;
    if ((databaseFilePtr = fopen("../Server/Database/database file.dat" , "rb+")) == NULL){
        return ACCOUNT_NOT_FOUND;
    }

    while (!feof(databaseFilePtr)){
        i = fread(&temp , sizeof (ST_accountsDB_t),1,databaseFilePtr);

        if(i == 0 || temp.primaryAccountNumber[0] == '\0')
            return ACCOUNT_NOT_FOUND;

        if(strcmp(temp.primaryAccountNumber , cardData->primaryAccountNumber) == 0) {
            current_Card = temp;
            fclose(databaseFilePtr);
            return SERVER_OK;
        }
        card_index ++;
    }

    fclose(databaseFilePtr);
    return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData){

    if( termData->transAmount <= current_Card.balance){
        return SERVER_OK;
    }

    return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData){

    FILE* transFilePtr;

    if((transFilePtr = fopen("../Server/Database/transaction file.dat" , "rb+")) == NULL)
        return SAVING_FAILED;

    fwrite(transData , sizeof (ST_transaction_t) , 1 , transFilePtr);

    fclose(transFilePtr);

    return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData){

    FILE* transFilePtr;

    if((transFilePtr = fopen("../Server/Database/transaction file.dat" , "rb+")) == NULL)
        return TRANSACTION_NOT_FOUND;

    fseek(transFilePtr , (transactionSequenceNumber-1) * sizeof (ST_transaction_t) , SEEK_SET);

    if(!fread(transData , sizeof (ST_transaction_t) , 1 , transFilePtr))
        return TRANSACTION_NOT_FOUND;

    fclose(transFilePtr);

    return SERVER_OK;
}

EN_transState_t updateBalance ( float transAmount){

    current_Card.balance -= transAmount;

    FILE* transFilePtr;

    if((transFilePtr = fopen("../Server/Database/database file.dat" , "rb+")) == NULL)
        return INTERNAL_SERVER_ERROR;

    fseek(transFilePtr , card_index * sizeof (ST_accountsDB_t) , SEEK_SET);

    if(!fwrite(&current_Card , sizeof (ST_transaction_t) , 1 , transFilePtr))
        return INTERNAL_SERVER_ERROR;

    fclose(transFilePtr);

    return APPROVED;
}

uint8_t isBlocked (ST_cardData_t cardData){

    uint8_t i;
    ST_cardData_t temp;
    FILE* databaseFilePtr;
    //opening the file in the reading in the binary mode
    if((databaseFilePtr = fopen("../Server/Database/blockList file.dat","rb")) == NULL){
        return 1;
    }
    while (!feof(databaseFilePtr)){

        i = fread(&temp , sizeof (ST_cardData_t) , 1 , databaseFilePtr);

        if(i != 0 && strcmp(temp.primaryAccountNumber , cardData.primaryAccountNumber) == 0) {
            fclose(databaseFilePtr);
            return 1;
        }
    }
    fclose(databaseFilePtr);
    return 0;
}

void writeSomeData (){
    ST_accountsDB_t database1 [255]={{500,"4117394584032808"},
                                    {12000,"5370233590092076"},
                                    {500000,"5078034830297201"}};

    FILE* databaseFilePtr;

    if((databaseFilePtr = fopen("../Server/Database/database file.dat","w")) == NULL){
        puts("out of control");
        return;
    }

    fwrite(database1 , sizeof (ST_accountsDB_t) , 3 , databaseFilePtr);

    fclose(databaseFilePtr);

    if((databaseFilePtr = fopen("../Server/Database/blockList file.dat","w")) == NULL){
        puts("out of control in block list");
        return;
    }

    fwrite(&(database1[2]) , sizeof (ST_accountsDB_t),1,databaseFilePtr);

    fclose(databaseFilePtr);


}
/*
int main (void){

//    ST_accountsDB_t person = {45.5 ,"4564564564655645" };
//
//    HashTable* databaseHashTable = hash_table_new(string_hash , compare);
//
//    hash_table_insert(databaseHashTable , person.primaryAccountNumber , person.balance);
//
//
//    float i = hash_table_lookup(databaseHashTable ,"4564564564655645" );

//    loadDatabase(databaseHashTable);
//
//    float i = hash_table_lookup(databaseHashTable ,"4117394584032808" );

//FILE* blockFilePtr;
//
//if((blockFilePtr = fopen("blockList file.dat" , "w")) == NULL)
//    puts("unable to open the file");
//
//    fclose(blockFilePtr);


}
*/

/**
 * isValidAccount function testing


int main (){
    ST_cardData_t cardData = {"Mohamed Ali Qiaty Ali" , "4117394584032808" , "11/23"};

    EN_serverError_t  error = isValidAccount(&cardData);

    if(error == SERVER_OK)
        puts("valid card");
    else
        puts("Account isn't in the data base ");
}
*/

/**
 * isAmountAvailable function testing

 int main(){

     current_Card.balance = 15000 ;

     ST_terminalData_t  terminalData = { 500 , 5000,"24/07/2022"};

    EN_serverError_t error = isAmountAvailable(&terminalData);

    if(error == SERVER_OK)
        puts("the amount is available ");
    else
        puts("the amount isn't available");
 }
*/

/**
 * saveTransaction function testing

int main(){

    ST_cardData_t cardData = {"Mohamed Ali Qiaty Ali" , "4117394584032808" , "11/23"};
    ST_terminalData_t  terminalData = { 500 , 5000,"24/07/2022"};
    ST_transaction_t transaction={cardData, terminalData,APPROVED,1};

    saveTransaction(&transaction);

    ST_transaction_t temp;

    getTransaction(0,&temp);

    printf("%s\n" , temp.cardHolderData.cardHolderName);
    printf("%s" , temp.cardHolderData.primaryAccountNumber);

}
*/

/**
 * receiveTransactionData function testing

int main(){

    ST_cardData_t cardData = {"Mohamed Ali Qiaty Ali" , "4117394584032808" , "11/23"};
    ST_terminalData_t  terminalData = { 500 , 5000,"24/07/2022"};
    ST_transaction_t transaction={cardData, terminalData,APPROVED,1};

    EN_transState_t error = recieveTransactionData(&transaction);

    switch (error) {

        case DECLINED_INSUFFECIENT_FUND:
            printf("Low Balance");
            return  0;

        case DECLINED_STOLEN_CARD:
            printf("Stolen Card");
            return  0;

        case INTERNAL_SERVER_ERROR :
            printf("Transaction Couldn't be Saved ");
            return  0;

        case APPROVED:
            printf("Transaction is done successfully");
            return  0;

    }
    ST_transaction_t temp;

    getTransaction(0,&temp);

    printf("%s\n" , temp.cardHolderData.cardHolderName);
    printf("%s" , temp.cardHolderData.primaryAccountNumber);

}
 */

