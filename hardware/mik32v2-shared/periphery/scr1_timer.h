#ifndef SCR1_TIMER_H_INCLUDED
#define SCR1_TIMER_H_INCLUDED


#define SCR1_TIMER_CTRL_ENABLE_S             0
#define SCR1_TIMER_CTRL_ENABLE_M             (1 << SCR1_TIMER_CTRL_ENABLE_S)

#define SCR1_TIMER_CTRL_CLKSRC_S             1
#define SCR1_TIMER_CTRL_CLKSRC_M             (1 << SCR1_TIMER_CTRL_CLKSRC_S)
#define SCR1_TIMER_CTRL_CLKSRC_INTERNAL_M    (0 << SCR1_TIMER_CTRL_CLKSRC_S)
#define SCR1_TIMER_CTRL_CLKSRC_RTC_M         (1 << SCR1_TIMER_CTRL_CLKSRC_S)


#ifndef __ASSEMBLER__
    #include <inttypes.h>

    typedef struct
    {
        volatile uint32_t TIMER_CTRL;
        volatile uint32_t TIMER_DIV;
        union 
        {
            struct 
            {
                volatile uint32_t MTIME;
                volatile uint32_t MTIMEH;        
            };
            uint64_t TIME;
        };
        
        union
        {
            struct
            {
                volatile uint32_t MTIMECMP;
                volatile uint32_t MTIMECMPH;        
            };
            uint64_t TIMECMP;
        };
        
    } SCR1_TIMER_TypeDef;
#endif

#endif // SCR1_TIMER_H_INCLUDED
