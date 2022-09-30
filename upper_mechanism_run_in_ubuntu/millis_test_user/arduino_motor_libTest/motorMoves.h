#ifndef motorMoves_h
#define motorMoves_h

#include "Arduino.h"

class motorMoves
{
    public:
        void StepperGo();
        void topStepper_forward();
        void topStepper_backward();
        void topStepper_task();
        void downDC_forward();
        void downDC_backward();
};

#endif
