#include "xparameters.h"
#include "xuartps.h"

int main() {
    XUartPs Uart1;
    int status;
    
    // Directly initialize UART1
    status = XUartPs_CfgInitialize(&Uart1, NULL, XPAR_XUARTPS_1_BASEADDR);
    
    if (status != XST_SUCCESS) {
        // If this fails, UART1 hardware has issues
        return -1;
    }
    
    XUartPs_SetBaudRate(&Uart1, 115200);
    
    // Send direct message to UART1
    char message[] = "DIRECT UART1 TEST - PC SHOULD SEE THIS\r\n";
    XUartPs_Send(&Uart1, (u8*)message, sizeof(message)-1);
    
    while(1) {
        char hello[] = "Hello from UART1!\r\n";
        XUartPs_Send(&Uart1, (u8*)hello, sizeof(hello)-1);
        for (volatile int i = 0; i < 10000000; i++);
    }
    
    return 0;
}