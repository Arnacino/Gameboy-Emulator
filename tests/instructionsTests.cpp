#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "../src/Instructions.h"
#include "../src/Memory.h"
#include "../src/Registers.h"

class TestFixture {
protected:
    Memory memory;
    Registers registers;
    Instructions instructions{&registers, &memory};
};

//OK
TEST_CASE_METHOD(TestFixture, "Add Instructions", "[instructions]") {
    SECTION("ADD A,B") {
        registers.a = 0x3A;
        registers.b = 0xC6;
        instructions.addRA(&registers.b);
        REQUIRE(registers.a == 0x00);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == true);
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == true);
    }

    SECTION("ADD A,B with half carry") {
        registers.a = 0x0F;
        registers.b = 0x01;
        instructions.addRA(&registers.b);
        REQUIRE(registers.a == 0x10);
        REQUIRE(registers.isFlagSet(RegistersFlags::HALF_CARRY_FLAG) == true);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Arithmetic Instructions", "[instructions]") {
    SECTION("ADC A,B without carry") {
        registers.a = 0x3A;
        registers.b = 0x42;
        registers.setFlag(RegistersFlags::CARRY_FLAG, false);
        instructions.adcRA(&registers.b);
        REQUIRE(registers.a == 0x7C);
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == false);
    }

    SECTION("ADC A,B with carry") {
        registers.a = 0xFF;
        registers.b = 0x01;
        registers.setFlag(RegistersFlags::CARRY_FLAG, true);
        instructions.adcRA(&registers.b);
        REQUIRE(registers.a == 0x01);
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == true);
    }
    
    SECTION("SUB A,B") {
        registers.a = 0x42;
        registers.b = 0x3A;
        instructions.subRA(&registers.b);
        REQUIRE(registers.a == 0x08);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == false);
        REQUIRE(registers.isFlagSet(RegistersFlags::SUBTRACTION_FLAG) == true);
    }
}

//MANCANO IMPLEMENTAZIONI
TEST_CASE_METHOD(TestFixture, "Basic Instructions", "[instructions]") {
    SECTION("DI and EI") {
        // Poiché queste funzioni sono vuote, verifichiamo solo che non causino errori
        REQUIRE_NOTHROW(instructions.di());
        REQUIRE_NOTHROW(instructions.ei());
    }
    
    SECTION("HALT and STOP") {
        // Anche queste funzioni sono essenzialmente vuote, verifichiamo solo che non causino errori
        REQUIRE_NOTHROW(instructions.halt());
        REQUIRE_NOTHROW(instructions.stop());
    }
}

//NON ANCORA IMPLEMENTATE
TEST_CASE_METHOD(TestFixture, "Bit Operations", "[instructions]") {
    SECTION("BIT b,r") {
        registers.b = 0b10000000;
        instructions.bitBR(&registers.b);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == false);
        
        registers.b = 0b01111111;
        instructions.bitBR(&registers.b);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == true);
    }

    SECTION("SET b,r") {
        registers.b = 0b00000000;
        instructions.setBR(&registers.b);
        REQUIRE(registers.b == 0b10000000);
    }

    SECTION("RES b,r") {
        registers.b = 0b11111111;
        instructions.resBR(&registers.b);
        REQUIRE(registers.b == 0b01111111);
    }
}

