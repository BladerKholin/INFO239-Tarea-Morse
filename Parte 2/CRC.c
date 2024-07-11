#include<stdio.h>  
#include<string.h>  

// length of the generator polynomial  
#define N strlen(gen_poly)  

// CRC value  
//char check_value[64]; // Aumentado para asegurar suficiente espacio  
// generator polynomial  
char gen_poly[10] = "100101";  

// variables   
int data_length, i, j;  

// function that performs XOR operation  
void XOR(char *check_value) {  
    for (j = 1; j < N; j++)  
        check_value[j] = ((check_value[j] == gen_poly[j]) ? '0' : '1');  
}  

// Function to check for errors on the receiver side  
void receiver(char *data, char *check_value) {  
    // get the received data  
    printf("Data received: %s\n", data);
    printf("check_value received: %s\n", check_value);
    
    char result[strlen(data)+strlen(check_value)-1];
    result[0] = '\0';
    strcat(result, data);
    strcat(result, check_value);
    
    printf("El resultado de la concatenación es: %s\n", result);

    // Cyclic Redundancy Check  
    crc(data, check_value);  

    // Check if the remainder is zero to find the error  
    for (i = 0; (i < N - 1) && (check_value[i] != '1'); i++);  
    if (i < N - 1)  
        printf("\nError detected\n\n");  
    else  
        printf("\nNo error detected\n\n");  
}  

void crc(char *data,char *check_value) {  
    // initializing check_value  
    for (i = 0; i < N; i++)  
        check_value[i] = data[i];  

    do {  
        // check if the first bit is 1 and calls XOR function  
        if (check_value[0] == '1')  
            XOR(check_value);  

        // Move the bits by 1 position for the next computation  
        for (j = 0; j < N - 1; j++)  
            check_value[j] = check_value[j + 1];  

        // appending a bit from data  
        check_value[j] = data[i++];  
    } while (i <= data_length + N - 1);  

    check_value[N - 1] = '\0'; // Asegurar terminador nulo al final
}  

void charToBinary(char c, char *binary) {
    for (int i = 0; i < 8; ++i) {
        binary[7 - i] = ((c >> i) & 1) + '0'; // Convertir bit a '0' o '1'
    }
    binary[8] = '\0'; // Añadir el terminador nulo
}

int main() {  
    char data[8] = "ho";  
    int len = strlen(data);  
    char binaryStr[8 * len + 10]; // Espacio para los bits y el terminador nulo

    binaryStr[0] = '\0'; // Iniciar la cadena vacía
    char check_value[64];

    for (int i = 0; i < len; ++i) {
        char binary[9]; // Para almacenar la representación binaria de un carácter
        charToBinary(data[i], binary);
        strcat(binaryStr, binary); // Concatenar la representación binaria
    }

    printf("La cadena en binario es:\n%s\n", binaryStr);

    data_length = strlen(binaryStr);  
    // appending n-1 zeros to the data  
    for (i = data_length; i < data_length + N - 1; i++)  
        binaryStr[i] = '0';  
    binaryStr[i] = '\0'; // Asegurar terminador nulo al final

    printf("\n----------------------------------------");  
    // print the data with padded zeros  
    printf("\n Data padded with n-1 zeros : %s", binaryStr);  
    printf("\n----------------------------------------");  

    // Cyclic Redundancy Check  
    crc(binaryStr,check_value);  

    // print the computed check value  
    printf("\nCRC or Check value is : %s", check_value);  

    // Append data with check_value(CRC)    
    for (i = data_length; i < data_length + N - 1; i++)  
        binaryStr[i] = check_value[i - data_length];  
    binaryStr[i] = '\0'; // Asegurar terminador nulo al final

    printf("\n----------------------------------------");  
    // printing the final data to be sent  
    printf("\n Final data to be sent : %s", binaryStr);  
    printf("\n----------------------------------------\n");  

    // Calling the receiver function to check errors

    receiver(binaryStr,check_value);  

    return 0;  
}
