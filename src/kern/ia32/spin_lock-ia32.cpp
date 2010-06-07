//---------------------------------------------------------------------------
INTERFACE [(ia32|ux|amd64) && mp]:

EXTENSION class Spin_lock
{
public:
  enum { Arch_lock = 2 };
};

//--------------------------------------------------------------------------
IMPLEMENTATION [(ia32|ux|amd64) && mp]:

PRIVATE inline
void
Spin_lock::lock_arch()
{
  unsigned long dummy, tmp;
  __asm__ __volatile__ (
      "1: mov %[lock], %[tmp]  \n"
      "   test $2, %[tmp]      \n" // Arch_lock == #2
      "   jz 2f                \n"
      "   pause                \n"
      "   jmp 1b               \n"
      "2: mov %[tmp], %[d]     \n"
      "   or $2, %[d]          \n"
      "   lock; cmpxchg %[d], %[lock]  \n"
      "   jnz 1b                \n"
      : [d] "=&r" (dummy), [tmp] "=&a" (tmp), [lock] "+m" (_lock)
      );
}

PRIVATE inline
void
Spin_lock::unlock_arch()
{
  _lock &= ~Arch_lock;
}
