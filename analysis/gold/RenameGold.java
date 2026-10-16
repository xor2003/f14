import ghidra.app.script.GhidraScript;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.*;
import java.io.*;

public class RenameGold extends GhidraScript {
    public void run() throws Exception {
        BufferedReader r = new BufferedReader(new FileReader("/tmp/f14gold/rename_gold.txt"));
        String l; int n=0, dup=0;
        while ((l=r.readLine())!=null) {
            String[] p=l.split(" ");
            Address a=toAddr(p[0]);
            String name=p[1];
            ghidra.program.model.listing.Function f=getFunctionAt(a);
            if (f==null) continue;
            try {
                f.setName(name, SourceType.IMPORTED);
                n++;
            } catch (Exception e) {
                // duplicate name -> suffix
                try { f.setName(name+"_dup"+(dup++), SourceType.IMPORTED); n++; } catch(Exception e2){}
            }
        }
        r.close();
        println("renamed "+n+" functions");
    }
}
