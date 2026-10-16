import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;
import ghidra.program.model.mem.MemoryBlock;
import java.util.*;

public class DumpFuncs extends GhidraScript {
    static long lin(Address a){ // "2d81:0438" -> 0x2d810+0x438
        String s=a.toString(); int c=s.indexOf(':');
        return Long.parseLong(s.substring(0,c),16)*16+Long.parseLong(s.substring(c+1),16);
    }
    public void run() throws Exception {
        PrintWriter w = new PrintWriter("/tmp/f14ghidra/funcs.txt");
        PrintWriter r = new PrintWriter("/tmp/f14ghidra/ranges.txt");
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
            w.println(f.getEntryPoint() + " " + f.getName() + " " + f.getBody().getNumAddresses());
            InstructionIterator it = currentProgram.getListing().getInstructions(f.getBody(), true);
            List<long[]> ranges = new ArrayList<>();
            long rs=-1, re=-1;
            while (it.hasNext()) {
                Instruction ins = it.next();
                long s=lin(ins.getMinAddress()), e=s+ins.getLength();
                if (rs<0){rs=s;re=e;}
                else if (s<=re) re=Math.max(re,e);
                else { ranges.add(new long[]{rs,re}); rs=s;re=e; }
            }
            if (rs>=0) ranges.add(new long[]{rs,re});
            StringBuilder sb=new StringBuilder();
            sb.append(f.getEntryPoint().toString()).append(' ');
            for (long[] rr:ranges) sb.append(String.format("%x-%x ",rr[0],rr[1]));
            r.println(sb.toString().trim());
        }
        w.close(); r.close();
        PrintWriter m = new PrintWriter("/tmp/f14ghidra/memmap.txt");
        for (MemoryBlock b : currentProgram.getMemory().getBlocks())
            m.println(b.getName() + " " + b.getStart() + " " + b.getEnd() + " size=" + b.getSize());
        m.close();
    }
}
