#include <stdio.h>
#include "server.h"
#include <string.h>


ST_accountsDB_t current_Card ;
uint8_t  card_index = 0; // used to go directly to the record in updating the balance


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

    fwrite(&transData , sizeof (ST_transaction_t) , 1 , transFilePtr);

    fclose(transFilePtr);

    return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData){
/*
    if(transactionSequenceNumber < 0 || transactionSequenceNumber > 255){
        return ACCOUNT_NOT_FOUND;
    }

    *transData = transactions[transactionSequenceNumber];
*/

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
int compare (ST_accountsDB_t value1 , ST_accountsDB_t value2){

    if(strcmp(value1.primaryAccountNumber , value2.primaryAccountNumber) != 0 )
        return 0;
    else
        return 1;
}

void loadDatabase (HashTable* hashTable){
    ST_accountsDB_t temp;
    FILE* databaseFilePtr;
    uint8_t error ;
    if((databaseFilePtr = fopen("database file.dat","rb+")) == NULL){
        puts("failed to load");
        return;
    }
    while (!feof(databaseFilePtr)){
        error = fread(&temp , sizeof (ST_accountsDB_t) , 1 , databaseFilePtr);

        if(error == 0)
            break;

        hash_table_insert(hashTable , temp.primaryAccountNumber , temp.balance);


    }
    fclose(databaseFilePtr);


}
*/

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




