/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #include "bsp_api.h"
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (18)
        #endif
        /* ISR prototypes */
        void sci_uart_eri_isr(void);
        void sci_uart_rxi_isr(void);
        void sci_uart_txi_isr(void);
        void sci_uart_tei_isr(void);
        void rtc_alarm_periodic_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_SCI0_ERI ((IRQn_Type) 288) /* SCI0_ERI (SCI0 Receive error) */
        #define VECTOR_NUMBER_SCI0_RXI ((IRQn_Type) 289) /* SCI0_RXI (SCI0 Receive data full) */
        #define VECTOR_NUMBER_SCI0_TXI ((IRQn_Type) 290) /* SCI0_TXI (SCI0 Transmit data empty) */
        #define VECTOR_NUMBER_SCI0_TEI ((IRQn_Type) 291) /* SCI0_TEI (SCI0 Transmit end) */
        #define VECTOR_NUMBER_SCI3_ERI ((IRQn_Type) 300) /* SCI3_ERI (SCI3 Receive error) */
        #define VECTOR_NUMBER_SCI3_RXI ((IRQn_Type) 301) /* SCI3_RXI (SCI3 Receive data full) */
        #define VECTOR_NUMBER_SCI3_TXI ((IRQn_Type) 302) /* SCI3_TXI (SCI3 Transmit data empty) */
        #define VECTOR_NUMBER_SCI3_TEI ((IRQn_Type) 303) /* SCI3_TEI (SCI3 Transmit end) */
        #define VECTOR_NUMBER_SCI4_ERI ((IRQn_Type) 304) /* SCI4_ERI (SCI4 Receive error) */
        #define VECTOR_NUMBER_SCI4_RXI ((IRQn_Type) 305) /* SCI4_RXI (SCI4 Receive data full) */
        #define VECTOR_NUMBER_SCI4_TXI ((IRQn_Type) 306) /* SCI4_TXI (SCI4 Transmit data empty) */
        #define VECTOR_NUMBER_SCI4_TEI ((IRQn_Type) 307) /* SCI4_TEI (SCI4 Transmit end) */
        #define VECTOR_NUMBER_RTC_ALM ((IRQn_Type) 432) /* RTC_ALM (Alarm interrupt) */
        #define VECTOR_NUMBER_RTC_PRD ((IRQn_Type) 434) /* RTC_PRD (Fixed interval interrupt) */
        #define VECTOR_NUMBER_SCI5_ERI ((IRQn_Type) 435) /* SCI5_ERI (SCI5 Receive error) */
        #define VECTOR_NUMBER_SCI5_RXI ((IRQn_Type) 436) /* SCI5_RXI (SCI5 Receive data full) */
        #define VECTOR_NUMBER_SCI5_TXI ((IRQn_Type) 437) /* SCI5_TXI (SCI5 Transmit data empty) */
        #define VECTOR_NUMBER_SCI5_TEI ((IRQn_Type) 438) /* SCI5_TEI (SCI5 Transmit end) */
        typedef enum IRQn {
            SoftwareGeneratedInt0 = -32,
            SoftwareGeneratedInt1 = -31,
            SoftwareGeneratedInt2 = -30,
            SoftwareGeneratedInt3 = -29,
            SoftwareGeneratedInt4 = -28,
            SoftwareGeneratedInt5 = -27,
            SoftwareGeneratedInt6 = -26,
            SoftwareGeneratedInt7 = -25,
            SoftwareGeneratedInt8 = -24,
            SoftwareGeneratedInt9 = -23,
            SoftwareGeneratedInt10 = -22,
            SoftwareGeneratedInt11 = -21,
            SoftwareGeneratedInt12 = -20,
            SoftwareGeneratedInt13 = -19,
            SoftwareGeneratedInt14 = -18,
            SoftwareGeneratedInt15 = -17,
            DebugCommunicationsChannelInt = -10,
            PerformanceMonitorCounterOverflowInt = -9,
            CrossTriggerInterfaceInt = -8,
            VritualCPUInterfaceMaintenanceInt = -7,
            HypervisorTimerInt = -6,
            VirtualTimerInt = -5,
            NonSecurePhysicalTimerInt = -2,
            SCI0_ERI_IRQn = 288, /* SCI0_ERI (SCI0 Receive error) */
            SCI0_RXI_IRQn = 289, /* SCI0_RXI (SCI0 Receive data full) */
            SCI0_TXI_IRQn = 290, /* SCI0_TXI (SCI0 Transmit data empty) */
            SCI0_TEI_IRQn = 291, /* SCI0_TEI (SCI0 Transmit end) */
            SCI3_ERI_IRQn = 300, /* SCI3_ERI (SCI3 Receive error) */
            SCI3_RXI_IRQn = 301, /* SCI3_RXI (SCI3 Receive data full) */
            SCI3_TXI_IRQn = 302, /* SCI3_TXI (SCI3 Transmit data empty) */
            SCI3_TEI_IRQn = 303, /* SCI3_TEI (SCI3 Transmit end) */
            SCI4_ERI_IRQn = 304, /* SCI4_ERI (SCI4 Receive error) */
            SCI4_RXI_IRQn = 305, /* SCI4_RXI (SCI4 Receive data full) */
            SCI4_TXI_IRQn = 306, /* SCI4_TXI (SCI4 Transmit data empty) */
            SCI4_TEI_IRQn = 307, /* SCI4_TEI (SCI4 Transmit end) */
            RTC_ALM_IRQn = 432, /* RTC_ALM (Alarm interrupt) */
            RTC_PRD_IRQn = 434, /* RTC_PRD (Fixed interval interrupt) */
            SCI5_ERI_IRQn = 435, /* SCI5_ERI (SCI5 Receive error) */
            SCI5_RXI_IRQn = 436, /* SCI5_RXI (SCI5 Receive data full) */
            SCI5_TXI_IRQn = 437, /* SCI5_TXI (SCI5 Transmit data empty) */
            SCI5_TEI_IRQn = 438, /* SCI5_TEI (SCI5 Transmit end) */
            SHARED_PERIPHERAL_INTERRUPTS_MAX_ENTRIES = BSP_VECTOR_TABLE_MAX_ENTRIES
        } IRQn_Type;
        #endif /* VECTOR_DATA_H */