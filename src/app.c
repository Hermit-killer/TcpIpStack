#include <stdio.h>
#include "tnet_core.h"

int main(){
    tnet_init();
    printf("tnet is running!");
    while(1){
        tnet_poll();
    }
    return 0;
}