//NON ANCORA IMPLEMENTATE
TEST_CASE_METHOD(TestFixture, "Control Flow Instructions", "[instructions]") {
    SECTION("JP HL") {
        registers.hl = 0x1234;
        instructions.jpHl();
        REQUIRE(registers.pc == 0x1234);
    }
    
    SECTION("JP C,nn") {
        memory.write(registers.pc, 0x34);
        memory.write(registers.pc + 1, 0x12);
        
        // Test con C=1 (condition = true)
        registers.setFlag(RegistersFlags::CARRY_FLAG, true);
        uint16_t oldPC = registers.pc;
        instructions.jpCIm(true);
        REQUIRE(registers.pc == 0x1234);
        
        // Test con C=0 (condition = false)
        registers.pc = oldPC;  // Reset PC
        memory.write(registers.pc, 0x34);
        memory.write(registers.pc + 1, 0x12);
        registers.setFlag(RegistersFlags::CARRY_FLAG, false);
        instructions.jpCIm(false);
        REQUIRE(registers.pc == oldPC + 2);  // Solo l'incremento del PC, nessun salto
    }
    
    SECTION("CALL nn") {
        registers.pc = 0x0100;
        registers.sp = 0xFFFE;
        memory.write(registers.pc, 0x34);
        memory.write(registers.pc + 1, 0x12);
        
        instructions.callIm();
        
        // Verifica che l'indirizzo di ritorno sia stato inserito nello stack
        REQUIRE(memory.read(0xFFFD) == 0x01);
        REQUIRE(memory.read(0xFFFC) == 0x02);
        
        // Verifica che il PC sia stato impostato al nuovo indirizzo
        REQUIRE(registers.pc == 0x1234);
        
        // Verifica che lo SP sia stato decrementato
        REQUIRE(registers.sp == 0xFFFC);
    }
    
    SECTION("CALL C,nn") {
        registers.pc = 0x0100;
        registers.sp = 0xFFFE;
        memory.write(registers.pc, 0x34);
        memory.write(registers.pc + 1, 0x12);
        
        // Test con C=1 (condition = true)
        registers.setFlag(RegistersFlags::CARRY_FLAG, true);
        instructions.callCIm(true);
        
        // Verifica che l'indirizzo di ritorno sia stato inserito nello stack
        REQUIRE(memory.read(0xFFFD) == 0x01);
        REQUIRE(memory.read(0xFFFC) == 0x02);
        
        // Verifica che il PC sia stato impostato al nuovo indirizzo
        REQUIRE(registers.pc == 0x1234);
        
        // Test con C=0 (condition = false)
        registers.pc = 0x0100;
        registers.sp = 0xFFFE;
        memory.write(registers.pc, 0x34);
        memory.write(registers.pc + 1, 0x12);
        registers.setFlag(RegistersFlags::CARRY_FLAG, false);
        instructions.callCIm(false);
        
        // Verifica che il PC sia stato incrementato senza chiamare
        REQUIRE(registers.pc == 0x0102);
        REQUIRE(registers.sp == 0xFFFE);  // SP non dovrebbe cambiare
    }
    
    SECTION("RET") {
        registers.sp = 0xFFFC;
        memory.write(0xFFFC, 0x34);
        memory.write(0xFFFD, 0x12);
        
        instructions.ret();
        
        // Verifica che il PC sia stato impostato all'indirizzo dello stack
        REQUIRE(registers.pc == 0x1234);
        
        // Verifica che lo SP sia stato incrementato
        REQUIRE(registers.sp == 0xFFFE);
    }
    
    SECTION("RET C") {
        // Test con C=1 (condition = true)
        registers.sp = 0xFFFC;
        registers.pc = 0x0100;
        memory.write(0xFFFC, 0x34);  // LSB
        memory.write(0xFFFD, 0x12);  // MSB
        registers.setFlag(RegistersFlags::CARRY_FLAG, true);
        
        instructions.retC(true);  // Chiamata con condition = true
        
        // Verifica che il PC sia stato impostato all'indirizzo dello stack
        REQUIRE(registers.pc == 0x1234);
        // Verifica che lo SP sia stato incrementato
        REQUIRE(registers.sp == 0xFFFE);
        
        // Test con C=0 (condition = false)
        registers.sp = 0xFFFC;
        registers.pc = 0x0100;
        memory.write(0xFFFC, 0x34);
        memory.write(0xFFFD, 0x12);
        
        instructions.retC(false);  // Chiamata con condition = false
        
        // Verifica che il PC non sia cambiato
        REQUIRE(registers.pc == 0x0100);
        // Verifica che lo SP non sia cambiato
        REQUIRE(registers.sp == 0xFFFC);
    }
    
    SECTION("RETI") {
        registers.sp = 0xFFFC;
        memory.write(0xFFFC, 0x34);
        memory.write(0xFFFD, 0x12);
        
        instructions.retI();
        
        // Verifica che il PC sia stato impostato all'indirizzo dello stack
        REQUIRE(registers.pc == 0x1234);
        
        // Verifica che lo SP sia stato incrementato
        REQUIRE(registers.sp == 0xFFFE);
        
        // RETI dovrebbe anche abilitare gli interrupt, ma questa funzionalità non è implementata nel codice
    }
    
    SECTION("RST n") {
        registers.pc = 0x0100;
        registers.sp = 0xFFFE;
        uint8_t rstVec = 0x08;  // Vettore di reset (0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38)
        
        instructions.rstIm();
        
        // Verifica che l'indirizzo di ritorno sia stato inserito nello stack
        REQUIRE(memory.read(0xFFFD) == 0x01);
        REQUIRE(memory.read(0xFFFC) == 0x00);
        
        // Verifica che il PC sia stato impostato all'indirizzo RST
        REQUIRE(registers.pc == rstVec);
        
        // Verifica che lo SP sia stato decrementato
        REQUIRE(registers.sp == 0xFFFC);
    }
    
    SECTION("JR n") {
        registers.pc = 0x0100;
        memory.write(registers.pc, 0x05);  // Jump forward 5
        instructions.jrIm();
        REQUIRE(registers.pc == 0x0106);  // 0x0100 + 1 + 5
        
        registers.pc = 0x0100;
        memory.write(registers.pc, 0xFB);  // Jump back 5 (-5 in two's complement)
        instructions.jrIm();
        REQUIRE(registers.pc == 0x00FC);  // 0x0100 + 1 - 5
    }
    
    SECTION("JR C,n") {
        registers.pc = 0x0100;
        memory.write(registers.pc, 0x05);
        
        // Test con C=1 (condition = true)
        registers.setFlag(RegistersFlags::CARRY_FLAG, true);
        instructions.jrCIm(registers.isFlagSet(RegistersFlags::CARRY_FLAG));
        REQUIRE(registers.pc == 0x0106);  // 0x0100 + 1 + 5
        
        // Test con C=0 (condition = false)
        registers.pc = 0x0100;
        memory.write(registers.pc, 0x05);
        registers.setFlag(RegistersFlags::CARRY_FLAG, false);
        instructions.jrCIm(registers.isFlagSet(RegistersFlags::CARRY_FLAG));
        REQUIRE(registers.pc == 0x0101);  // Solo l'incremento del PC, nessun salto
    }
    
    SECTION("JR NZ,n") {
        registers.pc = 0x0100;
        memory.write(registers.pc, 0x05);
        
        // Test con Z=0 (NZ=1, condition = true)
        registers.setFlag(RegistersFlags::ZERO_FLAG, false);
        instructions.jrCIm(!registers.isFlagSet(RegistersFlags::ZERO_FLAG));
        REQUIRE(registers.pc == 0x0106);  // 0x0100 + 1 + 5
        
        // Test con Z=1 (NZ=0, condition = false)
        registers.pc = 0x0100;
        memory.write(registers.pc, 0x05);
        registers.setFlag(RegistersFlags::ZERO_FLAG, true);
        instructions.jrCIm(!registers.isFlagSet(RegistersFlags::ZERO_FLAG));
        REQUIRE(registers.pc == 0x0101);  // Solo l'incremento del PC, nessun salto
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "DAA Instruction", "[instructions]") {
    SECTION("DAA after addition") {
        registers.a = 0x45;
        registers.b = 0x38;
        instructions.addRA(&registers.b);
        instructions.daA();
        REQUIRE(registers.a == 0x83);  // 45 + 38 = 83 in BCD
    }

    SECTION("DAA after subtraction") {
        registers.a = 0x83;
        registers.b = 0x38;
        instructions.subRA(&registers.b);
        instructions.daA();
        REQUIRE(registers.a == 0x45);  // 83 - 38 = 45 in BCD
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Decrement Instructions", "[instructions]") {
    SECTION("DEC B") {
        registers.b = 0x43;
        instructions.decR8(&registers.b);
        REQUIRE(registers.b == 0x42);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == false);
        REQUIRE(registers.isFlagSet(RegistersFlags::SUBTRACTION_FLAG) == true);
    }

    SECTION("DEC B with zero result") {
        registers.b = 0x01;
        instructions.decR8(&registers.b);
        REQUIRE(registers.b == 0x00);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == true);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Flag Instructions", "[instructions]") {
    SECTION("SCF") {
        registers.setFlag(RegistersFlags::CARRY_FLAG, false);
        instructions.scf();
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == true);
    }
    
    SECTION("CCF") {
        registers.setFlag(RegistersFlags::CARRY_FLAG, true);
        instructions.ccf();
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == false);
        
        instructions.ccf();
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == true);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Increment Instructions", "[instructions]") {
    SECTION("INC B") {
        registers.b = 0x41;
        instructions.incR8(&registers.b);
        REQUIRE(registers.b == 0x42);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == false);
        REQUIRE(registers.isFlagSet(RegistersFlags::SUBTRACTION_FLAG) == false);
    }

    SECTION("INC B with zero result") {
        registers.b = 0xFF;
        instructions.incR8(&registers.b);
        REQUIRE(registers.b == 0x00);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == true);
    }

    SECTION("INC B with half carry") {
        registers.b = 0x0F;
        instructions.incR8(&registers.b);
        REQUIRE(registers.b == 0x10);
        REQUIRE(registers.isFlagSet(RegistersFlags::HALF_CARRY_FLAG) == true);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Jump Instructions", "[instructions]") {
    SECTION("JP nn") {
        memory.write(registers.pc, 0x34);
        memory.write(registers.pc + 1, 0x12);
        instructions.jpIm();
        REQUIRE(registers.pc == 0x1234);
    }
    
    SECTION("JR n") {
        registers.pc = 0x100;
        memory.write(registers.pc, 0x05);  // Jump forward 5
        instructions.jrIm();
        REQUIRE(registers.pc == 0x106);  // 0x100 + 1 + 5
        
        registers.pc = 0x100;
        memory.write(registers.pc, 0xFB);  // Jump back 5 (-5 in two's complement)
        instructions.jrIm();
        REQUIRE(registers.pc == 0xFC);  // 0x100 + 1 - 5
    }

    SECTION("JR NZ,n with Z=0") {
        registers.pc = 0x100;
        memory.write(registers.pc, 0x05);
        registers.setFlag(RegistersFlags::ZERO_FLAG, false);
        instructions.jrCIm(!registers.isFlagSet(RegistersFlags::ZERO_FLAG));
        REQUIRE(registers.pc == 0x106);
    }

    SECTION("JR NZ,n with Z=1") {
        registers.pc = 0x100;
        memory.write(registers.pc, 0x05);
        registers.setFlag(RegistersFlags::ZERO_FLAG, true);
        instructions.jrCIm(!registers.isFlagSet(RegistersFlags::ZERO_FLAG));
        REQUIRE(registers.pc == 0x101);  // Only PC increment, no jump
    }
}

