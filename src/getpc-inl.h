// -*- eval: (read-only-mode) -*-
// WARNING: this file is autogenerated.
// Change and run gen_getpc.rb >getpc-inl.h if you want to
// update. (And submit both files)

// What this file does? We have several possible ways of fetching PC
// (program counter) of signal's ucontext. We explicitly choose to
// avoid ifdef-ing specific OSes (or even specific versions), to
// increase our chances that stuff simply works. Comments below refer
// to OS/architecture combos for documentation purposes, but what
// works is what is used.

// How it does it? It uses lightweight C++ template magic where
// "wrong" ucontext_t{nullptr}-><field access> combos are
// automagically filtered out (via SFINAE).

// Each known case is represented as a template class. For SFINAE
// reasons we masquerade ucontext_t type behind U template
// parameter. And we also parameterize by parent class. This allows us
// to arrange all template instantiations in a single ordered chain of
// inheritance. See RawUCToPC below.

// Note, we do anticipate that most times exactly one of those access
// methods works. But we're prepared there could be several. In
// particular, according to previous comments Solaris/x86 also has
// REG_RIP defined, but it is somehow wrong. So we're careful about
// preserving specific order. We couldn't handle this "multiplicity"
// aspect in pure C++, so we use code generation.

namespace internal {

struct Empty {
#ifdef DEFINE_TRIVIAL_GET
#define HAVE_TRIVIAL_GET
  // special thing for stacktrace_generic_fp-inl which wants no-op case
  static void* Get(...) {
    return nullptr;
  }
#endif
};

// NetBSD has really nice portable macros
template <class U, class P, class = void>
struct get_c47a30af : public P {
};
#ifdef _UC_MACHINE_PC
template <class U, class P>
struct get_c47a30af<U, P, void_t<decltype(_UC_MACHINE_PC(((U*){})))>> : public P {
  static void* Get(const U* uc) {
    // NetBSD has really nice portable macros
    return (void*)(_UC_MACHINE_PC(uc));
  }
};
#endif  // _UC_MACHINE_PC

// Solaris/x86
template <class U, class P, class = void>
struct get_c4719e8d : public P {
};
#ifdef REG_PC
template <class U, class P>
struct get_c4719e8d<U, P, void_t<decltype(((U*){})->uc_mcontext.gregs[REG_PC])>> : public P {
  static void* Get(const U* uc) {
    // Solaris/x86
    return (void*)(uc->uc_mcontext.gregs[REG_PC]);
  }
};
#endif  // REG_PC

// Linux/i386
template <class U, class P, class = void>
struct get_278cba85 : public P {
};
#ifdef REG_EIP
template <class U, class P>
struct get_278cba85<U, P, void_t<decltype(((U*){})->uc_mcontext.gregs[REG_EIP])>> : public P {
  static void* Get(const U* uc) {
    // Linux/i386
    return (void*)(uc->uc_mcontext.gregs[REG_EIP]);
  }
};
#endif  // REG_EIP

// Linux/amd64
template <class U, class P, class = void>
struct get_b49f2593 : public P {
};
#ifdef REG_RIP
template <class U, class P>
struct get_b49f2593<U, P, void_t<decltype(((U*){})->uc_mcontext.gregs[REG_RIP])>> : public P {
  static void* Get(const U* uc) {
    // Linux/amd64
    return (void*)(uc->uc_mcontext.gregs[REG_RIP]);
  }
};
#endif  // REG_RIP

// Linux/ia64
template <class U, class P, class = void>
struct get_8fda99d3 : public P {
};
template <class U, class P>
struct get_8fda99d3<U, P, void_t<decltype(((U*){})->uc_mcontext.sc_ip)>> : public P {
  static void* Get(const U* uc) {
    // Linux/ia64
    return (void*)(uc->uc_mcontext.sc_ip);
  }
};

// Linux/loongarch64
template <class U, class P, class = void>
struct get_4e9b682d : public P {
};
template <class U, class P>
struct get_4e9b682d<U, P, void_t<decltype(((U*){})->uc_mcontext.__pc)>> : public P {
  static void* Get(const U* uc) {
    // Linux/loongarch64
    return (void*)(uc->uc_mcontext.__pc);
  }
};

// Linux/{mips,aarch64}
template <class U, class P, class = void>
struct get_b94b7246 : public P {
};
template <class U, class P>
struct get_b94b7246<U, P, void_t<decltype(((U*){})->uc_mcontext.pc)>> : public P {
  static void* Get(const U* uc) {
    // Linux/{mips,aarch64}
    return (void*)(uc->uc_mcontext.pc);
  }
};

// Linux/ppc
template <class U, class P, class = void>
struct get_d0eeceae : public P {
};
#ifdef PT_NIP
template <class U, class P>
struct get_d0eeceae<U, P, void_t<decltype(((U*){})->uc_mcontext.uc_regs->gregs[PT_NIP])>> : public P {
  static void* Get(const U* uc) {
    // Linux/ppc
    return (void*)(uc->uc_mcontext.uc_regs->gregs[PT_NIP]);
  }
};
#endif  // PT_NIP

// Linux/ppc
template <class U, class P, class = void>
struct get_a81f6801 : public P {
};
#ifdef PT_NIP
template <class U, class P>
struct get_a81f6801<U, P, void_t<decltype(((U*){})->uc_mcontext.gp_regs[PT_NIP])>> : public P {
  static void* Get(const U* uc) {
    // Linux/ppc
    return (void*)(uc->uc_mcontext.gp_regs[PT_NIP]);
  }
};
#endif  // PT_NIP

// Linux/riscv
template <class U, class P, class = void>
struct get_24e794ef : public P {
};
#ifdef REG_PC
template <class U, class P>
struct get_24e794ef<U, P, void_t<decltype(((U*){})->uc_mcontext.__gregs[REG_PC])>> : public P {
  static void* Get(const U* uc) {
    // Linux/riscv
    return (void*)(uc->uc_mcontext.__gregs[REG_PC]);
  }
};
#endif  // REG_PC

// Linux/s390
template <class U, class P, class = void>
struct get_d9a75ed3 : public P {
};
template <class U, class P>
struct get_d9a75ed3<U, P, void_t<decltype(((U*){})->uc_mcontext.psw.addr)>> : public P {
  static void* Get(const U* uc) {
    // Linux/s390
    return (void*)(uc->uc_mcontext.psw.addr);
  }
};

// Linux/arm (32-bit; legacy)
template <class U, class P, class = void>
struct get_07114491 : public P {
};
template <class U, class P>
struct get_07114491<U, P, void_t<decltype(((U*){})->uc_mcontext.arm_pc)>> : public P {
  static void* Get(const U* uc) {
    // Linux/arm (32-bit; legacy)
    return (void*)(uc->uc_mcontext.arm_pc);
  }
};

// FreeBSD/i386
template <class U, class P, class = void>
struct get_9be162e6 : public P {
};
template <class U, class P>
struct get_9be162e6<U, P, void_t<decltype(((U*){})->uc_mcontext.mc_eip)>> : public P {
  static void* Get(const U* uc) {
    // FreeBSD/i386
    return (void*)(uc->uc_mcontext.mc_eip);
  }
};

// FreeBSD/ppc
template <class U, class P, class = void>
struct get_2812b129 : public P {
};
template <class U, class P>
struct get_2812b129<U, P, void_t<decltype(((U*){})->uc_mcontext.mc_srr0)>> : public P {
  static void* Get(const U* uc) {
    // FreeBSD/ppc
    return (void*)(uc->uc_mcontext.mc_srr0);
  }
};

// FreeBSD/x86_64
template <class U, class P, class = void>
struct get_5bb1da03 : public P {
};
template <class U, class P>
struct get_5bb1da03<U, P, void_t<decltype(((U*){})->uc_mcontext.mc_rip)>> : public P {
  static void* Get(const U* uc) {
    // FreeBSD/x86_64
    return (void*)(uc->uc_mcontext.mc_rip);
  }
};

// OS X (i386, <=10.4)
template <class U, class P, class = void>
struct get_880f83fe : public P {
};
template <class U, class P>
struct get_880f83fe<U, P, void_t<decltype(((U*){})->uc_mcontext->ss.eip)>> : public P {
  static void* Get(const U* uc) {
    // OS X (i386, <=10.4)
    return (void*)(uc->uc_mcontext->ss.eip);
  }
};

// OS X (i386, >=10.5)
template <class U, class P, class = void>
struct get_92fcd89a : public P {
};
template <class U, class P>
struct get_92fcd89a<U, P, void_t<decltype(((U*){})->uc_mcontext->__ss.__eip)>> : public P {
  static void* Get(const U* uc) {
    // OS X (i386, >=10.5)
    return (void*)(uc->uc_mcontext->__ss.__eip);
  }
};

// OS X (x86_64)
template <class U, class P, class = void>
struct get_773e27c8 : public P {
};
template <class U, class P>
struct get_773e27c8<U, P, void_t<decltype(((U*){})->uc_mcontext->ss.rip)>> : public P {
  static void* Get(const U* uc) {
    // OS X (x86_64)
    return (void*)(uc->uc_mcontext->ss.rip);
  }
};

// OS X (>=10.5 [untested])
template <class U, class P, class = void>
struct get_6627078a : public P {
};
template <class U, class P>
struct get_6627078a<U, P, void_t<decltype(((U*){})->uc_mcontext->__ss.__rip)>> : public P {
  static void* Get(const U* uc) {
    // OS X (>=10.5 [untested])
    return (void*)(uc->uc_mcontext->__ss.__rip);
  }
};

// OS X (ppc, ppc64 [untested])
template <class U, class P, class = void>
struct get_da992aca : public P {
};
template <class U, class P>
struct get_da992aca<U, P, void_t<decltype(((U*){})->uc_mcontext->ss.srr0)>> : public P {
  static void* Get(const U* uc) {
    // OS X (ppc, ppc64 [untested])
    return (void*)(uc->uc_mcontext->ss.srr0);
  }
};

// OS X (>=10.5 [untested])
template <class U, class P, class = void>
struct get_cce47a40 : public P {
};
template <class U, class P>
struct get_cce47a40<U, P, void_t<decltype(((U*){})->uc_mcontext->__ss.__srr0)>> : public P {
  static void* Get(const U* uc) {
    // OS X (>=10.5 [untested])
    return (void*)(uc->uc_mcontext->__ss.__srr0);
  }
};

// OS X (arm64)
template <class U, class P, class = void>
struct get_0a082e42 : public P {
};
template <class U, class P>
struct get_0a082e42<U, P, void_t<decltype(((U*){})->uc_mcontext->__ss.__pc)>> : public P {
  static void* Get(const U* uc) {
    // OS X (arm64)
    return (void*)(uc->uc_mcontext->__ss.__pc);
  }
};

// OpenBSD/i386
template <class U, class P, class = void>
struct get_3baa113a : public P {
};
template <class U, class P>
struct get_3baa113a<U, P, void_t<decltype(((U*){})->sc_eip)>> : public P {
  static void* Get(const U* uc) {
    // OpenBSD/i386
    return (void*)(uc->sc_eip);
  }
};

// OpenBSD/x86_64
template <class U, class P, class = void>
struct get_79f33851 : public P {
};
template <class U, class P>
struct get_79f33851<U, P, void_t<decltype(((U*){})->sc_rip)>> : public P {
  static void* Get(const U* uc) {
    // OpenBSD/x86_64
    return (void*)(uc->sc_rip);
  }
};

inline void* RawUCToPC(const ucontext_t* uc) {
  // OpenBSD/x86_64
  using g_79f33851 = get_79f33851<ucontext_t, Empty>;
  // OpenBSD/i386
  using g_3baa113a = get_3baa113a<ucontext_t, g_79f33851>;
  // OS X (arm64)
  using g_0a082e42 = get_0a082e42<ucontext_t, g_3baa113a>;
  // OS X (>=10.5 [untested])
  using g_cce47a40 = get_cce47a40<ucontext_t, g_0a082e42>;
  // OS X (ppc, ppc64 [untested])
  using g_da992aca = get_da992aca<ucontext_t, g_cce47a40>;
  // OS X (>=10.5 [untested])
  using g_6627078a = get_6627078a<ucontext_t, g_da992aca>;
  // OS X (x86_64)
  using g_773e27c8 = get_773e27c8<ucontext_t, g_6627078a>;
  // OS X (i386, >=10.5)
  using g_92fcd89a = get_92fcd89a<ucontext_t, g_773e27c8>;
  // OS X (i386, <=10.4)
  using g_880f83fe = get_880f83fe<ucontext_t, g_92fcd89a>;
  // FreeBSD/x86_64
  using g_5bb1da03 = get_5bb1da03<ucontext_t, g_880f83fe>;
  // FreeBSD/ppc
  using g_2812b129 = get_2812b129<ucontext_t, g_5bb1da03>;
  // FreeBSD/i386
  using g_9be162e6 = get_9be162e6<ucontext_t, g_2812b129>;
  // Linux/arm (32-bit; legacy)
  using g_07114491 = get_07114491<ucontext_t, g_9be162e6>;
  // Linux/s390
  using g_d9a75ed3 = get_d9a75ed3<ucontext_t, g_07114491>;
  // Linux/riscv (with #ifdef REG_PC)
  using g_24e794ef = get_24e794ef<ucontext_t, g_d9a75ed3>;
  // Linux/ppc (with #ifdef PT_NIP)
  using g_a81f6801 = get_a81f6801<ucontext_t, g_24e794ef>;
  // Linux/ppc (with #ifdef PT_NIP)
  using g_d0eeceae = get_d0eeceae<ucontext_t, g_a81f6801>;
  // Linux/{mips,aarch64}
  using g_b94b7246 = get_b94b7246<ucontext_t, g_d0eeceae>;
  // Linux/loongarch64
  using g_4e9b682d = get_4e9b682d<ucontext_t, g_b94b7246>;
  // Linux/ia64
  using g_8fda99d3 = get_8fda99d3<ucontext_t, g_4e9b682d>;
  // Linux/amd64 (with #ifdef REG_RIP)
  using g_b49f2593 = get_b49f2593<ucontext_t, g_8fda99d3>;
  // Linux/i386 (with #ifdef REG_EIP)
  using g_278cba85 = get_278cba85<ucontext_t, g_b49f2593>;
  // Solaris/x86 (with #ifdef REG_PC)
  using g_c4719e8d = get_c4719e8d<ucontext_t, g_278cba85>;
  // NetBSD has really nice portable macros (with #ifdef _UC_MACHINE_PC)
  using g_c47a30af = get_c47a30af<ucontext_t, g_c4719e8d>;
  return g_c47a30af::Get(uc);
}

}  // namespace internal