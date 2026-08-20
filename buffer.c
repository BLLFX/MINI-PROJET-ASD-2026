#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"

// Fonction auxiliaire pour vider le buffer stdin
void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
