package org.vanautrui.languages.vmcompiler.codegenerator;

import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;
import org.vanautrui.languages.vmcompiler.AssemblyWriter;
import org.vanautrui.languages.vmcompiler.instructions.VMInstr;
import org.vanautrui.languages.vmcompiler.model.Register;

import static org.vanautrui.languages.vmcompiler.codegenerator.StackFocusedAssemblyCodeGenerator.compile_push;
import static org.vanautrui.languages.vmcompiler.codegenerator.StackFocusedAssemblyCodeGenerator.compile_swap;
import static org.vanautrui.languages.vmcompiler.model.Register.*;

/**
 * contains the builtin subroutines of dragon, in assembly.
 * These subroutines are to be added at the end of the code to every .asm file generated by the dragon vm compiler.
 *
 * These subroutines must behave like any other subroutine when called.
 */

public class BuiltinSubroutinesToBeAddedOnceToEveryAssemblyFile {

  /**
   * Compiles all builtin subroutines.
   * This is so that we can call this subroutine in assembly code generation,
   * and do not forget a subroutine.
   * So a new subroutine can be added easily
   */
  public static void compile_all_builtin_subroutines(AssemblyWriter a)throws Exception{
    compile_readchar(a);
    compile_putchar(a);
    compile_putdigit(a);

    compile_malloc(a);
    compile_free(a);
  }

  /**
   * reads a single character from stdin
   */
  private static void compile_readchar(AssemblyWriter a){
    final String name="readchar: ";

    //TODO: handle errors that could occur

    SubroutineFocusedAssemblyCodeGenerator.compile_subroutine("readchar",a);

    //push a placeholder for the value to read on the stack
    a.mov(eax,0);
    a.push(eax);

    //SYSCALL START
    a.mov(eax,3,name+"sys_read");
    a.mov(ebx,0,name+"stdin");

    //print the char on stack
    a.mov(ecx, esp,name+"read into the placeholder DWORD we pushed onto the stack");

    //val length
    a.mov(Register.edx,1,name+"value length");
    a.call_kernel();
    //SYSCALL END

    //push return value: we do not need to push a return value,
    //as we already pushed a placeholder where our char (which has been read by now)
    // should have been placed

    //we must swap return value with the return address in order to return
    //(i am so dumb. took me so long to find this.)
    compile_swap(name+"swap return address with return value to return",a);

    //return from subroutine
    SubroutineFocusedAssemblyCodeGenerator.compile_return(a);
  }

  private static void compile_free(AssemblyWriter a){
    final String name = "free";
    //TODO: not yet implemented
  }

  private static void compile_malloc(AssemblyWriter a) throws Exception{
    //http://lxr.linux.no/#linux+v3.6/arch/x86/ia32/ia32entry.S#L372
    //http://man7.org/linux/man-pages/man2/mmap.2.html

    //TODO: handle the error if memory could not be allocated
    final String name="malloc";
    //malloc receives as an argument the amount of DWORDs to allocate

    SubroutineFocusedAssemblyCodeGenerator.compile_subroutine("malloc",a);

    //access our argument, push it onto the stack
    compile_push(DracoVMCodeWriter.SEGMENT_ARG,0,a);

    //this is to multiply by 4, so we allocate 32bit units.
    a.pop(ecx,"size of segment to be allocated"); //pop our argument into ecx
    a.mov(eax,4);
    a.mul_eax_with(ecx);
    a.mov(ecx,eax);


    a.mov(eax,192,"192 : mmap system call");
    a.xor(ebx,ebx,"addr=NULL");
    a.mov(edx,0x7,"prot = PROT_READ | PROT_WRITE | PROT_EXEC");
    a.mov(esi,0x22,"flags=MAP_PRIVATE | MAP_ANONYMOUS");
    a.mov(edi,-1,"fd=-1");

    a.push(ebp,"save ebp as we should not mess with it"); //LINKED CODE 1 (they only work together)

    a.mov(ebp,0,"offset=0 (4096*0)");
    a.call_kernel();

    a.pop(ebp,"restore ebp as we should not mess with it"); //LINKED CODE 1 (they only work together)

    //eax should now contain
    //the address of the allocated memory segment

    //now we push that pointer on the stack
    a.push(eax);

    //we must swap return value with the return address in order to return
    //(i am so dumb. took me so long to find this.)
    compile_swap(name+"swap return address with return value to return",a);

    //return from subroutine
    SubroutineFocusedAssemblyCodeGenerator.compile_return(a);
  }

  private static void compile_putchar(AssemblyWriter a)throws Exception{
    //prints top of stack as ascii char to stdout

    SubroutineFocusedAssemblyCodeGenerator.compile_subroutine("putchar",a);

    //access our argument , ARG 0, by pushing it onto the stack
    compile_push(DracoVMCodeWriter.SEGMENT_ARG,0,a);

    a.mov(eax,4,"putchar: sys_write");
    a.mov(ebx,1,"putchar: std_out");

    //print the char on stack
    a.mov(ecx, esp,"putchar: print the char on the stack");


    //val length
    a.mov(Register.edx,1,"putchar: value length");
    a.call_kernel();

    //remove our argument which we had pushed
    a.pop(eax,"putchar: remove the ARG 0 which we had pushed");

    //push return value
    a.mov(Register.edx,0,"putchar: push return value");
    a.push(Register.edx,"putchar: push return value");

    //we must swap return value with the return address in order to return
    //(i am so dumb. took me so long to find this.)
    compile_swap("swap return address with return value to return",a);

    //return from subroutine
    SubroutineFocusedAssemblyCodeGenerator.compile_return(a);
  }

  private static void compile_putdigit(AssemblyWriter a) throws Exception{
    //prints the Int on top of stack as char to stdout
    SubroutineFocusedAssemblyCodeGenerator.compile_subroutine("putdigit",a);
    final String name="putdigit";

    //access our argument , ARG 0, by pushing it onto the stack
    compile_push(DracoVMCodeWriter.SEGMENT_ARG,0,a);

    a.mov(eax,4,name+" sys_write");
    a.mov(ebx,1,name+" std_out");

    //duplicate the value on stack, add offset to make it a char
    a.pop(ecx);
    a.push(ecx);
    a.add(ecx,48,name+" add offset to make it char");
    a.push(ecx);

    a.mov(ecx, esp,name+" print the Int on the stack");

    //val length
    a.mov(Register.edx,1,name+" value length");
    a.call_kernel();

    //pop that value which we pushed
    a.pop(ecx);

    //pop ARG 0 which we pushed
    a.pop(ecx);

    //push return value
    a.mov(Register.edx,0,name+" push return value");
    a.push(Register.edx,name+" push return value");

    //we must swap return value with the return address in order to return
    //(i am so dumb. took me so long to find this.)
    compile_swap("swap return address with return value to return",a);

    SubroutineFocusedAssemblyCodeGenerator.compile_return(a);
  }

  private static void compile_float2int(VMInstr instr, AssemblyWriter a) throws Exception{
    throw new Exception("unhandled");
  }

  private static void compile_int2float(VMInstr instr, AssemblyWriter a) throws Exception{
    throw new Exception("unhandled");
  }

  private static void compile_int2char(VMInstr instr, AssemblyWriter a) throws Exception{
    throw new Exception("unhandled");
  }

  private static void compile_readint(VMInstr instr, AssemblyWriter a)throws Exception{
    throw new Exception("unhandled");
  }
}
