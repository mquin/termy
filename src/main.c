
#pragma output noprotectmsdos
#pragma output nofileio

#include "wifi.h"
#include "next.h"
#include <stdlib.h>
#include <stdio.h>

void main(int argc, char *argv[])
{
    unsigned char buffer[2048];
    unsigned int bpos=0;

    if (argc < 3) {
        printf("Usage:\n%s host port\n", argv[0]);
        
        return;
    }

    unsigned char c, k, f, q, t, status;
    unsigned int i;
    q=0;

    uart_init();
    printf("Uart inited\r\n");
    printf("CTRL-Q for exit\r\n");

    if (wifi_connect(argv[1], argv[2]))
        for (;k!=17;) {
       status = inp(UART_GetStatus);
        while (status & UART_DATA_READY != 0) {
            buffer[bpos]=inp(UART_RxD);
            buffer[bpos+1]=0;
            bpos++;
            status = inp(UART_GetStatus);
        }

        if (bpos !=0) {
            for (i=0 ; i < bpos ; i++) {
                c=buffer[i];

                if (c == 255 && q==0) {
                        q=1;
                        continue;
                }
                else if (c > 250 && q==1) {
                    t=c;
                    q=2;
                    continue;
                }
                else if (q==2 && ( t==251 || t==252 )) {
                    if ( c== 1 ) {
                        uart_putc(255);
                        uart_putc(253);
                        uart_putc(c);
                    } else {
                        uart_putc(255);
                        uart_putc(254);
                        uart_putc(c);
                    }
                    q=0;
                    continue;
                }
                else if (q==2 && ( t==253 || t==254 )) {
                        uart_putc(255);
                        uart_putc(252);
                        uart_putc(c);
                        q=0;
                        continue;
                } else {
                   printf("%c",c);
                }
            }
        }
        buffer[0]=0;
        bpos=0;

        k = getk();
        if (k) {
            uart_putc(k);
//                if (k==13)g
//                    uart_putc(10);
            while(getk() == k);
        }
    }
    reset_esp();
    
    return ;
}

