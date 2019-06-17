#ifdef UNIT_TEST
#include <string.h>
#include <stdio.h>
/***********************************************/
void UTBeforeStart()
{

}

void test_low_power();
void test_battery();
void test_timer();

void UTRunSuite()
{
    //test_low_power();
    //test_battery();
    //test_timer();
    test_pulse();
}

void UTStart(void)
{
    int ch;
    while(1)
    {
        printf("unit test start\n");

        UTBeforeStart();
        UTRunSuite();

        printf("unit test end\n");

        printf("press \'a\' to test again or stop\n");
        ch = getchar();
        if (ch != 'a')
        {
            printf("---test end!---\n");
            break;
        }
    }
}

/***********************************************/
#endif
