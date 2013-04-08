INTERFACE:

#include "types.h"

EXTENSION class Timer_tick
{
public:
  static Static_object<Timer_tick> _glbl_timer;
};

IMPLEMENTATION:

#include "timer.h"

Static_object<Timer_tick> Timer_tick::_glbl_timer;

IMPLEMENT void
Timer_tick::setup(Cpu_number cpu)
{
  // all CPUs use the same timer IRQ, so initialize just on CPU 0
  if (cpu != Cpu_number::boot_cpu())
    return;

  _glbl_timer.construct(Any_cpu);
  if (!allocate_irq(_glbl_timer, Timer::irq()))
    panic("Could not allocate scheduling IRQ %d\n", Timer::irq());

  _glbl_timer->set_mode(Timer::irq_mode());
}

IMPLEMENT
void
Timer_tick::enable(Cpu_number)
{
  _glbl_timer->chip()->unmask(_glbl_timer->pin());
}

IMPLEMENT
void
Timer_tick::disable(Cpu_number)
{
  _glbl_timer->chip()->mask(_glbl_timer->pin());
}

PUBLIC inline NEEDS["timer.h"]
void
Timer_tick::ack()
{
  Timer::acknowledge();
  Irq_base::ack();
}
