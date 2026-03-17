#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h> // For isdigit() function

typedef struct {
    int base;
    int *digits;
    int size;
    bool sign;
} BigInt;

void initializeBigIntFromString(BigInt *bigInt, const char *str, int base) {
    if (bigInt == NULL || str == NULL) return;

    int length = strlen(str);
    int startIndex = 0;

    bigInt->base = base; // Set the base
    bigInt->sign = false; // Default to positive

    // Check if the number is negative
    if (str[0] == '-') {
        bigInt->sign = true;
        startIndex = 1; // Start reading digits after the sign
    }

    // Allocate memory for digits (number of digits = string length - startIndex)
    bigInt->size = length - startIndex;
    bigInt->digits = malloc(bigInt->size * sizeof(int));
    if (bigInt->digits == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    // Convert each character to a digit and store in `digits`
    for (int i = 0; i < bigInt->size; i++) {
        char c = str[startIndex + i];
        if (isdigit(c)) {
            bigInt->digits[i] = c - '0'; // Convert character to integer digit
        } else {
            fprintf(stderr, "Invalid character '%c' in string.\n", c);
            free(bigInt->digits);
            bigInt->digits = NULL;
            bigInt->size = 0;
            return;
        }
    }
}

void printBigInt(const BigInt *bigInt) {
    if (bigInt == NULL || bigInt->digits == NULL) {
        printf("BigInt is invalid (NULL).\n");
        return;
    }

    if (bigInt->sign) {
        printf("-");
    }

    for (int i = 0; i < bigInt->size; i++) {
        printf("%d", bigInt->digits[i]);
    }
    printf("\n");
}

// Add two BigInts
void addBigInts(const BigInt *a, const BigInt *b, BigInt *result) {
    int max_size = (a->size > b->size) ? a->size : b->size;  // Find the bigger size for allocation
    result->digits = malloc((max_size + 1) * sizeof(int));
    result->base = a->base;
    result->sign = false;  // Assume positive result
    result->size = max_size;
    int carry = 0;
    // Addition logic
    for (int i = 0; i < max_size; i++) {
        int digitA = (i < a->size) ? a->digits[a->size - 1 - i] : 0;
        int digitB = (i < b->size) ? b->digits[b->size - 1 - i] : 0;

        int sum = digitA + digitB + carry;
        carry = sum / a->base;
        result->digits[max_size - 1 - i] = sum % a->base;
    }
    // Handling the final carry
    if (carry > 0) {
        result->digits = realloc(result->digits, (max_size + 1) * sizeof(int));
        memmove(result->digits + 1, result->digits, max_size * sizeof(int));
        result->digits[0] = carry;
        result->size++;
    }
}
// Subtract one BigInt from another
void subtractBigInts(const BigInt *a, const BigInt *b, BigInt *result) {
    int max_size = (a->size > b->size) ? a->size : b->size;
    result->digits = malloc(max_size * sizeof(int));
    result->base = a->base;
    result->sign = false; // Assume result is positive for simplicity
    result->size = max_size;
    int borrow = 0;
    for (int i = 0; i < max_size; i++) {
        int digitA = (i < a->size) ? a->digits[a->size - 1 - i] : 0;
        int digitB = (i < b->size) ? b->digits[b->size - 1 - i] : 0;
        int diff = digitA - digitB - borrow;
        if (diff < 0) {
            diff += a->base;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->digits[max_size - 1 - i] = diff;
    }
    // Remove leading zeros from the result
    int first_non_zero = 0;
    while (first_non_zero < result->size - 1 && result->digits[first_non_zero] == 0) {
        first_non_zero++;
    }
    if (first_non_zero > 0) {
        int new_size = result->size - first_non_zero;
        memmove(result->digits, result->digits + first_non_zero, new_size * sizeof(int));
        result->digits = realloc(result->digits, new_size * sizeof(int));
        result->size = new_size;
    }
}
// Check if two BigInts are equal
bool equal(const BigInt *a, const BigInt *b) {
    if (a->size != b->size || a->sign != b->sign) {
        return false;
    }
    for (int i = 0; i < a->size; i++) {
        if (a->digits[i] != b->digits[i]) {
            return false;
        }
    }
    return true;
}

// Check if one BigInt is less than another
int lessthan(const BigInt *a, const BigInt *b) {
    if (a->sign != b->sign) {
        return a->sign ? 1 : 0; // If signs differ, the negative is lesser
    }
    // Same sign comparison
    int max_size = (a->size > b->size) ? a->size : b->size;
    if (a->sign) {
        // Negative comparison
        if (a->size < b->size) return 0;
        if (a->size > b->size) return 1;
    } else {
        // Positive comparison
        if (a->size < b->size) return 1;
        if (a->size > b->size) return 0;
    }
    // Same number of digits
    for (int i = 0; i < max_size; i++) {
        int digitA = (i < a->size) ? a->digits[i] : 0;
        int digitB = (i < b->size) ? b->digits[i] : 0;
        if (digitA < digitB) return a->sign ? 0 : 1;
        if (digitA > digitB) return a->sign ? 1 : 0;
    }
    return 3; // Numbers are equal
}
// Multiply two BigInts naively
void naive_mul(const BigInt *a, const BigInt *b, BigInt *result) {
    int max_size = a->size + b->size;
    result->digits = calloc(max_size, sizeof(int));
    result->base = a->base;
    result->sign = a->sign != b->sign; // Determine sign based on input signs
    result->size = max_size;
    // Basic multiplication logic
    for (int i = 0; i < a->size; i++) {
        for (int j = 0; j < b->size; j++) {
            int index = max_size - 1 - i - j;
            result->digits[index] += a->digits[a->size - 1 - i] * b->digits[b->size - 1 - j];
            if (result->digits[index] >= a->base) {
                result->digits[index - 1] += result->digits[index] / a->base;
                result->digits[index] %= a->base;
            }
        }
    }
    // Remove any leading zeros
    while (result->size > 1 && result->digits[0] == 0) {
        memmove(result->digits, result->digits + 1, (result->size - 1) * sizeof(int));
        result->size--;
    }
    // Reallocate to the correct size
    int *temp = realloc(result->digits, result->size * sizeof(int));
    if (temp != NULL) {
        result->digits = temp;
    }
}

// Modulo operation for BigInts
void modBigInts(const BigInt *a, const BigInt *b, BigInt *result) {
    BigInt tempA;
    tempA.base = a->base;
    tempA.size = a->size;
    tempA.sign = a->sign;
    tempA.digits = malloc(a->size * sizeof(int));
    memcpy(tempA.digits, a->digits, a->size * sizeof(int));
    BigInt tempB;
    tempB.base = b->base;
    tempB.size = b->size;
    tempB.sign = b->sign;
    tempB.digits = malloc(b->size * sizeof(int));
    memcpy(tempB.digits, b->digits, b->size * sizeof(int));
    BigInt tempResult;
    tempResult.base = a->base;
    tempResult.sign = false;
    // Continue subtraction until a < b
    while (lessthan(&tempA, &tempB) == 0 || lessthan(&tempA, &tempB) == 3) {
        subtractBigInts(&tempA, &tempB, &tempResult);
        free(tempA.digits);
        tempA.size = tempResult.size;
        tempA.digits = malloc(tempResult.size * sizeof(int));
        if (tempA.digits == NULL) {
            free(tempB.digits);
            return;
        }
        memcpy(tempA.digits, tempResult.digits, tempResult.size * sizeof(int));
        free(tempResult.digits);
    }
    // Set result to the remainder
    result->base = tempA.base;
    result->sign = tempA.sign;
    result->size = tempA.size;
    result->digits = malloc(tempA.size * sizeof(int));
    if (result->digits == NULL) {
        free(tempA.digits);
        free(tempB.digits);
        return;
    }
    memcpy(result->digits, tempA.digits, tempA.size * sizeof(int));

    free(tempA.digits);
    free(tempB.digits);
}
// Exponentiation by squaring for BigInts with modulus
void mod_exp(BigInt *A, unsigned int e, BigInt *N, BigInt *result) {
    BigInt base, temp_result;   
    // Initialize base and result
    base.base = A->base;
    base.sign = A->sign;
    base.size = A->size;
    base.digits = malloc(base.size * sizeof(int));
    memcpy(base.digits, A->digits, base.size * sizeof(int));
    temp_result.base = A->base;
    temp_result.sign = false;
    temp_result.size = 1;
    temp_result.digits = malloc(sizeof(int));
    temp_result.digits[0] = 1;
    BigInt mod_result, temp;
    mod_result.base = A->base;
    mod_result.sign = false;
    temp.base = A->base;
    temp.sign = false;
    // Modulo exponentiation
    while (e > 0) {
        if (e % 2 == 1) {
            naive_mul(&temp_result, &base, &temp);
            modBigInts(&temp, N, &mod_result);
            free(temp_result.digits);
            temp_result.size = mod_result.size;
            temp_result.digits = malloc(mod_result.size * sizeof(int));
            memcpy(temp_result.digits, mod_result.digits, mod_result.size * sizeof(int));
            free(mod_result.digits);
        }
        e >>= 1;
        naive_mul(&base, &base, &temp);
        modBigInts(&temp, N, &mod_result);
        free(base.digits);
        base.size = mod_result.size;
        base.digits = malloc(mod_result.size * sizeof(int));
        memcpy(base.digits, mod_result.digits, mod_result.size * sizeof(int));
        free(mod_result.digits);
        free(temp.digits);
    }
    // Set final result
    result->base = temp_result.base;
    result->sign = temp_result.sign;
    result->size = temp_result.size;
    result->digits = malloc(temp_result.size * sizeof(int));
    memcpy(result->digits, temp_result.digits, temp_result.size * sizeof(int));

    free(temp_result.digits);
    free(base.digits);
}


int main() {
    char number1[1024];
    char number2[1024];
    char number3[1024];
    unsigned int number4;

    // For addition
    printf("A=");
    scanf("%s", number1);
    printf("B=");
    scanf("%s", number2);
    BigInt myNumber1, myNumber2, result, diff, naive_m, myNumber3, result2, naive_m2;
    initializeBigIntFromString(&myNumber1, number1, 10);
    initializeBigIntFromString(&myNumber2, number2, 10);
    addBigInts(&myNumber1, &myNumber2, &result); // Perform addition
    printf("ADDITION=");
    printBigInt(&result);
    printf("\n");
    // For subtraction
    printf("A=");
    scanf("%s", number1);
    printf("B=");
    scanf("%s", number2);
    initializeBigIntFromString(&myNumber1, number1, 10);
    initializeBigIntFromString(&myNumber2, number2, 10);
    subtractBigInts(&myNumber1, &myNumber2, &diff); // Perform subtraction
    printf("SUBTRACTION=");
    printBigInt(&diff);
    printf("\n");
    // For equality
    printf("A=");
    scanf("%s", number1);
    printf("B=");
    scanf("%s", number2);
    initializeBigIntFromString(&myNumber1, number1, 10);
    initializeBigIntFromString(&myNumber2, number2, 10);
    if (equal(&myNumber1, &myNumber2)) {
        printf("EQUAL:TRUE\n");
    } else {
        printf("EQUAL:FALSE\n");
    }
    // For less than
    if (lessthan(&myNumber1, &myNumber2) == 1) {
        printf("LESSTHAN:TRUE\n");
    } else {
        printf("LESSTHAN:FALSE\n");
    }
    // For multiplication
    printf("\nA=");
    scanf("%s", number1);
    printf("B=");
    scanf("%s", number2);
    initializeBigIntFromString(&myNumber1, number1, 10);
    initializeBigIntFromString(&myNumber2, number2, 10);
    naive_mul(&myNumber1, &myNumber2, &naive_m);
    printf("MULTIPLICATIONNAIVE=");
    printBigInt(&naive_m);
    printf("\nA=");
    scanf("%s", number1);
    initializeBigIntFromString(&myNumber1, number1, 10);
    naive_mul(&myNumber1, &myNumber1, &naive_m2);
    printf("MULTIPLICATIONNAIVE=");
    printBigInt(&naive_m2);
    //for modular exponentitation
    printf("\nA=");
    scanf("%s", number1);
    initializeBigIntFromString(&myNumber1, number1, 10);
    printf("N=");
    scanf("%s", number3);
    printf("E=");
    scanf("%u", &number4);
    initializeBigIntFromString(&myNumber1, number1, 10);
    initializeBigIntFromString(&myNumber3, number3, 10);
    //modBigInts(&myNumber1, &myNumber3, &result2);  //to try the modulu function
    mod_exp(&myNumber1, number4, &myNumber3, &result2);
    printf("A^E MODULO N=");
    printBigInt(&result2);
    printf("\n");
    // Free allocated memory
    free(diff.digits);
    free(myNumber1.digits);
    free(myNumber2.digits);
    free(result.digits);
    free(naive_m.digits);
    free(result2.digits);
    return EXIT_SUCCESS;
}
