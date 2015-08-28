import it.unimi.dsi.mg4j.io.*;
import java.io.*;

/**
 * Test harness for output bitstream
 */
public class OutputBitstreamTestMinimal {
    public static void main( String[] args ) throws Exception {
        PrintStream ps = new PrintStream( new BufferedOutputStream( new FileOutputStream( "java_obs_log" )));

        byte[] dat = new byte[5];

        DebugOutputBitStream obs = new DebugOutputBitStream( new OutputBitStream( dat ), ps );

        // Call a bunch of methods.

        PrintWriter out = new PrintWriter(ps);
//         obs.writeGamma( 5 );
//         logArray( dat, out );
//         obs.writeUnary( 0 );
//         logArray( dat, out );
//         obs.writeGamma( 0 );
//         logArray( dat, out );
//         obs.writeZeta( 44, 5 );
//         logArray( dat, out );
//         obs.writeZeta( 15, 5 );
//         logArray( dat, out );
//         obs.writeZeta( 33, 5 );
//         logArray( dat, out );
//         obs.writeZeta( 2, 5 );
//         logArray( dat, out );
//         obs.writeZeta( 12, 5 );
//         logArray( dat, out );
        obs.writeGamma( 1 );
        logArray( dat, out );
        obs.writeUnary( 0 );
        logArray( dat, out );
        obs.writeGamma( 0 );
        logArray( dat, out );
        obs.writeZeta( 168, 5 );
        logArray( dat, out );
        out.close();
    }

    public static void logArray( byte[] ary, PrintWriter pw ) {
        pw.print( "		" );
        
        for( int i = 0; i < ary.length; i++ ) {
            String str = Integer.toHexString( ary[i] );
            // only the last two digits count.
            str = str.substring( (int)Math.max(str.length() - 2, 0), str.length() );

            pw.print( str + " " );
        }

        pw.println();

        pw.flush();
    }
}
