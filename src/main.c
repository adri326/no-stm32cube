#include <inttypes.h>

char *mystring = "1234569802";
int src[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
int dest[20];
volatile int dummy = 20;
volatile int gnr=5 ;

/**
  * @brief Copy nr 32bits data from one location to another
  * @param  source the source address of the data to copy
  * @param  destination the destination address of the data to copy
  * @param  nr number of 32bits data to copy
  * @retval number of data left to be copied
 */
int wordcopy(int* source, int* destination, int nr) {
    int tmp;
    /* global variable for debug */
    gnr = nr;
    while (nr > 0) {
        tmp = *source++;
        *destination++ = tmp;
        gnr = --nr;
    }
    return nr;
}

/**
  * @brief Convert an unsigned int in a non zero terminated string
  *        of decimal number (sprintf like). The function is recursive
  * @param  str string address to to receive the converted string
  * @param  a     uint32 to be converted
  * @retval return the first address after the last converted character address
 */
// This is awfully inefficient and horrible to debug
// char* uint2str2(char* str, uint32_t a) {
//     uint8_t r = a % 10;
//     a /= 10;

//     if (a > 0) {
//         str = uint2str2(str, a);
//     }

//     *str++ = r + '0'; // conversion en asccii du reste
//                       // et on incrémente le pointeur (postincrémentation)
//                       // afin qu'il pointe sur le caractère suivant
//     return (str);    // renvoie l'adresse du caractère suivant
// }

/**
  * @brief Convert an unsigned int in a zero terminated string of decimal number
  *        (sprintf like)
  * @param  str string address to to receive the converted string
  * @param  a     uint32 to be converted
  * @retval return the string address
 */
char* uint2str(char* str, uint32_t a) {
    char* end = str;
    while (a > 0) {
        *end++ = (a % 10) + '0';
        a /= 10;
    }
    *end = 0;

    // just call uint2char2
    // char* EndOfStr = uint2str2(str, a);
    // cand add the end of strig char
    // *EndOfStr = 0;
    // return string address
    return str;
}

/**
  * @brief Convert a string of number in an unsigned int. (strtoul like)
  *        the string must contain only decimal numbers in ascii
  * @param  str string address to be converted
  * @param  destination the destination address of the data to copy
  * @param  nr number of 32bits data to copy
  * @retval the uint32_t result
 */
uint32_t str2uint(char* str) {
    uint32_t i=0;
    uint32_t a=0;

    while (str[i] != '\0') {
        a = a*10 + str[i]-'0';
        i = i+1;
    }
    return a ;
}

/**
  * @brief main function
  * @param  none
  * @retval 0 if success
 */
int main(void) {
    unsigned int a,b;
    char localstring[20];
    char *ptr;

    a = 20;
    b = 2876523465;
    localstring[1] = 16;
    localstring[5] = 65;
    localstring[3] = 99;

    /* systeme 'bare metal', pas de systeme d'exploitation qui maintient une
     * boucle infinie lorsqu'aucune application ne tourne, l'application doit
     * comprendre une boucle infinie
     */
    while (1) {
        // test wordcopy
        a = wordcopy(src, dest, a);

        // test uint2str
        ptr = uint2str(localstring, b);


        // test str2uint
        a = str2uint(mystring);
    }

    return 0;
}