//NON ANCORA IMPLEMENTATE
TEST_CASE_METHOD(TestFixture, "Load 16-bit Instructions", "[instructions]") {
    SECTION("LD BC,nn") {
        // Set up PC to point to test instruction data
        registers.pc = 0x0100;
        // Write test data to memory
        memory.write(0x0100, 0x34);  // LSB
        memory.write(0x0101, 0x12);  // MSB
        // Execute the instruction
        instructions.loadImR16(&(registers.bc));
        // Check that BC contains the correct value
        REQUIRE(registers.bc == 0x1234);
        // Check that PC was incremented correctly
        REQUIRE(registers.pc == 0x0102);
    }
    
    SECTION("LD DE,nn") {
        registers.pc = 0x0100;
        memory.write(0x0100, 0x78);  // LSB
        memory.write(0x0101, 0x56);  // MSB
        instructions.loadImR16(&(registers.de));
        REQUIRE(registers.de == 0x5678);
        REQUIRE(registers.pc == 0x0102);
    }
    
    SECTION("LD HL,nn") {
        registers.pc = 0x0100;
        memory.write(0x0100, 0xAB);  // LSB
        memory.write(0x0101, 0xCD);  // MSB
        instructions.loadImR16(&(registers.hl));
        REQUIRE(registers.hl == 0xCDAB);
        REQUIRE(registers.pc == 0x0102);
    }
    
    SECTION("LD SP,nn") {
        registers.pc = 0x0100;
        memory.write(0x0100, 0xEF);  // LSB
        memory.write(0x0101, 0x89);  // MSB
        instructions.loadImR16(&(registers.sp));
        REQUIRE(registers.sp == 0x89EF);
        REQUIRE(registers.pc == 0x0102);
    }
    
    SECTION("LD SP,HL") {
        registers.hl = 0xABCD;
        instructions.loadSpHl();
        REQUIRE(registers.sp == 0xABCD);
    }
    
    SECTION("PUSH BC") {
        registers.bc = 0x1234;
        registers.sp = 0xFFFE;
        instructions.pushR(&(registers.bc));
        REQUIRE(memory.read(0xFFFD) == 0x12);  // MSB
        REQUIRE(memory.read(0xFFFC) == 0x34);  // LSB
        REQUIRE(registers.sp == 0xFFFC);
    }
    
    SECTION("POP BC") {
        registers.sp = 0xFFFC;
        memory.write(0xFFFC, 0x34);  // LSB
        memory.write(0xFFFD, 0x12);  // MSB
        instructions.popR(&(registers.bc));
        REQUIRE(registers.bc == 0x1234);
        REQUIRE(registers.sp == 0xFFFE);
    }
    
    SECTION("PUSH DE") {
        registers.de = 0x5678;
        registers.sp = 0xFFFE;
        instructions.pushR(&(registers.de));
        REQUIRE(memory.read(0xFFFD) == 0x56);  // MSB
        REQUIRE(memory.read(0xFFFC) == 0x78);  // LSB
        REQUIRE(registers.sp == 0xFFFC);
    }
    
    SECTION("POP DE") {
        registers.sp = 0xFFFC;
        memory.write(0xFFFC, 0x78);  // LSB
        memory.write(0xFFFD, 0x56);  // MSB
        instructions.popR(&(registers.de));
        REQUIRE(registers.de == 0x5678);
        REQUIRE(registers.sp == 0xFFFE);
    }
    
    SECTION("PUSH HL") {
        registers.hl = 0xABCD;
        registers.sp = 0xFFFE;
        instructions.pushR(&(registers.hl));
        REQUIRE(memory.read(0xFFFD) == 0xAB);  // MSB
        REQUIRE(memory.read(0xFFFC) == 0xCD);  // LSB
        REQUIRE(registers.sp == 0xFFFC);
    }
    
    SECTION("POP HL") {
        registers.sp = 0xFFFC;
        memory.write(0xFFFC, 0xCD);  // LSB
        memory.write(0xFFFD, 0xAB);  // MSB
        instructions.popR(&(registers.hl));
        REQUIRE(registers.hl == 0xABCD);
        REQUIRE(registers.sp == 0xFFFE);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Load 8-bit Immediate Instructions", "[instructions]") {
    SECTION("LD B,n") {
        memory.write(registers.pc, 0x42);
        instructions.loadImR8(&registers.b);
        REQUIRE(registers.b == 0x42);
        REQUIRE(registers.pc == 1);
    }
    
    SECTION("LD C,n") {
        memory.write(registers.pc, 0x24);
        instructions.loadImR8(&registers.c);
        REQUIRE(registers.c == 0x24);
        REQUIRE(registers.pc == 1);
    }
    
    SECTION("LD D,n") {
        memory.write(registers.pc, 0x5A);
        instructions.loadImR8(&registers.d);
        REQUIRE(registers.d == 0x5A);
        REQUIRE(registers.pc == 1);
    }
    
    SECTION("LD H,n") {
        memory.write(registers.pc, 0x7B);
        instructions.loadImR8(&registers.h);
        REQUIRE(registers.h == 0x7B);
        REQUIRE(registers.pc == 1);
    }
    
    SECTION("LD A,n") {
        memory.write(registers.pc, 0x3F);
        instructions.loadImR8(&registers.a);
        REQUIRE(registers.a == 0x3F);
        REQUIRE(registers.pc == 1);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Load 8-bit Increment/Decrement Instructions", "[instructions]") {
    SECTION("LD A,(HL+)") {
        registers.hl = 0x1234;
        memory.write(0x1234, 0x42);
        instructions.loadHlAPlus();
        REQUIRE(registers.a == 0x42);
        REQUIRE(registers.hl == 0x1233);  // Decrement HL
    }
    
    SECTION("LD A,(HL-)") {
        registers.hl = 0x1234;
        memory.write(0x1234, 0x42);
        instructions.loadHlAMinus();
        REQUIRE(registers.a == 0x42);
        REQUIRE(registers.hl == 0x1235);  // Increment HL
    }
    
    SECTION("LD (HL+),A") {
        registers.hl = 0x1234;
        registers.a = 0x42;
        instructions.loadAHlPlus();
        REQUIRE(memory.read(0x1234) == 0x42);
        REQUIRE(registers.hl == 0x1235);  // Increment HL
    }
    
    SECTION("LD (HL-),A") {
        registers.hl = 0x1234;
        registers.a = 0x42;
        instructions.loadAHlMinus();
        REQUIRE(memory.read(0x1234) == 0x42);
        REQUIRE(registers.hl == 0x1233);  // Decrement HL
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Load 8-bit Memory Instructions", "[instructions]") {
    SECTION("LD B,(HL)") {
        registers.hl = 0x1234;
        memory.write(0x1234, 0x42);
        instructions.loadHlR(&registers.b);
        REQUIRE(registers.b == 0x42);
    }
    
    SECTION("LD C,(HL)") {
        registers.hl = 0x1234;
        memory.write(0x1234, 0x24);
        instructions.loadHlR(&registers.c);
        REQUIRE(registers.c == 0x24);
    }
    
    SECTION("LD (HL),B") {
        registers.hl = 0x1234;
        registers.b = 0x42;
        instructions.loadRHl(&registers.b);
        REQUIRE(memory.read(0x1234) == 0x42);
    }
    
    SECTION("LD (HL),C") {
        registers.hl = 0x1234;
        registers.c = 0x24;
        instructions.loadRHl(&registers.c);
        REQUIRE(memory.read(0x1234) == 0x24);
    }
    
    SECTION("LD (HL),n") {
        registers.hl = 0x1234;
        memory.write(registers.pc, 0x42);
        instructions.loadImHl();
        REQUIRE(memory.read(0x1234) == 0x42);
        REQUIRE(registers.pc == 1);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Load 8-bit Register-to-Register Instructions", "[instructions]") {
    SECTION("LD B,B") {
        registers.b = 0x12;
        instructions.loadRR8(&registers.b, &registers.b);
        REQUIRE(registers.b == 0x12);
    }

    SECTION("LD B,C") {
        registers.b = 0x12;
        registers.c = 0x34;
        instructions.loadRR8(&registers.b, &registers.c);
        REQUIRE(registers.b == 0x34);
    }
    
    SECTION("LD D,E") {
        registers.d = 0x56;
        registers.e = 0x78;
        instructions.loadRR8(&registers.d, &registers.e);
        REQUIRE(registers.d == 0x78);
    }
    
    SECTION("LD H,L") {
        registers.h = 0x9A;
        registers.l = 0xBC;
        instructions.loadRR8(&registers.h, &registers.l);
        REQUIRE(registers.h == 0xBC);
    }
    
    SECTION("LD A,B") {
        registers.a = 0xDE;
        registers.b = 0xF0;
        instructions.loadRR8(&registers.a, &registers.b);
        REQUIRE(registers.a == 0xF0);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Load 8-bit Special Instructions", "[instructions]") {
    SECTION("LD A,(BC)") {
        registers.bc = 0x1234;
        memory.write(0x1234, 0x42);
        instructions.loadRA(&registers.bc);
        REQUIRE(registers.a == 0x42);
    }
    
    SECTION("LD A,(DE)") {
        registers.de = 0x1234;
        memory.write(0x1234, 0x42);
        instructions.loadRA(&registers.de);
        REQUIRE(registers.a == 0x42);
    }
    
    SECTION("LD (BC),A") {
        registers.bc = 0x1234;
        registers.a = 0x42;
        instructions.loadAR(&registers.bc);
        REQUIRE(memory.read(0x1234) == 0x42);
    }
    
    SECTION("LD (DE),A") {
        registers.de = 0x1234;
        registers.a = 0x42;
        instructions.loadAR(&registers.de);
        REQUIRE(memory.read(0x1234) == 0x42);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Load Address Instructions", "[instructions]") {
    SECTION("LD A,(nn)") {
        instructions.loadAdA();
    }
    
    SECTION("LD (nn),A") {
        instructions.loadAAd();
    }

    SECTION("LDH A,(n)") {
        instructions.loadHAIm();
    }
    
    SECTION("LDH (n),A") {
        instructions.loadHImA();
    }

    SECTION("LDH A,(C)") {
        instructions.loadHCA();
    }

    SECTION("LDH (C),A") {
        instructions.loadHAC();
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Load Immediate Instructions", "[instructions]") {
    SECTION("LD B,n") {
        memory.write(registers.pc, 0x42);
        instructions.loadImR8(&registers.b);
        REQUIRE(registers.b == 0x42);
        REQUIRE(registers.pc == 1);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Load Instructions with HL", "[instructions]") {
    SECTION("LD (HL),n") {
        registers.hl = 0x8000;
        memory.write(registers.pc, 0x42);
        instructions.loadImHl();
        uint8_t value = memory.read(0x8000);
        REQUIRE(value == 0x42);
        REQUIRE(registers.pc == 1);
    }
    
    SECTION("LD (HL+),A") {
        registers.hl = 0x8000;
        registers.a = 0x42;
        instructions.loadAHlPlus();
        uint8_t value = memory.read(0x8000);
        REQUIRE(value == 0x42);
        REQUIRE(registers.hl == 0x8001);
    }
    
    SECTION("LD (HL-),A") {
        registers.hl = 0x8000;
        registers.a = 0x42;
        instructions.loadAHlMinus();
        uint8_t value = memory.read(0x8000);
        REQUIRE(value == 0x42);
        REQUIRE(registers.hl == 0x7FFF);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Logical Operations", "[instructions]") {
    SECTION("AND A,B") {
        registers.a = 0b11110000;
        registers.b = 0b10101010;
        instructions.andAR(&registers.b);
        REQUIRE(registers.a == 0b10100000);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == false);
        REQUIRE(registers.isFlagSet(RegistersFlags::HALF_CARRY_FLAG) == true);
    }

    SECTION("OR A,B") {
        registers.a = 0b11110000;
        registers.b = 0b00001111;
        instructions.orAR(&registers.b);
        REQUIRE(registers.a == 0b11111111);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == false);
    }

    SECTION("XOR A,B") {
        registers.a = 0b11110000;
        registers.b = 0b11111111;
        instructions.xorAR(&registers.b);
        registers.printRegistersBit();
        REQUIRE(registers.a == 0b00001111);
        REQUIRE(registers.isFlagSet(RegistersFlags::ZERO_FLAG) == false);
    }
}

//OK
TEST_CASE_METHOD(TestFixture, "Rotate Instructions", "[instructions]") {
    SECTION("RLA") {
        registers.a = 0b10110011;
        registers.setFlag(RegistersFlags::CARRY_FLAG, true);
        instructions.rla();
        REQUIRE(registers.a == 0b01100111);
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == true);
    }

    SECTION("RLCA") {
        registers.a = 0b10110011;
        instructions.rlcR(&registers.a);
        REQUIRE(registers.a == 0b01100111);
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == true);
    }
}

//NON ANCORA IMPLEMENTATE
TEST_CASE_METHOD(TestFixture, "Shift Instructions", "[instructions]") {
    SECTION("SLA r") {
        registers.b = 0b10000001;
        instructions.slaR(&registers.b);
        REQUIRE(registers.b == 0b00000010);
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == true);
    }

    SECTION("SRA r") {
        registers.b = 0b10000001;
        instructions.sraR(&registers.b);
        REQUIRE(registers.b == 0b11000000);
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == true);
    }

    SECTION("SWAP r") {
        registers.b = 0xAB;
        instructions.swapR(&registers.b);
        REQUIRE(registers.b == 0xBA);
    }

    SECTION("SRL r") {
        registers.b = 0b10000001;
        instructions.srlR(&registers.b);
        REQUIRE(registers.b == 0b01000000);
        REQUIRE(registers.isFlagSet(RegistersFlags::CARRY_FLAG) == true);
    }
}

//NON ANCORA IMPLEMENTATE
TEST_CASE_METHOD(TestFixture, "Stack Operations", "[instructions]") {
    SECTION("PUSH and POP AF") {
        registers.af = 0x1234;
        registers.sp = 0xFFFE;
        instructions.pushR(&registers.af);
        registers.af = 0;  // Clear AF
        instructions.popR(&registers.af);
        REQUIRE(registers.af == 0x1234);
    }

    SECTION("LD SP,E + n") {
        registers.sp = 0x1000;
        registers.e = 0x05;
        instructions.loadSpEHl();
        REQUIRE(registers.hl == 0x1005);
    }
}